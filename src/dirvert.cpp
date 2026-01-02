#include "../include/dirvert.hpp"
#include <filesystem>
#include <iostream>

Dirvert::Dirvert(int argc, char** argv) : 
  DirRoot(argc, argv)
{}

void Dirvert::begin() {
  spanRootDir();

  if (!DirRoot::_isVerbose) 
    printSteps();
  else 
    verbosePrintTree();

  std::cout << "Press enter to continue... " << std::cin.get();
}

void Dirvert::spanRootDir() {
  if (!DirRoot::_childrenOnly) {
    dir_instance fndDir;
    fndDir.parentDir = DirRoot::_inPath.parent_path();
    fndDir.dir = DirRoot::_inPath;
    fndDir.depth = 0;

    _dirvertDirectories.emplace_back(); 
  }

  if (DirRoot::_isRecursive) {
    std::filesystem::recursive_directory_iterator recIterator(DirRoot::_inPath);
    for(const auto& dirEntry : recIterator) {
      if (dirEntry.is_directory()) {
        dir_instance fndDir;
        fndDir.parentDir = dirEntry.path().parent_path();
        fndDir.dir = dirEntry.path();
        fndDir.depth = (recIterator.depth()) + 1;

        _dirvertDirectories.emplace_back(fndDir);
      }
    }
  }
}

void Dirvert::printSteps() const {
  std::cout << "\nFormatting the following directories -\n";

  for (const auto& dir : _dirvertDirectories)
    std::cout << dir.dir << " at " << dir.depth << std::endl;
}

void Dirvert::verbosePrintTree() const {
  for (const auto& dir : _dirvertDirectories){
    std::cout << dir.depth << std::endl;
    std::cout << dir.dir << std::endl;
    std::cout << dir.parentDir << std::endl;
    std::cout << " ---- \n";
  }
}
