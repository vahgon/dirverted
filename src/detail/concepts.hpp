#ifndef ARCHIE_DETAIL_CONCEPTS_HPP_
#define ARCHIE_DETAIL_CONCEPTS_HPP_

#include <concepts>

namespace concepts {

template<typename T>
concept is_char_t =
  std::same_as<char, std::remove_cvref_t<T> > &&
  sizeof(std::remove_cvref_t<T>) == 1;

template<typename T>
concept is_wchar_t =
  std::same_as<wchar_t, std::remove_cvref_t<T> > &&
  sizeof(std::remove_cvref_t<T>) == 4;

}  // namespace concepts

namespace archie::path::concepts {

template<typename T>
concept path_char_t =
  ::concepts::is_char_t<T> ||
  ::concepts::is_wchar_t<T>;

}  // namespace archie::path::concepts

#endif  // ARCHIE_DETAIL_CONCEPTS_HPP_
