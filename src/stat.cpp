#include "stat.hpp"

#ifdef __linux__
# include <sys/stat.h>
#endif

#include <cstdlib>

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

void* dvrt::io::allocate_stat() noexcept {
  return std::malloc(sizeof(type::stat_t));
}

uint32_t dvrt::io::set_file_stats(int fd) noexcept {
  type::stat_t* st_ptr{ static_cast<type::stat_t*>(io::allocate_stat()) };
  uint32_t ftype{};
# ifdef __statx_defined
  if (io::stat(nullptr, fd, *st_ptr, statx_flags::BasicStats, flags::AtEmptyPath)) {
    ftype = (st_ptr->stx_mode & S_IFMT) << 16;
  }
# else
  if (io::fstat(fd, *st_ptr)) {
    ftype = (st_ptr->st_mode & S_IFMT) << 16;
  }
# endif
  std::free(st_ptr);
  return ftype;
}
#elifdef _WIN32
#endif
