#ifndef GET_OPTS
#define GET_OPTS

#include <getopt.h>
#include <filesystem>

struct Opts {
Opts(int argc, char **argv) : 
    _indir(""), 
    _odir("."),
    _recursive(false),
    _copy(false),
    _delete(false)
  { this->parseOpts(argc, argv); }

void parseOpts(const int, char**);
void dirExists(char*) const;

std::filesystem::path _indir;
std::filesystem::path _odir;

bool _recursive;
bool _copy;
bool _delete;

static inline struct option long_options[] = {
  { "input",      required_argument,   0,   'i' },
  { "output",     required_argument,   0,   'o' },
  { "copy",       optional_argument,   0,   'c' },
  { "recursive",  no_argument,         0,   'r' },
  { 0,            no_argument,         0,    0  }
  };
};

#endif
