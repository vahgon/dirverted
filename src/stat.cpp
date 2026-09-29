#include "stat.hpp"

#ifdef __linux__
# include <sys/stat.h>
#endif

#include <cstdint>

namespace io          = dvrt::io;
namespace constants   = dvrt::constants;
namespace statx_flags = dvrt::flags::statx;

using stat_mode_t   = std::conditional_t<constants::StatxSupport,
                                        uint16_t, int>;

#ifdef __linux__
int io::stat(char const* path, struct stat& st) noexcept {
  return !::stat(path, __builtin_addressof(st));
}

int io::stat(char const* path, int fd, int flags, struct stat& st) noexcept {
  return !::fstatat(fd, path, __builtin_addressof(st), flags);
}

int io::fstat(int fd, struct stat& st) noexcept {
  return !::fstat(fd, __builtin_addressof(st));
}

# ifdef __statx_defined
int io::stat(char const* path, int dfd, struct statx& st, uint32_t mask, int flags) noexcept {
  return !::statx(dfd, path, flags, mask, __builtin_addressof(st));
}

int io::stat(char const* path, int dfd, struct statx& st, int flags) noexcept {
  return !::statx(dfd, path, flags, flags::statx::BasicStats, __builtin_addressof(st));
}
# endif

dvrt::type::stat_t* dvrt::io::allocate_stat() noexcept {
  return static_cast<type::stat_t*>(std::malloc(sizeof(type::stat_t)));
}

uint32_t dvrt::io::set_file_stats(int fd, uint32_t mask) noexcept {
  type::stat_t* st_ptr{ io::allocate_stat() };
  uint32_t stats{};
# ifdef __statx_defined
  if (io::stat(nullptr, fd, *st_ptr, mask, flags::AtEmptyPath)) {
    stats = (st_ptr->stx_mode & S_IFMT) << 16;  // file type
  }
# else
  if (io::fstat(fd, *st_ptr)) {
    ftype = (st_ptr->st_mode & S_IFMT) << 16;
  }
# endif
  std::free(static_cast<void*>(st_ptr));
  return stats;
}
#elifdef _WIN32
#endif
