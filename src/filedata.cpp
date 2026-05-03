#include "archie/filedata.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "archie/converter.hpp"
#include "archie/headers.hpp"

using path = std::filesystem::path;
using byte = std::byte;

archie::file::file(const path& p)
: p_{ p }
, file_size_{ std::filesystem::file_size(p) }
, file_name_{ std::make_shared<byte[]>(p.string().size()) }
, buffer_{ std::make_shared<byte[]>(file_size_) } {}

void archie::file::get_byte_reps() {
  read_bytes();
  convert::path_bytes(p_, file_name_);
}

void archie::file::read_bytes() {
  std::ifstream file{ p_, std::ios::binary };
  std::weak_ptr<byte[]> ptr{ buffer_ };

  if (ptr.lock())
    file.read(reinterpret_cast<char*>(buffer_.get()), file_size_);
  else
    throw std::runtime_error(".lock() failed in archie::file::crc32_lookup()");
}
