#include <iostream>
#include <cstdio>

#include <omp.h>

int main()
{
	int maxThreads = omp_get_max_threads();
	constexpr int numTeams = 4;
	int numThreads = maxThreads/numTeams;

	int aTeam[numTeams];

	std::cout << "maxThreads=" << maxThreads
		<< "\tnumTeams=" << numTeams << std::endl;

	int N = 5;
	std::cin >> N;

#pragma omp target data map(from:aTeam)
#pragma omp target
	{
#pragma omp teams num_teams(numTeams)
		{
#pragma omp distribute
			for(int t = 0; t < omp_get_num_teams(); ++t)
			{
				int* team = reinterpret_cast<int*>(omp_alloc(N*sizeof(int), omp_pteam_mem_alloc));
				for(int a = 0; a < N; ++a)
					team[a] = 0;
#pragma omp parallel num_threads(numThreads)
				{
					for(int a = 0; a < N; ++a)
#pragma omp atomic update
						team[a] += t+1;
#pragma omp barrier
					if(omp_get_thread_num() == 0)
					{
						int sum = 0;
						for(int a = 0; a < N; ++a)
							sum += team[a];
						aTeam[t] = sum;
					}
				}
				omp_free(team, omp_pteam_mem_alloc);
			}
		}
	}

	bool good = true;
	for(int a = 0; a < numTeams; ++a)
	{
		const auto expected = N*(a+1)*numThreads;
		if(expected != aTeam[a])
		{
			std::cerr << "team array error in team " << a << ": "
				<< expected << " != " << aTeam[a] << '\n';
			good = false;
		}
	}

	return !good;
}
