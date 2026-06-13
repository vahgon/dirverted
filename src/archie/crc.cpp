#include "archie/internal/crc.hpp"

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <cstddef>
#include <cstdint>

namespace crc = archie::crc;

[[noreturn]] std::uint32_t crc::crc32_intrinsic(std::span<std::byte const>);

[[nodiscard]] std::uint32_t crc::crc32_lookup(std::span<std::byte const> bytes, std::uint32_t& crc) {
  if (crc == 0) {
    crc = init_crc;
  }

  for (auto byte : bytes) {
    crc = (crc >> 8) ^ crctable[(crc & 0xff) ^ std::to_integer<std::uint32_t>(byte)];
  }

  return crc;  // ^ 0xffffffff
}
