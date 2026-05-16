#ifndef ARCHIE_HEADERS_EXTRA_FIELD_HPP_
#define ARCHIE_HEADERS_EXTRA_FIELD_HPP_

#include <cstdint>

namespace headers::extra_field {

constexpr uint16_t ZIP_ID{ 0x5455 };

constexpr uint16_t ZIP_EXFIELD_SIZE{ 5 };

constexpr uint8_t ZIP_FLAGS{ 0x80 };  // Only mod time bit set

constexpr auto ZIP_SIZE{
  sizeof(ZIP_ID)            +  // id
  sizeof(ZIP_EXFIELD_SIZE)  +  // size of extra field
  sizeof(ZIP_FLAGS)         +  // flags
  sizeof(uint32_t)             // last modified time
};

constexpr uint16_t ZIP64_ID{ 0x0001 };

constexpr uint16_t ZIP64_EXFIELD_SIZE{ 28 };

constexpr auto ZIP64_SIZE{
  sizeof(ZIP64_ID)            +  // id
  sizeof(ZIP64_EXFIELD_SIZE)  +  // size of extra field
  sizeof(uint64_t)            +  // size of uncompressed data
  sizeof(uint64_t)            +  // size of compressed data
  sizeof(uint64_t)            +  // offset of local file header
  sizeof(uint32_t)               // disk num of current file's location
};

}  // namespace headers::extra_field

#endif  // ARCHIE_HEADERS_EXTRA_FIELD_HPP_
