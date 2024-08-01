cmake_minimum_required(VERSION 3.1)
PROJECT(datastruct)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(Sophus_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ICPCUDA/third-party/Sophus" CACHE PATH "Sophus Include directory")
include_directories(${Sophus_INCLUDE_DIR})
# cuda
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

# opencv
find_package(OpenCV REQUIRED)
if (OpenCV_INCLUDE_DIRS)
    include_directories("${OpenCV_INCLUDE_DIRS}")
    link_directories(${OpenCV_DIR}/lib)
endif (OpenCV_INCLUDE_DIRS)

# eigen
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
message("OPENMP FOUND")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# cuda
find_package(CUDA)
SET(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode=arch=compute_70,code=sm_70)

# pcl 
find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
add_definitions(${PCL_DEFINITIONS})

#TOML
include_directories(include)
AUX_SOURCE_DIRECTORY(Toml DIR_TOML_SRCS)
SET(TOML_DIR ${DIR_TOML_SRCS})
#ADD_LIBRARY(DispInfo SHARED ${DIR_TOML_SRCS})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
#open3d
set(Open3D_DIR /usr/local/lib/cmake/Open3D)  
set(Open3D_include /usr/local/include)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
list(APPEND Open3D_LIBRARIES dl)
find_package(Open3D HINTS REQUIRED)
if (Open3D_FOUND)
    message(STATUS "Found Open3D ${Open3D_VERSION}")
    link_directories(${Open3D_LIBRARY_DIRS})
endif()


find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})




# 包含头文件路径
include_directories(
	${PROJECT_INCLUDE_DIR})


set(CMAKE_CXX_STANDARD 14)
find_package(Boost REQUIRED COMPONENTS
    thread
)

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
    ${OpenCV_LIBS}
    #${Open3D_LIBS}
    )

foreach(example_file ${example_files})
    get_filename_component(example_name ${example_file} NAME_WE)
    cuda_add_executable(${example_name} ${example_file} ${SRC_DIR})
    target_link_libraries(${example_name} ${EXTRA_LIBS} ${Boost_LIBRARIES} ${PCL_LIBRARIES} -lpthread -lGL Open3D::Open3D)
endforeach(example_file ${example_files})

