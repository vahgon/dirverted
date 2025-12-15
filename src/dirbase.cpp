#include "../include/dirbase.hpp"
#include <filesystem>
#include <stdexcept>

DirBase::Opts::Opts(int argc, char** argv) {
  recur = false;
  del = false;
  input = "";
  output = "";
}

void DirBase::isDir() const { 
  if(!std::filesystem::exists(dirPath)) throw std::invalid_argument("Directory does not exist!");
}

