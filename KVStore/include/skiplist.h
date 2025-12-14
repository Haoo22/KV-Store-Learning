#pragma once
#include <iostream> 
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <random>

namespace KVStore {

    // -------------------------------------------
    // 序列化 (To Binary String)
    // -------------------------------------------

    // 默认版本：处理 int, double, char 等 POD (Plain Old Data) 类型
    template <typename T>
    std::string to_string(const T& value) {
        // 编译期安全检查：如果 T 不是“简单可复制”的（比如包含 std::string），直接报错！
        static_assert(std::is_trivially_copyable<T>::value, "危险！此类型包含指针或复杂对象，必须手动实现 to_string 特化！");
        return std::string((const char*)&value, sizeof(T));
    }

    // 特化版本：处理 std::string (已经是二进制流了，直接返回)
    template <>
    std::string to_string(const std::string& value) {
        return value;
    }

    // -------------------------------------------
    // 反序列化 (From Binary String)
    // -------------------------------------------

    // 默认版本：处理 POD 类型
    template <typename T>
    void from_string(const std::string& str, T& value) {
        // 编译期安全检查：如果 T 不是“简单可复制”的（比如包含 std::string），直接报错！
        static_assert(std::is_trivially_copyable<T>::value, "危险！此类型包含指针或复杂对象，必须手动实现 from_string 特化！");
        memcpy(&value, str.c_str(), sizeof(T));
    }

    // 特化版本：处理 std::string
    template <>
    void from_string(const std::string& str, std::string& value) {
        value = str;
    }

    // 前置声明，为了让 Node 知道 SkipList 的存在
    template <typename K, typename V> 
    class SkipList;

    template<typename K, typename V>
    class Node{
        friend class SkipList<K, V>;
    public:
        Node() {}
        
        Node(K k, V v, int level) {
            this->key = k;
            this->value = v;
            this->node_level = level;
            this->forward = new Node<K, V>*[level + 1];
            memset(this->forward, 0, sizeof(Node<K, V>*) * (level + 1));
        }
        
        ~Node() { delete[] forward; }

        K get_key() const { return key; }
        V get_value() const { return value; }
    
        private:
            K key;
            V value;
            Node<K, V> **forward;
            int node_level;
    };

    template <typename K, typename V>
    class SkipList {
    public:
        SkipList(int max_level, double probability = 0.5) 
            : _max_level(max_level), 
              _skip_list_level(0), 
              _element_count(0),
              _rng(std::random_device{}()), 
              _dist(probability) { 
            
            K k; V v;
            _header = new Node<K, V>(k, v, _max_level);
        }

        // 释放内存，防止内存泄露
        ~SkipList() {
            if (_header) {
                Node<K, V> *current = _header->forward[0];
                
                while(current != nullptr) {
                    Node<K, V> *temp = current;      // 先记下当前节点
                    current = current->forward[0];   // 指针后移
                    delete temp;                     // 删掉当前节点
                }

                delete _header;
            }
        }

        int insert(K key, V value);
        bool search(K key, V& value);
        void erase(K key);
        void display_list();

        // --- 持久化接口 ---
        
        // 将内存数据全量写入文件
        void dump_file();
        // 从文件加载数据到内存
        void load_file();

    private:
        // --- 内部工具函数 ---
        int get_random_level() {
            int k = 0;
            while (_dist(_rng)) {
                k++;
            }
            k = (k < _max_level) ? k : _max_level;
            return k;
        }
        
        Node<K, V>* create_node(K k, V v, int level) {
            Node<K, V> *n = new Node<K, V>(k, v, level);
            return n;
        }

    private:
        int _max_level;
        int _skip_list_level;
        Node<K, V> *_header;
        
        int _element_count;
        std::mutex _mtx;

        // 随机化参数相关的成员变量
        std::mt19937 _rng;
        std::bernoulli_distribution _dist;

        // 文件操作相关的成员变量
        std::string _file_path = "dump_file"; // 默认存储文件名
        std::ofstream _file_writer;
        std::ifstream _file_reader;
    };

    template<typename K, typename V>
    int SkipList<K, V>::insert(K key, V value) {
        std::lock_guard<std::mutex> lock(_mtx); // 上锁

        Node<K, V> *current = _header;
        // update数组：用来记录每一层“该在谁后面插入”
        std::vector<Node<K, V>*> update(_max_level + 1, nullptr);

        for (int i = _skip_list_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }

            update[i] = current;
        }

        current = current->forward[0];

        // 如果 key 存在，则更新 value
        if (current != nullptr && current->key == key) {
            current->value = value;
            return 1; // 返回 1 代表更新
        } 

        // 如果 key 不存在，执行插入
        int random_level = get_random_level();

        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level + 1; i <= random_level; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        Node<K, V>* insert_node = create_node(key, value, random_level);

        for (int i = 0; i <= random_level; i++) {
            insert_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = insert_node;
        }

        _element_count++;

        return 0; // 返回 0 代表插入成功
    }

    template<typename K, typename V>
    bool SkipList<K, V>::search(K key, V& value) {
        Node<K, V> *current = _header;

        for (int i = _skip_list_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            value = current->value;
            return true;
        }
        return false;
    }

    template<typename K, typename V>
    void SkipList<K, V>::erase(K key) {
        std::lock_guard<std::mutex> lock(_mtx);
        
        Node<K, V> *current = _header;
        std::vector<Node<K, V>*> update(_max_level + 1);

        for (int i = _skip_list_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            // 从下往上断开链接
            for (int i = 0; i <= _skip_list_level; i++) {
                if (update[i]->forward[i] != current) 
                    break;
                update[i]->forward[i] = current->forward[i];
            }

            // 移除空出的最高层
            while (_skip_list_level > 0 && _header->forward[_skip_list_level] == nullptr) {
                _skip_list_level--;
            }

            delete current;
            _element_count--;
        }
    }

    template<typename K, typename V>
    void SkipList<K, V>::display_list() {
        std::cout << "\n***** Skip List *****"<<"\n"; 
        for (int i = 0; i <= _skip_list_level; i++) {
            Node<K, V> *node = _header->forward[i];
            std::cout << "Level " << i << ": ";
            while (node != nullptr) {
                std::cout << node->get_key() << ":" << node->get_value() << "; ";
                node = node->forward[i];
            }
            std::cout << std::endl;
        }
    }
}