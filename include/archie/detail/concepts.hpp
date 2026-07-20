#pragma once

#ifdef __linux__
# include <limits.h>
#elifdef _WIN32
# include <windows.h>
#endif

#include <concepts>
#include <type_traits>

#include <archie/detail/types.hpp>

namespace archie::concepts {

template<typename T>
concept is_char_t =
  std::same_as<std::remove_cvref_t<T>, char> &&
  sizeof(std::remove_cvref_t<T>) == 1;

template<typename T>
concept is_wchar_t =
  std::same_as<std::remove_cvref_t<T>, wchar_t> &&
  sizeof(std::remove_cvref_t<T>) == 4;

template<typename T>
concept is_path_t =
  std::same_as<std::remove_cvref_t<T>, char const*>;

}  // namespace archie::detail

namespace archie::concepts {

template<typename T>
concept path_char =
  concepts::is_char_t<T> ||
  concepts::is_wchar_t<T>;

#if __linux__
inline constexpr int  MaxPathStr     = PATH_MAX;
inline constexpr char path_separator = '/';
#elif _WIN32
inline constexpr int  MaxPathStr     = MAX_PATH;
inline constexpr char path_separator = '/' || '\\';
#endif

constexpr bool is_path_separator(path_char auto const c) noexcept {
  if constexpr (concepts::is_char_t<decltype(c)>) {
    return c == path_separator;
  } else if constexpr (concepts::is_wchar_t<decltype(c)>) {
    return c == L'/' || c == L'\\';
  }
}

}  // namespace archie::concepts::path

namespace archie::concepts::file {

#ifdef __linux__
inline constexpr int MaxFileStr = NAME_MAX;
#elifdef _WIN32
inline constexpr int MaxFileStr = NAME_MAX;
#endif

template<typename T>
concept path_or_fd =
  concepts::is_path_t<T> ||
  std::same_as<std::remove_cvref_t<T>, int>;

template<typename T>
concept stat_supported =
  std::same_as<std::remove_cvref_t<T>, type::stat_t> &&
  std::is_final_v<type::stat_t>;
  
template<typename T>
concept is_statx =
  stat_supported<T> && T::is_statx;

template<typename T>
concept is_stat =
  stat_supported<T> && T::is_stat;

}  // namespace archie::concepts::file
