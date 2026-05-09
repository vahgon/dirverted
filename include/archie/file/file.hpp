#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <cstdint>
#include <filesystem>
#include <string>

#include "archie/file/buffer.hpp"

namespace archie {

class file {
 public:
  explicit file(const std::filesystem::path& t_path)
  : m_path{ t_path } {}

  ~file() { free(m_buffer); }

  void set_file_info();

  void get_crc();

  void determine_zip_filetype();

  void prepend_lfh();

 private:
  const std::filesystem::path m_path;
  uintmax_t                   m_size;
  uint32_t                    m_crc32;
  uint32_t                    m_dos_mtime_;
  std::string                 m_path_str;
  std::byte*                  m_buffer;

 private:
  friend uint64_t bytes::read_buffer(file*);

  friend void* bytes::path_str_to_bytes(file*);

  friend void* bytes::memory_mapped_file(file*);
};

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
