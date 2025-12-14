#include <iostream>
#include "../include/skiplist.h"

int main() {
    // 1. 创建跳表对象
    // 注意：如果是生产环境，load_file 应该放在构造函数里自动调用
    KVStore::SkipList<int, std::string> skipList(6);

    // 2. 【关键】先尝试从文件加载数据
    skipList.load_file();

    std::cout << "--- 当前数据内容 ---" << std::endl;
    skipList.display_list(); // 如果之前运行过，这里应该能打印出旧数据

    // 3. 插入一些新数据 (每次运行都插不一样的，方便观察)
    // 我们可以用随机数或者手动修改这里的值来测试
    std::cout << "--- 插入新数据 ---" << std::endl;
    skipList.insert(100, "持久化测试");
    skipList.insert(200, "Day10完成");

    // 4. 再次打印确认
    std::cout << "--- 插入后数据内容 ---" << std::endl;
    skipList.display_list();

    // 5. 【关键】程序退出前保存数据
    skipList.dump_file();

    return 0;
}