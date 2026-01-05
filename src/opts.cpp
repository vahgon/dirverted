#include "../include/opts.hpp"
#include <cstring>

void isSpacedArg(char** argv, int &ind) { 
  if (optarg[0] == '-') 
    throw std::invalid_argument(std::string(argv[0]) + ": directory must be provided with " + std::string(argv[ind])); 
}

void throwMissing(char** argv, int &ind) { 
  throw std::invalid_argument(std::string(argv[0]) + ": invalid arg " + std::string(optarg) + " in " + std::string(argv[ind])); 
}

Opts::Opts(int argc, char** argv) : 
    _recursive(0),
    _onlyChildren(0)
{ parseOpts(argc, argv); }

void Opts::parseOpts(const int argc, char** argv) {
  while(true) {
    int currOptsInd = optind ? optind : 0;
    int optIndex = 0;
    int opt = getopt_long(argc, argv, "i:o:r::vs", long_options, &optIndex);

    if (opt == -1) break;
    switch(opt) {
      case 'i': isSpacedArg(argv, currOptsInd); 
        dirExists(argv[0]);
        _indir = optarg;
      break;

      case 'o': isSpacedArg(argv, currOptsInd);
        _odir = optarg;
      break;

      case 'r': _recursive = true;
        if (optarg)
          parseSubOpts();
      break;

      case '?':
        throw std::invalid_argument(optarg);
      break;

      case ':':
        throw std::invalid_argument(optarg);
      break;
    }
  }
}
 
void Opts::dirExists(const char* argv) const {
  if (!std::filesystem::exists(optarg)) 
    throw std::invalid_argument(std::string(argv) + ": " + std::string(optarg) + " is not a valid directory"); 
}

void Opts::parseSubOpts() {
  char* subopts = optarg;
  char* val;
  int err = 0;

  if (subopts[0] == '=' && subopts[1] != '\0')
    std::memmove(subopts, subopts + 1, strlen(subopts));

  while(*subopts != '\0' && !err) {
    switch(getsubopt(&subopts, const_cast<char* const*>(tokens), &val)) {
      case rec_onlychildren_opt:
        _onlyChildren = 1;  
      break;

      case rec_followsymbolic_opt:
        _followSymbolic = 1;
      break;

      default:
        throw std::invalid_argument(optarg);
      break;
    }
  }
}
