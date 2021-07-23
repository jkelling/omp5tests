#!/bin/bash

ARCH="${1:gfx900}"

cmake .. \
	-DCMAKE_CXX_FLAGS="-Wno-openmp-mapping -Wno-reserved-identifier -Wno-extra-semi-stmt -Wno-comma -fopenmp -fopenmp=libomp -fopenmp-targets=amdgcn-amd-amdhsa -Xopenmp-target=amdgcn-amd-amdhsa -march=$ARCH" \
	-DCMAKE_EXE_LINKER_FLAGS="-fopenmp"
