#ifndef DIRBASE
#define DIRBASE

#include <filesystem>
#include <iostream>

class DirBase{
private:
  const std::filesystem::path dirPath;

public:
  DirBase() : dirPath(std::filesystem::current_path()) {}
  
  struct Opts {
    bool recur, del;
    std::string input, output;
    
    static void getOpts(int, char**);
    bool checkArgs() const;
  };

  void isDir() const;
  virtual ~DirBase() { std::cout << "~DirBase()"; }
};

#endif
