#include "io.hpp"

#ifdef __linux__
# include <fcntl.h>
# include <syscall.h>
# include <unistd.h>
#elifdef _WIN32
#endif

namespace io      = dvrt::detail::io;
using     fd      = dvrt::detail::fd;
using     fd_base = dvrt::detail::fd_base;

#ifdef __linux__
int io::open(char const* path) noexcept {
  return ::open(path, O_CLOEXEC);
}

# ifdef SYS_openat2
int io::openat(char const* path, int dirfd) noexcept {
  struct open_how how{ .flags   = static_cast<decltype(how.flags)>(O_CLOEXEC),
                       .resolve = static_cast<decltype(how.resolve)>(RESOLVE_IN_ROOT) };

  return static_cast<int>(
    syscall(SYS_openat2, dirfd, path, __builtin_addressof(how), sizeof(how)));
}
# else
int io::openat(char const* path, int dirfd) noexcept {
  return ::openat(dirfd, path, O_CLOEXEC);
}
# endif
int io::close(int fd) noexcept {
  return !::close(fd);
}

int io::duplicate_fd(int fd_doner) noexcept {
  if (int dupe_fd{}; (dupe_fd = ::fcntl(fd_doner, F_DUPFD_CLOEXEC, 0)) != io::InvalidFileDesc) {
    return dupe_fd;
  } else {
    return io::InvalidFileDesc;
  }
}

int io::duplicate_fd(int fd_orig, int fd_doner) noexcept {
  if (int dupe_fd{}; (dupe_fd = ::dup3(fd_doner, fd_orig, O_CLOEXEC)) != io::InvalidFileDesc) {
    return dupe_fd;
  } else {
    return io::InvalidFileDesc;
  }
}

#elifdef _WIN32
int io::open(wchar_t const* path, int flags) noexcept;
#endif

fd_base::operator bool(this fd_base self) noexcept {
  return self.m_fd != io::InvalidFileDesc;
}

fd_base::operator int(this fd_base self) noexcept {
  return self.m_fd;
}

int fd_base::raw(this fd_base self) noexcept {
  return self.m_fd;
}

fd& fd::operator=(fd const& rhs) noexcept {
  if (*this && rhs) {
    if (m_fd != rhs.m_fd) {
      m_fd = io::duplicate_fd(m_fd, rhs.m_fd);
    }
  } else if (rhs) {
    m_fd = io::duplicate_fd(rhs.m_fd);
  }
  return *this;
}

fd& fd::operator=(fd&& rhs) noexcept {
  if (m_fd != rhs.m_fd && rhs) {
    if (*this) io::close(m_fd);
    m_fd = rhs.release();
  }
  return *this;
}

fd& fd::operator=(int fd) noexcept {
  if (m_fd != io::InvalidFileDesc) io::close(m_fd);
  m_fd = fd;
  return *this;
}

fd& fd::operator=(char const* path) noexcept {
  if (m_fd != io::InvalidFileDesc) io::close(m_fd);
  m_fd = io::open(path);
  return *this;
}

int fd::release() noexcept {
  int tmp = m_fd;
  m_fd = io::InvalidFileDesc;
  return m_fd;
}
