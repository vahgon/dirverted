#pragma once

#include <cstdint>

#include "detail.hpp"
#include "flags.hpp"

namespace dvrt::io {

#ifdef __linux__
[[nodiscard]] int stat(char const*, ::stat&) noexcept;

[[nodiscard]] int stat(char const*, int, int, ::stat&) noexcept;

[[nodiscard]] int stat(char const*, int, ::statx&, uint32_t, int) noexcept;

[[nodiscard]] int stat(char const*, int, ::statx&, int) noexcept;

[[nodiscard]] int is_regfile(std::conditional_t<constants::StatxSupported, uint16_t, int>) noexcept;

[[nodiscard]] int is_symlink(std::conditional_t<constants::StatxSupported, uint16_t, int>) noexcept;

[[nodiscard]] int is_directory(std::conditional_t<constants::StatxSupported, uint16_t,int>) noexcept;
#elifdef _WIN32
#endif

}  // namespace dvrt::io

namespace dvrt {

template<concepts::IsPathFd TInput>
int stat(TInput in,
    type::stat_t& st,
    int flags = flags::AtEmptyPath,
    uint32_t mask = flags::statx::BasicStats) noexcept {
  if constexpr (constants::StatxSupported) {
    if constexpr (concepts::IsPath<TInput>) {
      return io::stat(in, flags::AtCurrWorkingDir, __builtin_addressof(st), mask, flags);
    } else if constexpr (concepts::IsFileDescriptor<TInput>) {
      return io::stat(nullptr, in, __builtin_addressof(st), mask, flags);
    }
  } else {
    if constexpr (concepts::IsPath<TInput>) {
      return io::stat(in, flags::AtCurrWorkingDir, __builtin_addressof(st), flags);
    } else if constexpr (concepts::IsFileDescriptor<TInput>) {
      return io::stat(nullptr, in, __builtin_addressof(st), flags);
    }
  }
}

}  // namespace dvrt
