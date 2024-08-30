cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(ProjectName)

# 设置 C++ 标准版本
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(include)

# 添加源文件
set(SOURCES
    main.cpp
    a.cpp
    b.cpp
    c.cpp
    a.h
    b.h
    common.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})
