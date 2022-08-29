#include <iostream>
#include <cstdio>

#include <omp.h>

int main()
{
	int maxThreads = omp_get_max_threads();
	constexpr int numTeams = 4;

#pragma omp target
	{
#pragma omp teams num_teams(numTeams) //thread_limit(maxThreads)
		{
#pragma omp distribute
			for(int t = 0; t < omp_get_num_teams(); ++t)
			{
				int team = 0;
#pragma omp parallel
				{
#pragma omp atomic update
					team += t+1;
#pragma omp barrier // nvptx sm_35 backend: "invalid instruction"
					team += t+1;
				}
			}
		}
	}

	return 0;
}
