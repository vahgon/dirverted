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
  int _onlyChildren;
  int _followSymbolic;


  inline static const char* tokens[] = {
    "children",
    "symbolic",
    nullptr
  };
  enum {
    rec_onlychildren_opt = 0,
    rec_followsymbolic_opt,
  };

  inline static int _verbose = 0;
  inline static int _quiet = 0;

  inline static struct option long_options[] = {
    { "input",          required_argument,   0,          'i' },
    { "output",         required_argument,   0,          'o' },
    { "recursive",      no_argument,         0,          'r' },
    { "verbose",        no_argument,    &_verbose,        1  },
    { "quiet",          no_argument,    &_quiet,          1  },
    { 0,                no_argument,         0,           0  }
    };
};

#endif
