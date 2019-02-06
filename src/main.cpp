#include "fem.hpp"

#include <iostream>

using namespace fem;

int main(int argc, char *argv[])
{
  Mat b(10, 1);
  b.set(3, 7, 3.1);
  std::cout << b.count() << '\n';
  std::cout << b.dump() << '\n';
  b.set(3,7,0);
  std::cout << b.count() << '\n';
  std::cout << b.dump() << '\n';
  return 0;
}
