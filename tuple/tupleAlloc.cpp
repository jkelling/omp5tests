#include <iostream>
#include <tuple>
#include <cstdio>
#include <omp.h>

int main()
{
	//tuple<unsigned int *, unsigned int *, unsigned int *, unsigned long>
	int A=1;
        int *tA = (int *) omp_target_alloc(4, 0);
        int *tB = (int *) omp_target_alloc(4, 0);
        int *tC = (int *) omp_target_alloc(4, 0);
	auto argsD = std::make_tuple(tA, tB, tC, A);
	printf("HOST: tA=%#llx, tB=%#llx, tC=%#llx, A=%d\n"
		, (unsigned long long int)(std::get<0>(argsD)),(unsigned long long int)(std::get<1>(argsD)),(unsigned long long int)(std::get<2>(argsD))
		, int(std::get<3>(argsD)));
        int thD = -1;

#pragma omp target map(from:thD)
	{
#pragma omp teams num_teams(1) thread_limit(4)
		{
			printf("DEVICE: tA=%#llx, tB=%#llx, tC=%#llx, A=%d\n"
				, (unsigned long long int)(std::get<0>(argsD)),(unsigned long long int)(std::get<1>(argsD)),(unsigned long long int)(std::get<2>(argsD))
				, int(std::get<3>(argsD)));
                    int * td = (int*)(std::get<0>(argsD));
                    *td = 12;
                    thD = *td;
		}
	}
       printf("thD=%d\n", thD);
}
