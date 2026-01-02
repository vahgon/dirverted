#ifndef GET_OPTS_H
#define GET_OPTS_H

#include <getopt.h>
#include <filesystem>

struct Opts {
  Opts(int, char**);
  void parseOpts(const int, char**);
  void dirExists(const char*) const;
  void parseSubOpts();
  
  std::filesystem::path _indir;
  std::filesystem::path _odir;
  
  int _recursive;
  int _copy;
  int _onlyChildren;
  int _noRoot;
  
  enum {
    NO_ROOT_OPT = 0,
    COPY_OPT = 0
  };


  static inline int _verbose = 0;
  static inline struct option long_options[] = {
    { "input",          required_argument,   0,          'i' },
    { "output",         required_argument,   0,          'o' },
    { "copy",           optional_argument,   0,          'c' },
    { "recursive",      no_argument,         0,          'r' },
    { "verbose",        no_argument,    &_verbose,        1  },
    { 0,                no_argument,         0,           0  }
    };
};

#endif
