#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <concepts>
#include <cstddef>
#include <filesystem>
#include <string>
#include <type_traits>
#include <utility>

namespace archie {

inline constexpr std::size_t Zip64Threshold{ 1024uz * 1024uz * 1024uz };
inline constexpr std::size_t HeapThreshold{ 1024uz };

template<bool> struct is_zip64    : std::true_type {};
template<> struct is_zip64<false> : std::false_type {};

template<bool SizeThresh>
using is_zip64_t = is_zip64<SizeThresh>::type;

template<bool SizeThresh>
inline constexpr bool is_zip64_v = is_zip64<SizeThresh>::value;

template<std::size_t FSize1, std::size_t FSize2>
concept IsZip64 = is_zip64_v<(FSize1 > FSize2)>;

class file;

template<typename T>
concept RawFSizeType =
  std::convertible_to<std::remove_cvref_t<T>, std::size_t>;

template<typename T>
concept RawStdFilesysPath =
  std::same_as<std::remove_cvref_t<T>, std::filesystem::directory_entry> ||
  std::same_as<std::remove_cvref_t<T>, std::filesystem::path>;

template<typename T>
concept FileComparisonTypes =
  std::same_as<file, std::remove_cvref_t<T> > ||
  RawFSizeType<T>;

enum class HeaderType : std::uint8_t {
  LocalFileHeader = 0,
  CentralDirRecord,
  EndOfCentralDirRecord,
};

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

decltype(auto) operator=(this auto&& self, auto&& rhs) {
  using rhs_type = decltype(rhs);

  if constexpr (requires { rhs.m_path; }) {
    if (self.m_path == rhs.m_path) [[unlikely]] {
      return self;
    } else {
      return self = std::forward<rhs_type>(rhs);
    }
  } else if constexpr (std::convertible_to<rhs_type, std::string>) {
    self.m_path = std::forward<rhs_type>(rhs);
    self.m_size = std::filesystem::file_size(self.m_path);
    return self;
  } else if constexpr (RawStdFilesysPath<rhs_type>) {
    if (self.m_path == rhs) {
      return self;
    } else {
      return self = std::forward<rhs_type>(rhs);
    }
  } else {
    static_assert(false, "Invalid type used in assignment to archie::file object");
  }
}

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
