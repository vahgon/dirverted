#include "dvrt/fbuff.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

using file = dvrt::__buff::fbuff;

file::fbuff(const std::filesystem::path p)
  : fpath_{ p }
  , fsize_{ std::filesystem::file_size(p) }
  , bytes_{ std::make_shared<char[]>(fsize_) } { read_bytes(); }

const std::shared_ptr<char[]>& file::get_bytes() const { return bytes_; }

void file::read_bytes() {
  std::ifstream file{ fpath_, std::ios::binary };
  file.read(bytes_.get(), fsize_);
  file.close();
}

uint32_t file::calc_checksum() const {
  return 0;
}
