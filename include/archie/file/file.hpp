#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <filesystem>
#include <memory>
#include <string_view>

#include "archie/headers/lfh.hpp"

namespace archie {

class files {
 public:
  explicit files(const std::string_view t_path)
  : m_path{ t_path } {}

 private:
  const std::string_view        m_path{};
  std::size_t                   m_size{};
  std::unique_ptr<std::byte[]>  m_buff{};
};

class file {
 public:
  explicit file(const std::filesystem::path& t_path)
  : m_path{ t_path }
  , m_size{ std::filesystem::file_size(t_path) }
  , m_raw_buffer{ std::make_unique<std::byte[]>(m_size) }
  , m_path_str{ t_path.string() } {}

  void get_filebuf();

  void set_file_attrs();

  void* set_lfh_attrs() noexcept;

 private:
  const std::filesystem::path&  m_path{};
  std::size_t                   m_size{};
  std::unique_ptr<std::byte[]>  m_raw_buffer{};
  std::string_view              m_path_str{};
  std::uint32_t                 m_crc32{};
  std::uint32_t                 m_ctime{};
  std::uint16_t                 m_path_str_bytes{};
  std::unique_ptr<std::byte[]> m_local_file_header;
};

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
