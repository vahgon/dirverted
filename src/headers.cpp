#include "archie/headers.hpp"

#include <cstdint>
#include <cstdlib>
#include <filesystem>

namespace header  = archie::__header;
using     path    = std::filesystem::path;

void header::local_file_header(const path& p, uint32_t& crc) {

  uint32_t crc32{ crc };
  uint32_t mod_time;

  uint16_t name_len;
  uint16_t extra_field_len;

  std::byte* a = new std::byte[5];

  std::byte* file_name;
  std::byte* extra_field;
}

/* 20 bytes */
void header::eocd_locator() {
  uint32_t eocd64_disk;
  uint64_t eocd64_start_offset;
  uint32_t total_num_disks;
}

void header::ext_info_field_zip64(size_t& file_size, void*) {
  uint16_t extra_field_chunk_size;
  std::byte* size_uncompressed{ reinterpret_cast<std::byte*>(&file_size) };
  std::byte* size_compressed{ size_uncompressed };
  uint64_t local_header_rec_off;
  uint32_t file_disk_num;
}

void header::cdfh() {
  uint16_t ver_made_by;
  uint16_t ver_needed;
  uint16_t gen_flag;
  uint16_t last_mod_time;
  uint16_t last_mod_date;
  uint32_t crc32;
  uint16_t name_len;
  uint16_t extra_field_len;
  uint16_t file_comment_len;
  uint16_t internal_file_attrs_;
  uint16_t external_file_attrs_;
  std::byte* file_name;
  std::byte* extra_field;
  std::byte* file_comment;
}

void header::eocd() {
  uint64_t EOCD64_size;
  uint16_t ver_made_by;
  uint16_t ver_needed;
  uint32_t curr_disk_num;
  uint32_t cd_disk_location;
  uint64_t cdr_on_curr_disk;
  uint64_t num_of_cdr;
  uint64_t size_of_cdr;
  uint64_t start_of_cdr_offset;  //  relative to beginning of archive
}

