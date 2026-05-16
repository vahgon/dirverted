#ifndef ARCHIE_HEADERS_CDFH_HPP_
#define ARCHIE_HEADERS_CDFH_HPP_

#include <cstdint>

namespace headers {

struct __attribute__((packed)) cdfh {
  const std::uint32_t    m_magic_bytes{ 0x504b0102 };
  std::uint32_t          m_mtime{};
  std::uint32_t          m_crc32{};
  std::uint32_t          m_compressed_size{};
  std::uint64_t          m_uncompresed_size{};
  std::uint16_t          m_path_size{};
  std::uint16_t          m_extra_field_size{};
  std::uint16_t          m_comment_size{};
  std::uint16_t          m_file_disk_loc{};
  std::uint16_t          m_internal_attrs{};
  std::uint32_t          m_external_attrs{};
  std::uint32_t          m_lfh_offset{};
  std::uint8_t           m_comment{};
};

}  // namespace headers::cdfh

#endif  // ARCHIE_HEADERS_CDFH_HPP_
