cmake_minimum_required(VERSION 3.0)
project(MyProject)

# 设置 C++ 标准为 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


include_directories(${PROJECT_SOURCE_DIR}/include)


# 添加可执行文件并链接相关库
# 查找 pthread 库
find_package(Threads REQUIRED)

# 循环添加可执行文件并链接 pthread 库
set(EXAMPLES
        "code"
    )

foreach(EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ./${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE} Threads::Threads)
endforeach()
