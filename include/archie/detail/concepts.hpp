#pragma once

#include <fcntl.h>

#include <concepts>
#include <type_traits>

#include <archie/detail/types.hpp>

namespace archie::detail {

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

namespace archie::path::concepts {

template<typename T>
concept path_char =
  detail::is_char_t<T> ||
  detail::is_wchar_t<T>;

}  // namespace archie::path::concepts

namespace archie::file::concepts {

template<typename T>
concept path_or_fd =
  detail::is_path_t<T> ||
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

}  // namespace archie::file::concepts
