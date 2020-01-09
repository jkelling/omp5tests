#include <type_traits>

class T {
public:
  T(int a) : a(a) {};

private:
  int a;
};

static_assert(std::is_trivially_copyable<T>::value, "T not trivially copyable");

int main(int argc, char *argv[]) {
  T t(2);
  #pragma omp target enter data map(to: t)

  return 0;
}
