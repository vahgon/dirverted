#ifndef ARCHIE_HEADERS_EOCD_HPP_
#define ARCHIE_HEADERS_EOCD_HPP_

#include <cstdint>

namespace headers::eocd {

struct __attribute__((packed)) zip {
  const uint32_t    m_magic_bytes{ 0x504b0506 };
  uint16_t          m_curr_disk_num;
  uint16_t          m_central_dir_disk_loc;
  uint16_t          m_total_disks;
  uint32_t          m_central_dir_size;
  uint32_t          m_central_dir_offset;
  uint16_t          m_comment_size;
  uint8_t           m_comment[13]{ "vah's archie" };

  zip() = default;
};

struct __attribute__((packed)) zip64 {
  const uint32_t  m_magic_bytes{ 0x504b0606 };
  uint64_t        m_eocd_size;
  uint32_t        m_curr_disk_num;
  uint32_t        m_central_dir_disk_loc;
  uint64_t        m_central_dir_curr_disk_num;
  uint64_t        m_central_dir_num;
  uint64_t        m_central_dir_size;
  uint64_t        m_central_dir_start_offset;

  zip64() = default;
};

struct __attribute__((packed)) locator64 {
  const uint32_t  m_magic_bytes{ 0x504b0607 };
  uint32_t        m_eocd64_disk_loc;
  uint64_t        m_eocd64_offset;
  uint32_t        m_num_of_disks;

  locator64() = default;
};

}  // namespace headers::eocd

#endif  // ARCHIE_HEADERS_EOCD_HPP_
