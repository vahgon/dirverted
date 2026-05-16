#include "archie/file/crc.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

#include "archie/file/file.hpp"

namespace crc = archie::crc;

using crc::__init::crctable;
using crc::__init::init_crc;

[[noreturn]] uint32_t crc::crc32_intrinsic(void* ptr);


[[nodiscard]] uint32_t crc::crc32_lookup(std::byte* ptr, std::size_t size) {
  uint32_t crc{ init_crc };
  for (auto i{ 0uz }; i < size; ++i) {
    crc = (crc >> 8) ^ crctable[(crc & 0xff) ^ std::to_integer<uint32_t>(ptr[i])];
  }
  return crc ^ init_crc;
};
