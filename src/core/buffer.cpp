#include "core/buffer.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace io = archie::io;

void* io::buffer::open(std::filesystem::path const& path) {
  if (!m_file.open(path, std::ios::binary | std::ios::in)) {
    return nullptr;
  }
}

std::size_t io::buffer::save() {
  return 0z;
}
