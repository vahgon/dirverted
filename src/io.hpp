#pragma once

#ifdef __linux__
# include <limits.h>
# include <stdlib.h>
#endif


#include <memory>

namespace dvrt::io::detail {

inline constexpr int InvalidFileDesc = -1;

[[nodiscard]] int open(char const*) noexcept;

[[nodiscard]] int openat(char const*, int) noexcept;

int close(int) noexcept;

#ifdef __linux__
[[nodiscard]] char* abs_path(char const*) noexcept;

[[nodiscard]] int duplicate_fd(int) noexcept;

[[nodiscard]] int duplicate_fd(int, int) noexcept;
#endif

#ifdef _WIN32
[[nodiscard]] int open(wchar_t const*, int) noexcept;
#endif

}  // namespace dvrt::detail::io

#ifdef __linux__
# include <dirent.h>
#endif 

namespace dvrt::io::detail {

#ifdef __linux__
DIR* fdiropen(int) noexcept;
#endif

inline auto del = [](char const* ptr) { std::free(static_cast<void*>(const_cast<char*>(ptr))); };

using abs_path_t = std::unique_ptr<char const*, decltype(del)>;

inline auto get_abs(char const* path) {
  return std::unique_ptr<char const[], decltype(del)>{ abs_path(path), del };
}

}  // namespace dvrt::detail::io
