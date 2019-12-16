#include <iostream>
#include <tuple>
#include <cstdio>

#include <omp.h>

int main()
{
	int maxThreads = omp_get_max_threads();
	int numTeams = 4;

	std::cout << "maxThreads=" << maxThreads
		<< "\tnumTeams=" << numTeams << std::endl;
#pragma omp target
	{
#pragma omp teams num_teams(numTeams) thread_limit(maxThreads)
		{
#pragma omp distribute
			for(int t = 0; t < omp_get_num_teams(); ++t)
			{
				printf("TEAM %d, t=%d\n", (int)(omp_get_team_num()), t);
#ifndef __ibmxl_vrm__
				// setting num_threads to any value leads XL to run only one thread per team
				omp_set_num_threads(maxThreads);
#endif
#pragma omp parallel //num_threads(maxThreads)
				{
					printf("TEAM %d, THREAD=%d, maxThreads=%d\n"
						, (int)(omp_get_team_num()), (int)(omp_get_thread_num()), maxThreads);
				}
			}
		}
	}
}
