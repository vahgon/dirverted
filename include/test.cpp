#include "dirbase.hpp"
#include <iostream>

bool DirBase::isDir() const {
  if(std::filesystem::exists(dirPath)) {
    return true;
  }
  return false;
}

int main() {
  DirBase yes(".");

  if (yes.isDir()) {
    std::cout<<"hi";
  }
  return 0;
}
