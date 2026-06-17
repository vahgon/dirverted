#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <concepts>
#include <cstddef>
#include <filesystem>
#include <string>
#include <utility>

namespace archie {

enum class HeaderType : std::uint8_t {
  LocalFileHeader = 0,
};

class file;

template<typename T>
concept RawFSizeType = std::integral<T> && std::convertible_to<T, std::size_t>;

template<typename T>
concept FileComparisonTypes = std::same_as<file, T> || RawFSizeType<T>;

class file {
 public:
  file() = default;

  explicit file(std::filesystem::path const&);
  explicit file(std::string_view const);

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

  bool operator==(FileComparisonTypes auto const& rhs) const noexcept {
    if constexpr (std::same_as<decltype(rhs), decltype(this)>) {
      return this->m_size == rhs.m_size;
    } else if (RawFSizeType<decltype(rhs)>) {
      return this->m_size == rhs;
    }
  }

  bool operator!=(FileComparisonTypes auto const& rhs) const noexcept {
    if constexpr (std::same_as<decltype(rhs), decltype(this)>) {
      return !(this->m_size == rhs.m_size);
    } else if (RawFSizeType<decltype(rhs)>) {
      return !(this->m_size == rhs);
    }
  }

  bool operator<(FileComparisonTypes auto const& rhs) const noexcept {
    if constexpr (std::same_as<decltype(rhs), decltype(this)>) {
      return this->m_size < rhs.m_size;
    } else if (RawFSizeType<decltype(rhs)>) {
      return this->m_size < rhs;
    }
  }

  bool operator>(FileComparisonTypes auto const& rhs) const noexcept {
    if constexpr (std::same_as<decltype(rhs), decltype(this)>) {
      return !(this->m_size < rhs.m_size);
    } else if (RawFSizeType<decltype(rhs)>) {
      return !(this->m_size < rhs);
    }
  }

  bool operator<=(FileComparisonTypes auto const& rhs) const noexcept {
    if constexpr (std::same_as<decltype(rhs), decltype(this)>) {
      return this->m_size <= rhs.m_size;
    } else if (RawFSizeType<decltype(rhs)>) {
      return this->m_size <= rhs;
    }
  }

  bool operator>=(FileComparisonTypes auto const& rhs) const noexcept {
    if constexpr (std::same_as<decltype(rhs), decltype(this)>) {
      return !(this->m_size <= rhs.m_size);
    } else if (RawFSizeType<decltype(rhs)>) {
      return !(this->m_size <= rhs);
    }
  }

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
