#pragma onec
#include <iostream> 
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <random>

namespace KVStore {

    // 前置声明，为了让 Node 知道 SkipList 的存在
    template <typename K, typename V> 
    class SkipList;

    template<typename K, typename V>
    class Node{
        friend class SkipList<K, V>;
    public:
        Node() {}
        
        Node(K k, V v, int level) {
            this->Key = k;
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

        ~SkipList() {
            // 还需要遍历删除所有节点
            if (_header) delete _header;
        }

        int insert(K key, V value);
        bool search(K key, V& value);
        void erase(K key);
        void display_list();

    private:
        // --- 内部工具函数 ---
        int get_random_level() {
            int k = 1;
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

        std::mt19937 _rng;
        std::bernoulli_distribution _dist;
    };

    template<typename K, typename V>
    int SkipList<K, V>::insert(K key, V value) {
        std::lock_guard<std::mutex> lock(_mtx); // 上锁

        Node<K, V> *current = _header;

        // update数组：用来记录每一层“该在谁后面插入”
        Node<K, V> *update[_max_level + 1];
        memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));

        for (int i = _skip_list_level; i >= 0; i--) {
            while (current->forward[i] != nullptr && current->forward[i]->key < key) {
                current = current->forward[i];
            }

            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key) {
            current->value = value;
            return 1; // 返回 1 代表更新
        } else {
            int random_level = get_random_level();

            _skip_list_level = std::max(_skip_list_level, random_level);

            Node<K, V>* insert_node = create_node(key, value, random_level);

            for (int i = 0; i <= random_level; i++) {
                insert_node->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = insert_node;
            }

            _element_count++;
        }

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
}