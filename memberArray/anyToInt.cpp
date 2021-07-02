#include <iostream>

template<class L, class R>
void f()
{
	L b = 0;

#pragma omp target map(tofrom: b)
	{
#pragma omp teams distribute
		for(int i = 0; i < 1; ++i)
		{
			R a = 0;
#pragma omp parallel num_threads(64)
			{
#pragma omp atomic update
				a += 1;
			}
			b = static_cast<L>(a);
		}
	}

	std::cout << b << std::endl;
}

int main()
{
	// OK:
	f<int, int>();
	f<char, int>();
	f<short, int>();
	f<long long, int>();
	f<char, unsigned int>();
	f<float, unsigned int>();
	f<double, unsigned int>();
	f<int, float>();
	f<int, double>();
	f<int, long long>();

	//ICE: https://bugs.llvm.org/show_bug.cgi?id=50967
	f<int, char>();
	f<int, short>();
	f<short, char>();
	f<char, short>();
	f<char, char>();
}

