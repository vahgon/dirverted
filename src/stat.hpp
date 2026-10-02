#pragma once

#include "detail.hpp"

namespace dvrt::io {

#ifdef __linux__
[[nodiscard]] int stat(char const*, ::stat&) noexcept;

[[nodiscard]] int stat(char const*, int, int, ::stat&) noexcept;

[[nodiscard]] int fstat(int, ::stat&) noexcept;

[[nodiscard]] int lstat(int, ::stat&) noexcept;

[[nodiscard]] int stat(char const*, int, ::statx&, unsigned int, int) noexcept;

[[nodiscard]] int stat(char const*, int, ::statx&, int) noexcept;

[[nodiscard]] type::stat_t* allocate_stat() noexcept;

[[nodiscard]] uint32_t set_file_stats(int, uint32_t) noexcept;
#elifdef _WIN32
#endif

}  // namespace dvrt::io
