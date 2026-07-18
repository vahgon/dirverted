#pragma once

#ifndef __linux__
# ifdef _WIN32
#   warning statx not supported on WIN32 platforms
# else
#   warning statx not supported for current platform
# endif
#endif

#include <sys/stat.h>

#include <cstdint>

#if (STATX_BASIC_STATS | STATX_BTIME)

namespace archie::io {

using statx_t = struct statx;

inline int archie_statx(int dirfd, char const* path, int flags, uint32_t mask, statx_t& st) {
  return statx(dirfd, path, flags, mask, __builtin_addressof(st));
}

}  // namespace archie::io

#else
# warning <sys/stat.h> did not provide an implementation for statx
#endif
