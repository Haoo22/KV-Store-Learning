#pragma onec
#include <iostream> 
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>

namespace KVStore {

    // 前置声明，为了让 Node 知道 SkipList 的存在
    template <typename K, typename V> 
    class SkipList;

    template<typename K, typename V>
    class Node{
        friend class SkipList<K, V>;
    public:
        Node() {}
        Node(K k, V v, int level);
        ~Node();

        K get_key() const;
        V get_value() const;
        void set_value(V value);
    
        private:
            K key;
            V value;
            Node<K, V> **forward;
            int node_level;
    };

    template <typename K, typename V>
    class SkipList {
    public:
        SkipList(int max_level);
        ~SkipList();

        int insert(K key, V value);
        bool search(K key, V& value);
        void erase(K key);
        void display_list();

    private:
        // --- 内部工具函数 ---
        int get_random_level();
        Node<K, V>* create_node(K k, V v, int level);

    private:
        int _max_level;
        int _skip_list_level;
        Node<K, V> *_header;
        
        int _element_count;
        std::mutex _mtx;
    };
}