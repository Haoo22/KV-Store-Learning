cmake_minimum_required(VERSION 3.10)

project(SortDemo)

add_executable(sorter main.cpp)

# 解释：${CMAKE_CURRENT_SOURCE_DIR} 代表你 CMakeLists.txt 所在的目录
#       ${CMAKE_CURRENT_BINARY_DIR} 代表你运行 cmake 命令的目录 (即 build 目录)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})