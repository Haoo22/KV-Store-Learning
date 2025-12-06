#include <iostream>
#include "../include/skiplist.h"

int main() {
    // 1. 创建跳表，最大层数 6
    KVStore::SkipList<int, std::string> skipList(6);

    // 2. 插入一些数据
    std::cout << "=== 插入测试 ===" << std::endl;
    skipList.insert(1, "学C++");
    skipList.insert(3, "学Linux");
    skipList.insert(7, "写KV存储");
    skipList.insert(8, "拿Offer");
    skipList.insert(9, "去深圳");
    skipList.insert(19, "发大财");
    skipList.insert(19, "发大财(Update)"); // 测试更新

    // 3. 打印看看结构
    skipList.display_list();

    // 4. 查询测试
    std::cout << "\n=== 查询测试 ===" << std::endl;
    std::string val;
    if (skipList.search(1, val)) {
        std::cout << "Found key 1: " << val << std::endl;
    } else {
        std::cout << "Key 1 not found." << std::endl;
    }

    if (skipList.search(999, val)) {
        std::cout << "Found key 999: " << val << std::endl;
    } else {
        std::cout << "Key 999 not found (Correct)." << std::endl;
    }

    // 5. 删除一些数据
    std::cout << "\n=== 删除测试 ===" << std::endl;
    skipList.erase(1);
    std::cout << "erase key 1" << std::endl;
    if (skipList.search(1, val)) {
        std::cout << "Found key 1: " << val << std::endl;
    } else {
        std::cout << "Key 1 not found." << std::endl;
    }

    skipList.erase(999);
    std::cout << "erase key 999" << std::endl;
    if (skipList.search(999, val)) {
        std::cout << "Found key 999: " << val << std::endl;
    } else {
        std::cout << "Key 999 not found (Correct)." << std::endl;
    }

    return 0;
}