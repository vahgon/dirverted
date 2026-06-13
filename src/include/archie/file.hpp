#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <concepts>
#include <filesystem>
#include <string>
#include <utility>

namespace archie {

enum class HeaderType : std::uint8_t {
  LocalFileHeader = 0,
};

class file {
 public:
  file() = default;

  explicit file(std::filesystem::path const&);
  explicit file(std::convertible_to<std::string> auto const&);

  file(file const&) = default;
  file(file&&) noexcept = default;

  template<typename T>
    requires std::constructible_from<T, std::string>
  explicit file(T&& t_path) : m_path{ std::filesystem::path(std::forward(t_path)) } {}

  ~file() = default;

  file& operator=(file const&) = default;
  file& operator=(file&&) noexcept = default;

  template<typename T>
    requires std::constructible_from<T, std::string>
  file& operator=(T&& rhs)
  { return *this = file(std::forward(rhs)); }

  decltype(auto) operator()(this file&);
  decltype(auto) operator[](this file&, std::size_t);

  bool operator==(file const&) const;
  bool operator!=(file const&) const;

  bool operator<(file const&) const;
  bool operator>(file const&) const;

  bool operator<=(file const&) const;
  bool operator>=(file const&) const;

  friend std::ostream& operator<<(std::ostream&, file const&);
  friend std::istream& operator>>(std::istream&, file const&);

  void* gen_header(HeaderType const) const;

  std::size_t insert_at(void*, std::size_t const);
  std::size_t prepend(void*);
  std::size_t append(void*);

  std::size_t open();
  std::size_t write();

  template<typename... T>
  decltype(auto) slice(this auto&, std::size_t const, std::size_t const);

  std::span<std::byte const> read_slice(std::size_t const, std::size_t const) const;

  std::filesystem::path const& filepath() const noexcept { return m_path; }
  std::size_t size() const noexcept { return m_size; }

 private:
  std::filesystem::path m_path{};
  std::size_t           m_size{};
};

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
