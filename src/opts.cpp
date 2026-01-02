#include "../include/opts.hpp"
#include <iostream>
#include <stdexcept>

void isSpacedArg(char **argv, int &ind) { 
  if (optarg[0] == '-') 
    throw std::invalid_argument(std::string(argv[0]) + ": directory must be provided with " + std::string(argv[ind])); 
}

void throwMissing(char **argv, int &ind) { 
  throw std::invalid_argument(std::string(argv[0]) + ": invalid arg " + std::string(optarg) + " in " + std::string(argv[ind])); 
}

Opts::Opts(int argc, char **argv) : 
    _recursive(0),
    _copy(0),
    _onlyChildren(0),
    _noRoot(0)
{ this->parseOpts(argc, argv); }

void Opts::parseOpts(const int argc, char **argv) {
  int c, digitOptind = 0;
  while(true) {
    int currOptsInd = optind ? optind : 0;
    int optIndex = 0;

    c = getopt_long(argc, argv, "i:o:r::v", long_options, &optIndex);
    if (c == -1) break;

    switch(c) {
      case 'i': isSpacedArg(argv, currOptsInd); 
        Opts::dirExists(argv[0]);
        Opts::_indir = optarg;
        break;

      case 'o': isSpacedArg(argv, currOptsInd);
        Opts::_odir = optarg;
        break;

      case 'c': _copy = true;
        if (optarg && std::tolower(*optarg) != 'd') throwMissing(argv, currOptsInd);
        break; 

      case 'r': _recursive = true;
        //Three possible options
        parseSubOpts();
        throw;
        break;

      case '?':
        throw std::invalid_argument(optarg);
        break;

      case ':':
        std::cout<<"bye\n";
        break;
    }
  }
}
 
void Opts::dirExists(const char *argv) const {
  if (!std::filesystem::exists(optarg)) 
    throw std::invalid_argument(std::string(argv) + ": " + std::string(optarg) + " is not a valid directory"); 
}

void Opts::parseSubOpts() {
  char* const toke[2] = { "yes", "no" };
  std::cout << toke[0] << token[1];
}
