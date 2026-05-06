#ifndef ARCHIE_HEADERS_LFH_HPP_
#define ARCHIE_HEADERS_LFH_HPP_

#include <cstdint>
#include <variant>

#include "archie/headers/extra_field.hpp"

namespace headers {

struct __attribute__((packed)) lfh {
 public:
  const uint32_t file_signature{ 0x504b0304 };

  const uint16_t gen_purpose_flag{ 0x0000 };

  const uint16_t comp_method{ 0x0000 };

  uint32_t file_last_mod;  // MS-DOS 16-bit date 16-bit time

  uint32_t crc32;

  uint32_t uncomp_sz;

  uint32_t comp_sz;

  uint16_t file_name_len;

  uint16_t ext_field_len;

// path chars
 public:
  lfh() = default;

  void generate_extra_field(uintmax_t sizeof_data) {
  }
};

}  // namespace headers

#endif  // ARCHIE_HEADERS_LFH_HPP_
