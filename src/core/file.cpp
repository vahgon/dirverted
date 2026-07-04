#include "archie/file.hpp"

std::ostream& operator<<(std::ostream& os, archie::file const& file) {
  return os << file.path().string();
}

std::size_t archie::file::save() const {
  // return sizeof modified buffer - original sizeof file
}

std::span<std::byte const> archie::file::gen_header(std::size_t const) const {
}

std::size_t open() {
}
