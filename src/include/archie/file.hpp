#ifndef ARCHIE_FILEDATA_HPP_
#define ARCHIE_FILEDATA_HPP_

#include <concepts>
#include <filesystem>
#include <type_traits>
#include <string>
#include <utility>

namespace archie {

class file;

inline constexpr std::size_t Zip64Threshold{ 1024uz * 1024uz * 1024uz };
inline constexpr std::size_t StackThreshold{ 1024uz };

template<bool> struct is_zip64    : std::true_type {};
template<> struct is_zip64<false> : std::false_type {};

template<bool SizeThresh>
using is_zip64_t = is_zip64<SizeThresh>::type;

template<bool SizeThresh>
inline constexpr bool is_zip64_v = is_zip64<SizeThresh>::value;

template<typename T>
concept RawFSizeType =
  std::convertible_to<std::remove_cvref_t<T>, std::size_t>;

template<typename T>
concept RawFilesysPathSrc =
  std::same_as<std::remove_cvref_t<T>, std::filesystem::directory_entry> ||
  std::same_as<std::remove_cvref_t<T>, std::filesystem::path>;

template<typename T>
concept StringSrc =
  std::convertible_to<std::remove_cvref_t<T>, std::string>;

template<typename T>
concept FileComparisonTypes =
  std::same_as<file, std::remove_cvref_t<T> > ||
  RawFSizeType<T>;

template<typename T>
concept ArchieFileType =
  std::same_as<file, std::remove_cvref_t<T> >;

template<typename T>
concept BaseSrcTypes =
  (RawFilesysPathSrc<T> || StringSrc<T>) &&
  !ArchieFileType<T>;

template<typename T>
using deduced_path_t = std::conditional_t<
  std::is_rvalue_reference_v<T>,
  decltype(std::declval<T>().m_path),
  decltype(std::forward_like<T>(std::declval<T&>().m_path))>;

enum class HeaderType : std::uint8_t {
  LocalFileHeader = 0,
  CentralDirRecord,
  EndOfCentralDirRecord,
};

class file {
 public:
  file() = default;

  explicit file(BaseSrcTypes auto&& t_path)
  : m_path{ std::forward<decltype(t_path)>(t_path) }
  , m_size{ std::filesystem::file_size(m_path) } {}

  file(file const&) = delete;

  file(file&&) noexcept = default;

  file& operator=(file const&) = delete;

  file& operator=(file&&) = default;

  auto&& operator=(this auto&& self, auto&& rhs)
    requires (!std::is_const_v<std::remove_reference_t<decltype(self)> >) &&
              BaseSrcTypes<decltype(rhs)> {
    if (self.m_path != rhs) { self.m_path = std::forward_like<decltype(rhs)>(rhs); }
    return self;
  }

  decltype(auto) operator()(this file&);
  decltype(auto) operator[](this file&, std::size_t);

bool operator==(FileComparisonTypes auto&& rhs) noexcept {
  if constexpr (requires { rhs.m_size; }) {
    return this->m_size == rhs.m_size;
  } else {
    return this->m_size == rhs;
  }
}

bool operator!=(FileComparisonTypes auto&& rhs) noexcept {
  if constexpr (requires { rhs.m_size; }) {
    return !(this->m_size == rhs.m_size);
  } else {
    return !(this->m_size == rhs);
  }
}

bool operator<(FileComparisonTypes auto&& rhs) noexcept {
  if constexpr (requires { rhs.m_size; }) {
    return this->m_size < rhs.m_size;
  } else {
    return this->m_size < rhs;
  }
}

bool operator>(FileComparisonTypes auto&& rhs) noexcept {
  if constexpr (requires { rhs.m_size; }) {
    return !(this->m_size < rhs.m_size);
  } else {
    return !(this->m_size < rhs);
  }
}

bool operator<=(FileComparisonTypes auto&& rhs) noexcept {
  if constexpr (requires { rhs.m_size; }) {
    return this->m_size <= rhs.m_size;
  } else {
    return this->m_size <= rhs;
  }
}

bool operator>=(FileComparisonTypes auto&& rhs) noexcept {
  if constexpr (requires { rhs.m_size; }) {
    return !(this->m_size <= rhs.m_size);
  } else {
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

  template<typename Self>
  auto path(this Self&& self) -> deduced_path_t<Self&&> {
    if constexpr (std::is_rvalue_reference_v<decltype(self)>) {
      return std::remove_cvref_t<decltype(self.m_path)>(std::move(self.m_path));
    } else {
      return std::forward_like<Self>(self.m_path);
    }
  }

  std::size_t size() const noexcept { return this->m_size; }

 private:
  std::filesystem::path m_path{};
  std::size_t           m_size{};
};

}  // namespace archie

#endif  // ARCHIE_FILEDATA_HPP_
