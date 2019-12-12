/* Test target region with instatiation of struct with mutable member. */

#include <iostream>
#include <tuple>
#include <cstdio>

struct WithMutable
{
	int a;
	mutable int m;

		WithMutable(int a) : a(a) {}
};

int main()
{
	int a=1;

#pragma omp target
	{
#pragma omp teams num_teams(1) thread_limit(4)
		{
			WithMutable A(a);
			printf("DEVICE: A.a=%d\n"
				, int(A.a));
		}
	}
}
