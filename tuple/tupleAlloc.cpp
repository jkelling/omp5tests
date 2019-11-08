#include <iostream>
#include <tuple>
#include <cstdio>
#include <omp.h>

int main()
{
	//tuple<unsigned int *, unsigned int *, unsigned int *, unsigned long>
	int A[] = {10,2,3,4};
        int *tA = (int *) omp_target_alloc(4*sizeof(int), 0);
        int *tB = (int *) omp_target_alloc(4*sizeof(int), 0);
        int *tC = (int *) omp_target_alloc(4*sizeof(int), 0);

	omp_target_memcpy(tA, A, 4*sizeof(int), 0, 0, 0, omp_get_initial_device());

	auto argsD = std::make_tuple(tA, tB, tC, A[2]);
	printf("HOST: tA=%#llx, tB=%#llx, tC=%#llx, A=%d\n"
		, (unsigned long long int)(std::get<0>(argsD)),(unsigned long long int)(std::get<1>(argsD)),(unsigned long long int)(std::get<2>(argsD))
		, int(std::get<3>(argsD)));

#pragma omp target map(to:argsD)
	{
#pragma omp teams num_teams(1) thread_limit(4)
		{
			printf("DEVICE: tA=%#llx, tB=%#llx, tC=%#llx, A=%d, A[0]=%d\n"
				, (unsigned long long int)(std::get<0>(argsD)),(unsigned long long int)(std::get<1>(argsD)),(unsigned long long int)(std::get<2>(argsD))
				, int(std::get<3>(argsD))
				, std::get<0>(argsD)[0]
				);
                    int * td = (int*)(std::get<0>(argsD));
                    int * tdC = (int*)(std::get<2>(argsD));
		    if(omp_get_thread_num()==0)
			    *td *= 12;
		    for(int a = 0; a < 4; ++a)
			    tdC[a] = *td;
		}
	}

	int C[4];
	omp_target_memcpy(C, tC, 4*sizeof(int), 0, 0, omp_get_initial_device(), 0);
	printf("C[0]=%d\n", C[0]);
}
