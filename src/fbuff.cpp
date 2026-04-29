#include "dvrt/fbuff.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "dvrt/crctable.hpp"

using file = dvrt::__buff::fbuff;

file::fbuff(const std::filesystem::path p)
  : fpath_{ p }
  , fsize_{ std::filesystem::file_size(p) }
  , bytes_{ std::make_shared<std::byte[]>(fsize_) } { read_bytes(); }

void file::read_bytes() {
  std::ifstream file{ fpath_, std::ios::binary };
  file.read(reinterpret_cast<char*>(bytes_.get()), fsize_);
  file.close();
}

uint32_t file::crc32_intrinsic() noexcept {
  return 0;
}

uint32_t file::crc32_lookup_table() noexcept {
  uint32_t crc{ dvrt::__crc::init_crc };
  size_t curr_size{ fsize_ };
  std::byte* curr_byte{ bytes_.get() };

  while (curr_size--)
    crc = (crc >> 8) ^ __crc::crctable[(crc & 0xff) ^ std::to_integer<uint8_t>(*curr_byte++)];
  return crc ^ __crc::init_crc;
}
