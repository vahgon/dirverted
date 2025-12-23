#ifndef DIRVERT
#define DIRVERT

#include "dirroot.hpp"
#include <filesystem>

class Dirvert : public DirRoot {
public:
  Dirvert();
 ~Dirvert() { delete _dirNode; } 

  const bool isDir(std::filesystem::path& dir) const noexcept { return std::filesystem::is_directory(dir); }  

private:
  std::filesystem::path *_dirNode;
};

#endif
