#include <cstdlib>
#include <cstdio>

void f(int a ) {
#pragma omp target
    {
        int * aptr = (int *) malloc(a*sizeof(int));
        for (int i = 0; i < a; i ++) {
            aptr[i] = 0;
        }
    }
}

int main(int argc, char ** argv) {

    int val = (argc > 1) ? atoi(argv[1]) : 5;
    f(val);
    return 0;
}
