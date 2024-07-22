cmake_minimum_required(VERSION 3.12)
project(MysqlConnPool)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE DEBUG)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
add_compile_options(-std=c++11 -pthread -g  -ggdb -O0 -Wall
                        -fstack-protector-all -march=native -fnon-call-exceptions)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src DIR_SRCS)
SET(SRC_DIR ${DIR_SRCS})

file(GLOB example_files RELATIVE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/app/*.cpp
        )

SET(EXTRA_LIBS
        ${EXTRA_LIBS}
        )

link_directories(/usr/lib/mysql)

foreach(example_file ${example_files})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file} ${SRC_DIR})
    target_link_libraries(${example_name} ${EXTRA_LIBS} libmysqlclient.so pthread)
endforeach(example_file ${example_files})


