# 设置 CMake 的最小版本要求
cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(PipeExample VERSION 1.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加可执行文件
add_executable(linux_messageQue linux_messageQue.cpp)
add_executable(linux_pipe linux_pipe.cpp)