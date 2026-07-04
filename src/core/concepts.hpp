#ifndef SRC_CORE_CONCEPTS_HPP_
#define SRC_CORE_CONCEPTS_HPP_

#include <concepts>
#include <filesystem>
#include <string>

namespace archie { class file; }

namespace archie::concepts {

template<bool> struct is_zip64    : std::true_type {};
template<> struct is_zip64<false> : std::false_type {};

template<bool SizeThresh>
using is_zip64_t = is_zip64<SizeThresh>::type;

template<bool SizeThresh>
inline constexpr bool is_zip64_v = is_zip64<SizeThresh>::value;

template<typename T>
concept RawFileSizeType =
  std::convertible_to<std::remove_cvref_t<T>, std::size_t>;

template<typename T>
concept StrType =
  std::same_as<std::remove_cvref_t<T>, std::string> ||
  std::convertible_to<std::remove_cvref_t<T>, std::string>;

template<typename T>
concept RawPathSrc =
  std::same_as<std::remove_cvref_t<T>, std::filesystem::directory_entry> ||
  std::same_as<std::remove_cvref_t<T>, std::filesystem::path>;

template<typename T>
concept FileComparisonType =
  std::same_as<file, std::remove_cvref_t<T> > ||
  RawFileSizeType<T>;

template<typename T>
concept ArchieFileType =
  std::same_as<file, std::remove_cvref_t<T> >;

template<typename T>
concept ArchieFileSrcType =
  (RawPathSrc<T> || StrType<T>) &&
  !ArchieFileType<T>;

}  // namespace archie::concepts

#endif  // SRC_CORE_CONCEPTS_HPP_
