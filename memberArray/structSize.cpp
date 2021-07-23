#include <iostream>

template<int N>
struct A : public A<N-1>
{
	int arr[8];
	// int b = 0;

	A()
	{
		arr[0] = 0;
	}
};

template<>
struct A<1>
{
	int arr[8];
	// int b = 0;

	A()
	{
		arr[0] = 0;
	}
};

template<int N>
void f()
{
	int b = 0;

#pragma omp target map(tofrom: b)
	{
#pragma omp teams distribute
		for(int i = 0; i < 1; ++i)
		{
			A<N> a;
			b = a.arr[0] += sizeof(a);
			// b = a.b += sizeof(a);
		}
	}

	std::cout << "A: " << b << std::endl;
}

template<int N>
struct B : public B<N-1>
{
	long long b = 0;
	long long c = 0;
	long long d = 0;
};

template<>
struct B<1>
{
	long long b = 0;
	long long c = 0;
	long long d = 0;
};

template<int N>
void fB()
{
	int b = 0;

#pragma omp target map(tofrom: b)
	{
#pragma omp teams distribute
		for(int i = 0; i < 1; ++i)
		{
			B<N> a;
			b = a.b += sizeof(a);
		}
	}

	std::cout << "B: " << b << std::endl;
}

int main()
{
	//OK:
	f<2>();
	fB<1024>();

	// //[GPU Memory Error] Addr: 0x0 Reason: Page not present or supervisor privilege.:
	// // https://bugs.llvm.org/show_bug.cgi?id=50968
	// f<65>();
}
