cmake_minimum_required(VERSION 3.12)
project(ykf_workspace)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(
    ./include 
)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)
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
    target_link_libraries(${example_name} ${EXTRA_LIBS} mysqlclient pthread)
endforeach(example_file ${example_files})



