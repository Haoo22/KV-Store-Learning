#include <iostream>
#include "calc.h"  // 引入菜单，这样编译器就知道 add 和 sub 是什么

int main() {
    int x = 10;
    int y = 5;
    
    std::cout << "计算开始..." << std::endl;
    std::cout << x << " + " << y << " = " << add(x, y) << std::endl;
    std::cout << x << " - " << y << " = " << sub(x, y) << std::endl;
    
    return 0;
}