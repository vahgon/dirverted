#include <string>

#include "archie/archie.hpp"

int main(int argc, const char* argv[]) {
  if (!(argc > 1)) {
    return 1;
  }
  const std::string path{ argv[1] };
  archie::archive user_input{ std::string_view{ path } };

  return 0;
}
