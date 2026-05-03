#include "archie/crc.hpp"
#include <stdexcept>

#ifndef USE_IMMINTRIN
#include <immintrin.h>
#endif

#include <cstdint>

namespace crc   = archie::crc;
using     byte  = std::byte;

using archie::crc::__internal::crctable;
using archie::crc::__internal::init_crc;

uint32_t crc::crc32_intrinsic(std::weak_ptr<byte[]> bytes, const size_t& file_size) {
  return 0;
}

uint32_t crc::crc32_lookup(std::weak_ptr<byte[]> bytes, const size_t& file_size) {
  uint32_t crc{ init_crc };
  if (auto ptr = bytes.lock()) {
    auto t = ptr[0];
    for (size_t i = 0; i < file_size; i++)
      crc = (crc >> 8) ^ crctable[(crc & 0xff) ^ static_cast<uint32_t>(ptr[i])];
    return crc ^ init_crc;
  } else {
    throw std::runtime_error(".lock() failed in crc::crc32_lookup()");
  }
}

// uint32_t crc::crc32_lookup(std::weak_ptr<byte[]> ptr,
// const size_t& file_size) {
//   uint32_t crc{ internal::init_crc };
//   size_t curr_size{ file_size };
//
//   while (curr_size--)
//     crc = (crc >> 8) ^ internal::crctable[(crc & 0xff) ^ *bytes++];
//   return crc ^ internal::init_crc;
// }
