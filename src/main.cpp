#include <getopt.h>

#include <string>
#include <print>
#include <vector>

#include "archie/archie.hpp"

class ArgParser {
 private:
  std::vector<std::string> tokens_;

 public:
  ArgParser(int& argc, char** argv) {
    for (size_t i{ 0 }; i < argc; i++) {
      tokens_.push_back(argv[i]);
    }
  }

  const std::string& getOpt(const std::string& opt) const {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(tokens_.begin(), tokens_.end(), opt);
    if (itr != tokens_.end() && ++itr != tokens_.end())
      return *itr;
    static const std::string empty_string("");
    return empty_string;
  }

  bool optExists(const std::string& opt) const {
    return std::find(tokens_.begin(), tokens_.end(), opt) != tokens_.end();
  }
};

int main(int argc, char** argv) {
  ArgParser input(argc, argv);

  if (input.optExists("-h")) {
    std::print("This is the help");
    return -1;
  }

  if (input.optExists("-o")) {
    auto in_file = input.getOpt("-o").c_str();
    archie::archive file{ in_file, 1 };
  }

  return 0;
}
