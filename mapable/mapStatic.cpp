#include <type_traits>

// #define TEST_NO_STATIC

class T {
public:
#ifndef TEST_NO_STATIC
  static constexpr int s = 1;
#endif
  T(int a) : a(a) {};

#ifndef TEST_TRIVIALCOPY
  T(const T& other) {}

#ifndef TEST_TRIVIALDESTRUCT
  ~T() {}
#endif
#endif

private:
  [[maybe_unused]] int a;
};

#ifdef TEST_TRIVIALCOPY
static_assert(std::is_trivially_copyable<T>::value, "T not trivially copyable");
#endif

int main(int argc, char *argv[]) {
  T t(2);
  #pragma omp target enter data map(to: t)

  return 0;
}
