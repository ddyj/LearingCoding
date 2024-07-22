cmake_minimum_required(VERSION 3.10)  # 必须
set(CMAKE_CXX_STANDARD 11)	# C++14
project(main)  #工程名

set(OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)
FIND_PACKAGE(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(
    ./include
)
link_directories(./lib)
SET(EXTRA_LIBS
    ${EXTRA_LIBS}
    ${CMAKE_DL_LIBS}
    ${OpenCV_LIBS} 
)


ADD_EXECUTABLE( main ./app/main.cpp ./src/ShareMemory.cpp)
TARGET_LINK_LIBRARIES( main  ${EXTRA_LIBS} )
ADD_EXECUTABLE( fork ./app/fork.cpp  ./src/ShareMemory.cpp)
TARGET_LINK_LIBRARIES( fork  ${EXTRA_LIBS} )

