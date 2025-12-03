#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main() {
    std::ifstream infile("data.txt");
    if (!infile.is_open()) {
        std::cerr << "文件打开失败！" << std::endl;
        return 1;
    }
    std::ofstream outfile("result.txt");
    if (!outfile.is_open()) {
        std::cerr << "文件创建失败！" << std::endl;
        return 1;
    }

    std::vector<int> v;
    for (int x; infile >> x; ) {
        v.push_back(x);
    }
    std::sort(v.begin(), v.end());

    for (int i = 0; i < v.size(); i++) {
        outfile << v[i] << " \n"[i + 1 == v.size()];
    }

    infile.close();
    outfile.close();

    std::cout << "Sort done! Check result.txt" << std::endl;

    return 0;

}