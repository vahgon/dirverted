#include "../include/dirroot.hpp"
#include <iostream>

void DirRoot::formatOutPath(const int argc, char **argv) {
    
}


void DirRoot::spanRootDir() noexcept {
  for (const auto dir_entry : *_dirIterator) {
  std::cout << dir_entry << std::endl;
  }
}
