#include "fem.hpp"

#include <iostream>

using namespace fem;

int main(int argc, char *argv[])
{
  Vec a(25, 7.0);
  a.fill(rand);
  std::cout << a.dump() << '\n';
  return 0;
}
