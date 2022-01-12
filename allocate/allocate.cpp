#include <iostream>
#include <cstdio>

#include <omp.h>

int main()
{
	int maxThreads = omp_get_max_threads();
	constexpr int numTeams = 4;

	int aLocal[numTeams];
	int aTeam[numTeams];

	std::cout << "maxThreads=" << maxThreads
		<< "\tnumTeams=" << numTeams << std::endl;

#pragma omp target data map(tofrom:aLocal,aTeam)
#pragma omp target
	{
#pragma omp teams num_teams(numTeams) thread_limit(maxThreads)
		{
#pragma omp distribute
			for(int t = 0; t < omp_get_num_teams(); ++t)
			{
				int team = 0;
#pragma omp parallel //num_threads(maxThreads)
				{
					int local = 0;
#pragma omp allocate(local) allocator(omp_pteam_mem_alloc)
#pragma omp atomic update
					local += t+1;
#pragma omp atomic update
					team += t+1;
#pragma omp barrier
					// printf("TEAM %d, THREAD=%d, local=%d, team=%d\n"
					// 	, (int)(omp_get_team_num()), (int)(omp_get_thread_num()), local, team);
					if(omp_get_thread_num() == 0)
						aLocal[t] = local;
				}
				aTeam[t] = team;
			}
		}
	}

	bool good = true;
	for(int a = 0; a < numTeams; ++a)
	{
		if(aLocal[a] != aTeam[a])
		{
			std::cerr << "allocator(omp_pteam_mem_alloc) not shared in team for team " << a << ": "
				<< aLocal[a] << " != " << aTeam[a] << '\n';
			good = false;
		}
	}

	return !good;
}
