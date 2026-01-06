#include <iostream>
#include <cassert> // 引入断言库
#include <string>
#include "../include/skiplist.h"

#define FILE_PATH "./dump_file"

int main() {
    // 1. 初始化 KV Store
    // 使用默认参数，但在测试时我们显式指定路径，确保干净
    KVStore::SkipList<int, std::string> skipList(6, 0.5, FILE_PATH);

    std::cout << "=== 开始全面 CRUD 测试 ===" << std::endl;

    // ==========================================
    // 🟢 测试 1: 增加 (Insert)
    // ==========================================
    std::cout << "[Test 1] 插入基础数据..." << std::endl;
    skipList.insert(1, "Value_1");
    skipList.insert(3, "Value_3");
    skipList.insert(5, "Value_5");
    
    // 验证：查一下看在不在
    std::string val;
    assert(skipList.search(1, val) && val == "Value_1");
    assert(skipList.search(3, val) && val == "Value_3");
    std::cout << "✅ 插入测试通过" << std::endl;

    // ==========================================
    // 🟡 测试 2: 修改 (Update) -> 也是 Insert
    // ==========================================
    std::cout << "[Test 2] 修改数据 (Key=1)..." << std::endl;
    // 再次插入 Key=1，Value 变了，应该覆盖
    skipList.insert(1, "Value_1_Updated");
    
    // 验证：必须是新值
    assert(skipList.search(1, val));
    if (val == "Value_1_Updated") {
        std::cout << "✅ 修改测试通过 (Key=1 新值为: " << val << ")" << std::endl;
    } else {
        std::cerr << "❌ 修改失败！当前值为: " << val << std::endl;
        return -1;
    }

    // ==========================================
    // 🔵 测试 3: 查询 (Search) 边界
    // ==========================================
    std::cout << "[Test 3] 查询不存在的数据..." << std::endl;
    // 查一个根本没插过的 Key
    if (!skipList.search(999, val)) {
        std::cout << "✅ 正确：Key=999 不存在" << std::endl;
    } else {
        std::cerr << "❌ 错误：Key=999 不应该存在，但查到了: " << val << std::endl;
        return -1;
    }

    // ==========================================
    // 🔴 测试 4: 删除 (Erase)
    // ==========================================
    std::cout << "[Test 4] 删除数据 (Key=3)..." << std::endl;
    skipList.erase(3);
    
    // 验证：现在应该查不到了
    assert(!skipList.search(3, val));
    std::cout << "✅ 删除测试通过 (Key=3 已消失)" << std::endl;

    // ==========================================
    // 🟠 测试 5: 删除边界 (删一个不存在的)
    // ==========================================
    std::cout << "[Test 5] 删除不存在的 Key=999..." << std::endl;
    // 如果你的 erase 逻辑没写好（比如没判空），这里会 Segfault
    skipList.erase(999); 
    std::cout << "✅ 边界删除测试通过 (程序未崩溃)" << std::endl;

    // --- 打印最终状态，肉眼确认 ---
    std::cout << "\n=== 最终跳表结构 ===" << std::endl;
    skipList.display_list();

    std::cout << "\n🎉 全部 CRUD 测试通过！准备退出并自动保存数据..." << std::endl;
    return 0;
}