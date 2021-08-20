#include <iostream>

typedef double Type;

constexpr Type s = 23;

class T {
public:
	static constexpr Type s = 42;
	static constexpr int N = 5;

	Type m_s[N];

	constexpr T() : m_s() {
		for(int a = 0; a < N; ++a)
			m_s[a] = 5 - a;
	}

	Type getS (int a = N - 1) const {return m_s[a];}
	void setS (Type p) {
		for(int a = 0; a < N; ++a)
			m_s[a] = s + p - a;
	}
};

// #pragma omp declare target
constexpr T t;
// #pragma omp end declare target

int main(int argc, char *argv[]) {

	Type global = 0, member = 0, cemem = 0;

#pragma omp target map(tofrom: global, member, cemem)
	{
#pragma omp teams distribute
		for(int i = 0; i < 3; ++i)
		{
			global = s*3.14;
			member = T::s*3.14;

			T t;
			t.setS(s);
			member = t.getS()*2.718;

			cemem = ::t.getS(i);
		}
	}

	std::cout << global << " - " << member << " - " << cemem << std::endl;

	return 0;
}
