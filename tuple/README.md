Compile target `tuple`

# compiler results
|compiler|CMAKE_CXX_FLAGS|compile status|target|run status|
|---|---|---|---|---|
|LLVM 9.0.0 (CLAIX)|-fopenmp -fopenmp=libomp -fopenmp-targets=x86_64-pc-linux-gnu|omp warning about mapping tuple|x86|data not/wrongly mapped|
|GCC 9.2 (CLAIX)|-fopenmp|lto error (1)|no GPU|--|
|GCC 9.2 (CLAIX)|-fopenmp -fno-lto|ok|no GPU|ok |
|GCC 9.2 (CLAIX)|-fopenmp -fno-lto -foffload=nvptx-none|ok|nvptx|ok,not using GPU|

## errors

1. lto1: fatal error: LTO_tags out of range: Range is 0 to 420, value is
   939524096
   compilation terminated.
   mkoffload: fatal error:
   /usr/local_rwth/sw/gcc/9.2.0/bin/x86_64-pc-linux-gnu-accel-nvptx-none-gcc
   returned 1 exit status
   compilation terminated.
   lto-wrapper: fatal error:
   /usr/local_rwth/sw/gcc/9.2.0/libexec/gcc/x86_64-pc-linux-gnu/9.2.0//accel/nvptx-none/mkoffload
   returned 1 exit status
