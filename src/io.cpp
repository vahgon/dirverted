#ifdef __linux__
# include <syscall.h>
# include <unistd.h>
#elifdef _WIN32
#endif

#include <fcntl.h>

#include "flags.hpp"
#include "io.hpp"
#include "stat.hpp"

using file_desc = dvrt::detail::file_desc;
using ulong     = unsigned long;
namespace io    = dvrt::detail::io;

#ifdef __linux__
# ifdef SYS_openat2
int io::open(char const* path, int dirfd) noexcept {
  struct open_how how{ .flags   = O_RDONLY,
                       .resolve = RESOLVE_IN_ROOT };
  return static_cast<int>(
    syscall(SYS_openat2, dirfd, path, __builtin_addressof(how), sizeof(how)));
}

int io::open(char const* path, int dirfd, ulong flags, ulong resolve, ulong mode) noexcept {
  struct open_how how{ .flags   = flags,
                       .mode    = mode,
                       .resolve = resolve };
  return static_cast<int>(
    syscall(SYS_openat2, dirfd, path, __builtin_addressof(how), sizeof(how)));
}

# else
int io::open(char const* path, int flags) noexcept {
  return ::open(path, flags);
}
# endif
int io::close(int fd) noexcept {
  return !::close(fd);
}
#elifdef _WIN32
#endif

file_desc::file_desc(file_desc&& rhs) noexcept {
  if (this != &rhs) {
    m_fd = rhs.m_fd;
    rhs.m_fd = -1;
  }
}

file_desc& file_desc::operator=(file_desc&& rhs) noexcept {
  if (this != &rhs) {
    if (m_fd != -1) io::close(m_fd);
    m_fd = rhs.m_fd;
    rhs.m_fd = -1;
  }
  return *this;
}

int64_t file_desc::get_mtime(this file_desc fd) noexcept {
  stat_t st;

  if (io::stat(nullptr, fd.m_fd, flags::AtEmptyPath, __builtin_addressof(st), flags::MTime)) {
    return st.stx_mtime.tv_sec;
  } else {
    return 0;
  }
}

int64_t file_desc::get_atime(this file_desc fd) noexcept {
  stat_t st;

  if (io::stat(nullptr, fd.m_fd, flags::AtEmptyPath, __builtin_addressof(st), flags::ATime)) {
    return st.stx_mtime.tv_sec;
  } else {
    return 0;
  }
}

int64_t file_desc::get_btime(this file_desc fd) noexcept {
  stat_t st;

  if (io::stat(nullptr, fd.m_fd, flags::AtEmptyPath, __builtin_addressof(st), flags::BTime)) {
    return st.stx_mtime.tv_sec;
  } else {
    return 0;
  }
}

uint64_t file_desc::get_size(this file_desc fd) noexcept {
  stat_t st;

  if (io::stat(nullptr, fd.m_fd, flags::AtEmptyPath, __builtin_addressof(st), flags::Size)) {
    return st.stx_size;
  } else {
    return 0;
  }
}
