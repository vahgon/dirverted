#ifndef DIRVERT_H
#define DIRVERT_H

#include "dirroot.hpp"
#include <filesystem>
#include <vector>
    
struct dir_instance{
  std::filesystem::path parentDir;
  std::filesystem::path dir;
  int depth;
};

class Dirvert : public DirRoot {
public:
  Dirvert(int, char**);

  void begin();
  void spanRootDir();
  void announce() const;

  void printSteps() const;
  void verbosePrintTree() const;

private:
  std::vector<dir_instance> _dirvertDirectories;
};

#endif
