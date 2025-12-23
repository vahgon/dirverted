#ifndef DIRROOT
#define DIRROOT
#include <filesystem>
#include <memory>
#include "./opts.hpp"

class DirRoot {
public:
  DirRoot(int argc, char **argv) : 
    _opts(std::make_unique<Opts>(argc, argv)),
    _rootDir(std::make_unique<std::filesystem::path>(_opts->_indir)),
    _dirIterator(std::make_unique<std::filesystem::recursive_directory_iterator>(*_rootDir))
  { }
   
  void formatOutPath();
  void createOutDir() const;
  void spanRootDir() noexcept;

protected:
  const std::filesystem::path _inPath; 
  std::filesystem::path _outPath = "./";
  std::filesystem::path **_dirs; 

private:
  std::unique_ptr<Opts> _opts;
  const std::unique_ptr<std::filesystem::path> _rootDir;
  std::unique_ptr<std::filesystem::recursive_directory_iterator> _dirIterator;
};

#endif
