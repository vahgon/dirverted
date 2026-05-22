#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <concepts>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <vector>

namespace archie {

class file {
 public:
  explicit file(const std::filesystem::path& t_path)
  : m_path{ t_path }
  , m_size{ std::filesystem::file_size(t_path) } {
    m_buff.reserve(m_size);
  }

  const std::byte* buffer_bytes() const noexcept { return m_buff.data(); }

  std::size_t file_size() { return m_size; }

  template<std::unsigned_integral... Fields>
  std::size_t insert_into_buff(const Fields&... data) {
    ((void)(data), ...);
  }

 protected:
  std::uint32_t mtime() const;

  std::uint32_t mtime_ext() const noexcept;

 private:
  const std::filesystem::path&  m_path{};
  std::size_t                   m_size{};
  std::vector<std::byte>        m_buff{};
};

void deserialize_fs_path(const std::filesystem::path&);

// does file need to be created with ZIP64 format extensions?
template<bool IsZip64>
std::size_t set_headers(std::size_t);

// yes
template<>
std::size_t set_headers<true>(std::size_t);   // file >= 4GiB

// no
template<>
std::size_t set_headers<false>(std::size_t);  // file < 4GiB

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
