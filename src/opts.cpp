#include <stdexcept>
#include "../include/opts.hpp"

void isSpacedArg(char **argv, int &ind) { if (optarg[0] == '-') throw std::invalid_argument(std::string(argv[0]) + ": directory must be provided with " + std::string(argv[ind])); }
void throwMissing(char **argv, int &ind) { throw std::invalid_argument(std::string(argv[0]) + ": invalid arg " + std::string(optarg) + " in " + std::string(argv[ind])); }

void Opts::parseOpts(const int argc, char** argv) {
  int c, digitOptind = 0;
  while(true) {
    int currOptsInd = optind ? optind : 0;
    int optIndex = 0;

    c = getopt_long(argc, argv, "i:o:c::r", long_options, &optIndex);
    if (c == -1) break;

    switch(c) { 
      case 'i': isSpacedArg(argv, currOptsInd); 
        Opts::dirExists( argv[0]);
        Opts::_indir = optarg;
        break;
      case 'o': isSpacedArg(argv, currOptsInd);
        Opts::_odir = optarg;
        break;

      case 'c': _copy = true;
        if (optarg && *optarg != 'd') throwMissing(argv, currOptsInd);
        else _delete = true;
        break;
      case 'r': _recursive = true;
        break;
    }
  } 
}

void Opts::dirExists(char *argv) const {
  if (!std::filesystem::is_directory(optarg)) throw std::invalid_argument(std::string(argv) + ": " + std::string(optarg) + " is not a valid directory"); 
}
