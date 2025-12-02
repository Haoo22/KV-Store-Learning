# 1. 规定 CMake 的最低版本要求（这是固定写法）
cmake_minimum_required(VERSION 3.10)

# 2. 给你的项目起个名字（给 CMake 内部看的）
project(KVStoreLearning)

# 3. 核心指令：生成可执行文件
# 格式：add_executable(生成的程序名 源文件1 源文件2 ...)
# 注意：不需要写 .h 文件
add_executable(app main.cpp calc.cpp)
