#include <iostream>
#include <cstdio>

#include <omp.h>

bool testAssoc(int idev)
{
	constexpr int N = 4;
	constexpr int VAL = 42;
	constexpr int INIT = 1;
	constexpr int ADD = VAL-INIT;

	int host[N];
	for(auto& a: host) a = INIT;

	int* dev = (int*)omp_target_alloc(N*sizeof(int), idev);
	omp_target_associate_ptr(host, dev, N*sizeof(int), 0, idev);
#pragma omp target update to(host) device(idev)

	int** pdev = &dev; // implicit mapping of dev yields nullptr on target (at least clang)
#pragma omp target teams distribute parallel for device(idev) map(to:pdev[0:1])
	for(int a = 0; a < N; ++a)
	{
		(*pdev)[a] += ADD;
	}

#pragma omp target update from(host) device(idev)

	omp_target_disassociate_ptr(host, idev);
	omp_target_free(dev, idev);

	for(int a = 0; a < N; ++a)
		if(host[a] != VAL)
		{
			std::cout << "Fail: update from associated ptr failed from device " << idev << " val=" << host[a] << std::endl;
			return false;
		}

	std::cout << "Success: update from associated ptr from device " << idev << std::endl;
	return true;
}

int main()
{
	bool good = true;
	const int numDev = omp_get_num_devices();
	for(int d = 0; d < numDev; ++d)
		good &= testAssoc(d);
	good &= testAssoc(omp_get_initial_device());
	return good ? 0 : 1;
}
