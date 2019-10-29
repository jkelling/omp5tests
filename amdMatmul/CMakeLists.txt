add_executable(matmul matmul.c)
target_link_libraries(matmul omp omptarget)
