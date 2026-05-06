#ifndef ARCHIE_HEADERS_EXTRA_FIELD_HPP_
#define ARCHIE_HEADERS_EXTRA_FIELD_HPP_

#include <cstdint>

namespace headers::extra_field {

template<bool IsZip64>
struct zip{ };

// zip
template<>
struct __attribute__((packed)) zip<false> {
 public:
  const uint16_t id_{ 0x5455 };

  const uint16_t exf_sz_{ 5 };

  const uint8_t flags_{ 0x80 };  // Only mod time bit set

  uint32_t mod_time_;

 public:
  zip() = default;
};

// zip64
template<>
struct __attribute__((packed)) zip<true> {
 public:
  const uint16_t id_{ 0x0001 };

  const uint16_t exf_sz_{ 28 };

  uint64_t uncomp_sz_;

  uint64_t comp_sz_;

  uint64_t lfh_off_;

  uint32_t file_disk_loc_;

 public:
  zip() = default;
};

}  // namespace headers::extra_field

#endif  // ARCHIE_HEADERS_EXTRA_FIELD_HPP_
