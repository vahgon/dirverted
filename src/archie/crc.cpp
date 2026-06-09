#include "archie/internal/crc.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

namespace crc = archie::crc;

[[noreturn]] uint32_t crc::crc32_intrinsic(std::span<std::byte const>);

[[nodiscard]] uint32_t crc::crc32_lookup(std::span<std::byte const> bytes, std::size_t size) {
  uint32_t crc{ init_crc };
  for (auto i{ 0uz }; i < size; ++i) {
    crc = (crc >> 8) ^ crctable[(crc & 0xff) ^ std::to_integer<uint32_t>(bytes[i])];
  }
  return crc ^ init_crc;
}
