#include "../include/dirroot.hpp"
#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  try {
  DirRoot *d = new DirRoot(argc, argv);

  d->formatOutPath();
  d->spanRootDir();
} catch (std::exception e) { std::cout << e.what() << std::endl; } 

  return 0;
}
