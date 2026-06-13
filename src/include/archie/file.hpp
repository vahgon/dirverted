#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <concepts>
#include <filesystem>
#include <string>

namespace archie {

enum class HeaderType : std::uint8_t {
  LocalFileHeader = 0,
};

class file {
  using path      = std::filesystem::path;
  using basic_str = std::basic_string<char>;

 public:
  file() = default;


  file(file const&) = delete;
  file(file&&) noexcept = default;

  explicit file(path const t_path)
  : m_path { t_path } {};

  explicit file(basic_str const t_str)
  : m_path{ t_str } {};

  file& operator=(file const&) = delete;
  file& operator=(file&&) noexcept;
  file& operator=(basic_str&&);

  template<class T>
  requires std::constructible_from<path, T>
  file& operator=(T const& rhs)
  { return *this = file(rhs); }

  void* create_header(HeaderType const) const;

  // returns num of bytes added
  std::size_t insert_at(void*, size_t const);
  std::size_t prepend(void*);
  std::size_t append(void*);

  std::size_t open();

  std::size_t write();

  // getters
  path const& filepath() const noexcept { return m_path; }
  std::size_t size() const noexcept { return m_size; }

  ~file() = default;

 private:
  std::filesystem::path m_path{};
  std::size_t           m_size{};
};

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
