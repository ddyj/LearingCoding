cmake_minimum_required(VERSION 3.10)

project(MatrixMultiplication CUDA CXX)

# Find CUDA package
find_package(CUDA REQUIRED)

# Set CUDA architecture (change according to your GPU)
# set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} 
#                 -gencode arch=compute_52,code=sm_52)

# Add executable
cuda_add_executable(matrix_multiplication 
                ${CMAKE_CURRENT_SOURCE_DIR}/code/CudaMatrixMuti.cu)
message( ${CMAKE_CURRENT_SOURCE_DIR}/code/CudaMatrixMuti.cu)
# Link CUDA libraries
target_link_libraries(matrix_multiplication ${CUDA_LIBRARIES})