#include "archie/file/crc.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

namespace crc = archie::crc;

using crc::__init::crctable;
using crc::__init::init_crc;

[[noreturn]] uint32_t crc::crc32_intrinsic(const std::byte* buff, size_t size);

uint32_t crc::crc32_lookup(const std::byte* buff, size_t size) {
  uint32_t crc{ init_crc };

  for (size_t i{ 0 }; i < size; i++)
    crc = (crc >> 8) ^ crctable[(crc & 0xff) ^ std::to_integer<uint32_t>(buff[i])];
  return crc ^ init_crc;
}
