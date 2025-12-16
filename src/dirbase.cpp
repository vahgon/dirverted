#include <iostream>
#include <stdexcept>
#include <getopt.h>

#include "../include/dirbase.hpp"

void DirBase::Opts::getOpts(int argc, char** argv) {
  int c, digit_optind = 0;

  while(1) {
    int currOptsInd = optind ? optind : 1;
    int optIndex = 0;
    static struct option long_options[] = {
      { "input",     1,   0,   'i' },
      { "output",    2,   0,   'o' },
      { "recursive", 0,   0,   'r' },
      { 0,           0,   0,    0 }
    };
    
    c = getopt_long(argc, argv, "i:o::r", long_options, &optIndex);

    if (c == -1) break;
    switch(c) {
      case 'i':
        std::cout<<"Option I called with" << optarg << std::endl;
        break;
      case 'o':
        std::cout<<"Option O called with"<< optarg << std::endl;
        break;
      case 'r':
        std::cout<<"Option r called" << std::endl;
        break;
      case '?':
        throw  std::invalid_argument("");
        break;
    };
  }
}

void DirBase::isDir() const { 
  if(!std::filesystem::exists(dirPath)) throw std::invalid_argument("Directory does not exist!");
}

