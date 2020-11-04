constexpr double operator"" _X (long double a)
{
	return (double)a;
}

int main()
{
	auto a = 1._X;

#pragma omp target
	{
#pragma omp teams num_teams(1) thread_limit(4)
		{
			a += 1._X;
		}
	}
}
