#pragma once

#include "detail.hpp"

namespace dvrt::io {

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

}  // namespace dvrt::io
