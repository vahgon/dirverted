#include "../include/dirvert.hpp"
#include <iostream>

int main(int argc, char *argv[]) {

  Dirvert* tester = new Dirvert; 
  try { tester->setOpts(argc, argv); }
  catch(std::invalid_argument err) { std::cerr << err.what(); }
        

  return 0;
}
