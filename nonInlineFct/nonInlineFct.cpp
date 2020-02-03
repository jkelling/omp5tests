#include <iostream>
#include <cstdio>

#include "fct.h"

constexpr int TEAMS = 1;
constexpr int THREADS = 4;

int main()
{
#pragma omp target
	{
#pragma omp teams num_teams(TEAMS) thread_limit(THREADS)
		{
			#pragma omp distribute
			for(int t = 0; t < TEAMS; ++t)
			{
				#pragma omp parallel
				{
					printf("%d\n", fct(t));
				}
			}
		}
	}
}
