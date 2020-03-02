#include <iostream>

#include <omp.h>

int main()
{
	std::cout << "num_devices: " << ::omp_get_num_devices() << std::endl;
	return 0;
}
