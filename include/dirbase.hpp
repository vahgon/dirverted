#ifndef DIRBASE
#define DIRBASE

#include <filesystem>

class DirBase{
private:
  const std::filesystem::path dirPath;

public:
  DirBase() : dirPath("") {}
  
  struct Opts {
    bool recur, del;
    std::string input, output;
    
    Opts(int, char**);
  };

  void isDir() const;
  virtual ~DirBase();
};

#endif
