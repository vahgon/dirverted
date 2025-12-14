#ifndef DIRBASE
#define DIRBASE

#include <filesystem>

class DirBase{
private:
  const std::filesystem::path dirPath;

public:
  DirBase(std::filesystem::path p) : dirPath(p) {}
  bool isDir() const;
};

#endif
