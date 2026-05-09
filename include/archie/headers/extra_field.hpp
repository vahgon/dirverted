#ifndef ARCHIE_HEADERS_EXTRA_FIELD_HPP_
#define ARCHIE_HEADERS_EXTRA_FIELD_HPP_

#include <cstdint>

namespace headers::extra_field {

struct __attribute__((packed)) zip {
  const uint16_t  m_id{ 0x5455 };
  const uint16_t  m_extra_field_size{ 5 };
  const uint8_t   m_flags{ 0x80 };  // Only mod time bit set
  uint32_t        m_mtime;

  zip() = default;
};

struct __attribute__((packed)) zip64 {
  const uint16_t  m_id{ 0x0001 };
  const uint16_t  m_extra_field_size{ 28 };
  uint64_t        m_uncompressed_size;
  uint64_t        m_compressed_size;
  uint64_t        m_lfh_offset;
  uint32_t        m_this_file_disk_offset;

  zip64() = default;
};

}  // namespace headers::extra_field

#endif  // ARCHIE_HEADERS_EXTRA_FIELD_HPP_
