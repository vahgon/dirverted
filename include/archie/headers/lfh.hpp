#ifndef ARCHIE_HEADERS_LFH_HPP_
#define ARCHIE_HEADERS_LFH_HPP_

#include <cstdint>

namespace headers {

constexpr auto ZIP_THRESHOLD{ 1024ULL * 1024ULL * 1024ULL };

struct __attribute__((packed)) lfh {
  const uint32_t  m_file_signature{ 0x504b0304 };
  const uint16_t  m_gen_purpose_flag{ 0x0000 };
  const uint16_t  m_comp_method{ 0x0000 };
  uint32_t        m_mtime;  // MS-DOS 16-bit date 16-bit time
  uint32_t        m_crc32;
  uint32_t        m_uncompressed_size;
  uint32_t        m_compressed_size;
  uint16_t        m_path_size;
  uint16_t        m_extra_field_size;

  // m_path_name_bytes
  lfh() = default;
};

}  // namespace headers

#endif  // ARCHIE_HEADERS_LFH_HPP_
