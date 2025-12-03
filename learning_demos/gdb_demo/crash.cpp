#include <iostream>

void dangerous_function(int* ptr) {
    // 这里的 ptr 可能是空的，直接访问会崩溃
    *ptr = 100; 
}

void normal_function() {
    int* p = nullptr; // 定义一个空指针
    std::cout << "准备进入危险区域..." << std::endl;
    dangerous_function(p); // 传进去
    std::cout << "这行字永远不会打印出来" << std::endl;
}

int main() {
    std::cout << "程序启动" << std::endl;
    normal_function();
    return 0;
}