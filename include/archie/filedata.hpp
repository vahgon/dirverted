#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

#include "archie/converter.hpp"
#include "archie/crc.hpp"

namespace archie {

class file {
 private:
  const std::filesystem::path& p_;

  const size_t file_size_;

  mutable uint32_t crc32;

  std::shared_ptr<std::byte[]> file_name_;

  std::shared_ptr<std::byte[]> buffer_;

 public:
  explicit file(const std::filesystem::path&);

  void get_byte_reps();

  void read_bytes();

  friend void convert::path_bytes(const std::filesystem::path&, std::weak_ptr<std::byte[]>);

  friend uint16_t convert::path_size_bytes(const std::string&);

  friend uint32_t crc::crc32_lookup(std::weak_ptr<std::byte[]>, const size_t&);

  friend uint32_t crc::crc32_intrinsic(std::weak_ptr<std::byte[]>, const size_t &);
};

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
