#include <iostream>
#include <tuple>
#include <cstdio>

int main()
{
	//tuple<unsigned int *, unsigned int *, unsigned int *, unsigned long>
	int A=1, B=2, C=3;
	auto argsD = std::make_tuple(&A, &B, &C, A);

	printf("HOST: &A=%#llx, &B=%#llx, &C=%#llx, A=%d\n"
		, (unsigned long long int)(std::get<0>(argsD)),(unsigned long long int)(std::get<1>(argsD)),(unsigned long long int)(std::get<2>(argsD))
		, int(std::get<3>(argsD)));

#pragma omp target map(from:argsD)
	{
#pragma omp teams num_teams(1) thread_limit(4)
		{
			printf("DEVICE: &A=%#llx, &B=%#llx, &C=%#llx, A=%d\n"
				, (unsigned long long int)(std::get<0>(argsD)),(unsigned long long int)(std::get<1>(argsD)),(unsigned long long int)(std::get<2>(argsD))
				, int(std::get<3>(argsD)));
		}
	}
}
