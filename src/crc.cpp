#include "dvrt/crc.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <cstdint>

namespace crc = dvrt::crc;

uint32_t crc::crc32_intrinsic(char* bytes, size_t& file_size) {
  return 0;
}

uint32_t crc::crc32_lookup(char* bytes, size_t& file_size) {
  uint32_t crc{ __internal::init_crc };
  std::size_t curr_size{ file_size };

  while (curr_size--)
    crc = (crc >> 8) ^ __internal::crctable[(crc & 0xff) ^ *bytes++];
  return crc ^ __internal::init_crc;
}
