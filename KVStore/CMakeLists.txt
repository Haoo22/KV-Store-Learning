cmake_minimum_required(VERSION 3.10)
project(KVStoreEngine)

# 1. 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 告诉编译器头文件去哪找
include_directories(include)

# 3. 开启调试模式
set(CMAKE_BUILD_TYPE Debug)

# 它的作用相当于在 g++ 编译时加上了 -DDEBUG 参数
add_definitions(-DDEBUG)

# 告诉 CMake：所有生成的可执行文件，都放到项目根目录下的 bin 文件夹里
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 4. 生成可执行文件
add_executable(kv_server src/main.cpp)