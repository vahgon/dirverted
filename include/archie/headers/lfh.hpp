#ifndef ARCHIE_HEADERS_LFH_HPP_
#define ARCHIE_HEADERS_LFH_HPP_

#include <cstdint>

namespace headers::lfh {

constexpr auto ZIP{
  sizeof(uint32_t) +  // file signature
  sizeof(uint16_t) +  // general purpose flag
  sizeof(uint16_t) +  // compression method
  sizeof(uint32_t) +  // last modified time M-DOS
  sizeof(uint32_t) +  // crc32 calculation
  sizeof(uint32_t) +  // size of uncompressed data
  sizeof(uint32_t) +  // size of compressed data
  sizeof(uint16_t) +  // size of path string
  sizeof(uint16_t)    // size of extra field
  //  file name
  //  extra-field
};

constexpr auto ZIP_THRESHOLD{ 1024ULL * 1024ULL * 1024ULL };

constexpr uint32_t FILE_SIGNATURE{ 0x504b0304 };

constexpr uint16_t GEN_PURPOSE_FLAGS{ 0x0000 };

constexpr uint16_t COMP_METHOD{ 0x0000 };

}  // namespace headers::lfh

#endif  // ARCHIE_HEADERS_LFH_HPP_
