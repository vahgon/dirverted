#ifndef SRC_CONSTANTS_HPP_
#define SRC_CONSTANTS_HPP_

#include <linux/limits.h>
#if __linux__
#include <limits.h>
#elif _WIN32
#include <windows.h>
#endif

#include <cstdint>

namespace constants {

#if __linux__
inline constexpr int MaxFileStr = NAME_MAX; 

inline constexpr int MaxPathStr = PATH_MAX;

inline constexpr char PreferredSeparator = '/';
#elif _WIN32
inline constexpr int MaxFileStr = NAME_MAX; 

inline constexpr int MaxPathStr = MAX_PATH;

inline constexpr wchar_t PreferredSeparator = L'\\';
#endif

}  // namespace constants;

#endif  // SRC_CONSTANTS_HPP_
