#include <iostream>
#include <tuple>
#include <cstdio>
#include <tuple>

#include <omp.h>

constexpr float data[] = {1,2,3,};

float getX()
{
	return data[0];
}

struct Data
{
	inline const float& operator[] (int i) const {return data[i];}
};

template<typename T, int TDim>
struct TData
{
	static constexpr int dim = TDim;
	inline const T& operator[] (int i) const {return data[i];}
};

template<class Kernel, class Args>
void execT(Kernel kernel, Args targs)
{

	int numThreads = 2; //omp_get_max_threads();
	int numTeams = 2;

#pragma omp target
	{
#pragma omp teams num_teams(numTeams) thread_limit(numThreads)
		{
#pragma omp distribute
			for(int t = 0; t < omp_get_num_teams(); ++t)
			{
#pragma omp parallel num_threads(numThreads)
				{
					std::apply([kernel](const auto& ...args)
					{
						kernel(1, args...);
					}, targs);
				}
			}
		}
	}
}

template<class Kernel, class ...Args>
void exec(Kernel kernel, Args ...args)
{
	execT(kernel, std::tuple<std::decay_t<Args>...>(args...));
}

struct Kernel
{
	template<class Acc>
	void operator() (Acc acc, int i) const
	{
		// printf("access %f\n", data[i]); 
		// printf("getX %f\n", getX());

		// {
		// 	Data d;
		// 	printf("Data[] %f\n", d[i]);
		// }

		{
			TData<float, 3> d;
			printf("TData[] %f\n", d[i]);
		}
	}
};

int main(int argc, char * argv[])
{
	Kernel k;
	exec(k, argc);
}
