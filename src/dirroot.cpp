#include "../include/dirroot.hpp"
#include <iostream>
#include <stdexcept>

DirRoot::DirRoot(int argc, char** argv) : 
  _opts(std::make_unique<Opts>(argc, argv)), 
  _inPath(_opts->_indir), 
  _outPath(_opts->_odir),
  _isRecursive(_opts->_recursive),
  _childrenOnly(_opts->_onlyChildren),
  _isVerbose(_opts->_verbose)
{}

void DirRoot::checkInputDir() const { if (_opts->_indir == "") throw std::invalid_argument(std::string("Missing input arg")); }

// If output is not specified, no mkdir. The input directory will be directly modified 
void DirRoot::formatOutPath() {
  checkInputDir();

  if (!_opts->_odir.empty()) 
  {}
  else if (_opts->_odir.empty() && _opts->_copy)
    _outPath = "copy-" + std::string(_opts->_indir);
  else
    _outPath = _opts->_indir;
  
  DirRoot::createOutDir();
}

void DirRoot::createOutDir() const { 
  std::cout<<"_inPath: " << _inPath<<"\n";
  std::cout<<"_outPath: "<<_outPath<<"\n";
  // std::filesystem::create_directories(_outPath.relative_path()); 
}
