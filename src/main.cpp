#include "arta.hpp"

int main(int argc, char *argv[])
{
  arta::log::version("ARTA V.%d.%d", ARTA_VERSION_MAJOR, ARTA_VERSION_MINOR);
  return 0;
}
