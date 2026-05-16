#ifndef ARCHIE_HEADERS_EOCD_HPP_
#define ARCHIE_HEADERS_EOCD_HPP_

#include <cstdint>

namespace headers::eocd {

constexpr uint32_t ZIP_MAGIC_BYTES{ 0x504b0506 };

constexpr auto COMMENT{ "vah's archie" };

constexpr auto ZIP{
  sizeof(ZIP_MAGIC_BYTES) +  // magic bytes
  sizeof(uint16_t)        +  // current disk num
  sizeof(uint16_t)        +  // disk num of central directory location
  sizeof(uint16_t)        +  // total disks;
  sizeof(uint32_t)        +  // size of central directory
  sizeof(uint32_t)        +  // central directory's offset
  sizeof(uint16_t)           // size of comment
  // comment
};

constexpr uint32_t ZIP64_MAGIC_BYTES{ 0x504b0606 };

constexpr auto ZIP64{
  sizeof(ZIP64_MAGIC_BYTES) +  // magic bytes
  sizeof(uint64_t)          +  // size of EOCD - 12
  sizeof(uint32_t)          +  // current disk num
  sizeof(uint32_t)          +  // central directory disk location
  sizeof(uint64_t)          +  // number of CD's on current disk
  sizeof(uint64_t)          +
  sizeof(uint64_t) +
  sizeof(uint64_t)
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
