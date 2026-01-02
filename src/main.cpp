#include "../include/dirvert.hpp"
#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  try {
    Dirvert *d = new Dirvert(argc, argv);

    d->formatOutPath();
    d->begin();

} catch (std::exception& e) { std::cout << std::string(e.what()) << std::endl; }

  return 0;
}
