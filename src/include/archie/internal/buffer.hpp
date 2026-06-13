#ifndef ARCHIE_INTERNAL_HPP_
#define ARCHIE_INTERNAL_HPP_

#include <concepts>
#include <filesystem>
#include <fstream>
#include <vector>

namespace archie::internal {

inline constexpr size_t Zip64Threshold{ 1024uz * 1024uz * 1024uz };

template<std::size_t Sz>
inline auto write_into_buff(std::unsigned_integral auto const... data) {
  static_assert((sizeof(data) + ...) <= Sz);
}

template<std::signed_integral N>
inline constexpr auto time_to_int(auto const& time) {
  return static_cast<N>(time.count());
}

bool read_file(std::filesystem::path const&);

std::uint32_t mtime(std::filesystem::path const&);

struct zip_tag;

struct zip64_tag;

class file_base {
 public:
  file_base() = default;

  explicit file_base(std::filesystem::path const& t_path)
  : m_path{ t_path }
  , m_file_size{ std::filesystem::file_size(t_path) } {}

 private:
  std::filesystem::path const& m_path{};
  std::size_t m_file_size{};
  std::uint32_t m_crc32{};
};

class file_handle : public file_base {
 public:
  file_handle() = default;

  explicit file_handle(std::filesystem::path const& t_path) : file_base{t_path} {}
 private:
};

template<std::size_t FSize>
struct file_data {
  using zip_type = std::conditional_t<(FSize < Zip64Threshold), zip_tag, zip64_tag>;

  explicit file_data(std::filesystem::path const& t_path)
  : path{ t_path }, file_size{ FSize } {}

  std::filesystem::path const& path{};
  std::uint32_t                crc32{};
  std::size_t                  file_size{};
};

template<typename FileParams>
class file_handler {
  using zip_type = FileParams::zip_type;

 public:
  file_handler() = default;

  // file_handler(const std::filesystem::path&) requires std::same_as<zip_type, zip64_tag>;

  file_handler(file_handler const&) = delete;

  file_handler& operator=(file_handler const&) = delete;

  file_handler& operator=(file_handler&&) noexcept;

  friend bool operator==(file_handler& lhs, file_handler& rhs) {
    std::vector<int> test{};
    return lhs.m_attrs.path == rhs.m_attrs.path;
  }

  friend bool operator<(file_handler& lhs, file_handler& rhs) {
    return lhs.m_attrs.size < rhs.m_attrs.size;
  }

  std::size_t write(void*);

  std::size_t write_bytes(std::byte*);

  bool open();

  ~file_handler() {
    if (m_file.is_open()) {
      m_file.close(); 
    }
  }

 protected:
  struct file_attrs {
    std::filesystem::path const&  path{ FileParams::path };
    std::uint32_t         const   crc32{ FileParams::crc32 };
    std::size_t           const   size{ FileParams::file_size };
  };

 private:
  file_attrs                  m_attrs{};
  std::filebuf                m_file{};
  std::array<std::byte, 4096> m_buf{};
};

}  // namespace archie::internal

#endif  // ARCHIE_INTERNAL_HPP_
