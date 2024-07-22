cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(SocketCommunication VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含服务器端和客户端的子目录
add_executable(server server.cpp)
add_executable(client client.cpp)
