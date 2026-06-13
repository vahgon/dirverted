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

  template<typename T>
    requires std::same_as<T, std::string>
  file& operator=(T&& rhs)
  { return *this = file(std::forward(rhs)); }

  decltype(auto) operator()(this file&);
  decltype(auto) operator[](this file&, std::size_t);

  friend bool operator==(file const&, file const&);
  friend bool operator!=(file const&, file const&);

  friend bool operator<(file const&, file const&);
  friend bool operator>(file const&, file const&);

  friend bool operator<=(file const&, file const&);
  friend bool operator>=(file const&, file const&);

  friend std::ostream& operator<<(std::ostream&, file const&);
  friend std::istream& operator>>(std::istream&, file const&);

  void* gen_header(HeaderType const) const;

  std::size_t insert_at(void*, std::size_t const);
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
