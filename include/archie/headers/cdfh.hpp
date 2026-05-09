#ifndef ARCHIE_HEADERS_CDFH_HPP_
#define ARCHIE_HEADERS_CDFH_HPP_

#include <cstdint>

namespace headers {

struct __attribute__((packed)) cdfh {
  const uint32_t    m_magic_bytes{ 0x504b0102 };
  uint32_t          m_mtime;
  uint32_t          m_crc32;
  uint32_t          m_compressed_size;
  uint64_t          m_uncompresed_size;;
  uint16_t          m_path_size;
  uint16_t          m_extra_field_size;
  uint16_t          m_comment_size;
  uint16_t          m_file_disk_loc;
  uint16_t          m_internal_attrs;
  uint32_t          m_external_attrs;
  uint32_t          m_lfh_offset;
  uint8_t           m_comment[13]{ "vah's archie" };
};

}  // namespace headers::cdfh

#endif  // ARCHIE_HEADERS_CDFH_HPP_
