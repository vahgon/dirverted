#ifndef DIRROOT_H
#define DIRROOT_H

#include <filesystem>
#include <memory>
#include "opts.hpp"

class DirRoot {
public:
  DirRoot(int argc, char** argv);

  void formatOutPath();
  void createOutDir() const;
  void checkInputDir() const;

private:
  std::unique_ptr<Opts> _opts;

protected:
  const std::filesystem::path& _inPath; 
  std::filesystem::path _outPath = "./";

  const int& _isVerbose;
  const int& _isRecursive;
  const int& _childrenOnly;
};

#endif
