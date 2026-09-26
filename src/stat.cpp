#include "stat.hpp"

#ifdef __linux__
# include <sys/stat.h>
#endif

namespace io        = dvrt::io;
namespace constants = dvrt::constants;

using stat_mode_t = std::conditional_t<constants::StatxSupported, uint16_t, int>;

#ifdef __linux__
int io::stat(char const* path, struct stat& st) noexcept {
  return !::stat(path, __builtin_addressof(st));
}

int io::stat(char const* path, int fd, int flags, struct stat& st) noexcept {
  return !::fstatat(fd, path, __builtin_addressof(st), flags);
}

int io::stat(char const* path,
             int dfd,
             struct statx& st,
             uint32_t mask = flags::statx::BasicStats,
             int flags = flags::AtEmptyPath) noexcept {
  return !::statx(dfd, path, flags, mask, __builtin_addressof(st));
}

int io::stat(char const* path, int dfd, struct statx& st, int flags = flags::AtEmptyPath) noexcept {
  return !::statx(dfd, path, flags, flags::statx::BasicStats, __builtin_addressof(st));
}

int dvrt::io::is_regfile(stat_mode_t st_mode) noexcept {
  S_ISREG(st_mode);
}
#elifdef _WIN32
#endif
