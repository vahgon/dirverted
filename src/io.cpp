#include "io.hpp"

#ifdef __linux__
# include <syscall.h>
# include <unistd.h>
#elifdef _WIN32
#endif

#include <fcntl.h>

namespace io = dvrt::detail::io;
using     fd = dvrt::detail::fd;

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

int io::duplicate_fd(int newfd) noexcept {
  if (int duped_fd{}; (duped_fd = ::dup3(io::InvalidFileDesc, newfd, O_CLOEXEC)) != -1) {
    return duped_fd;
  }
  return io::InvalidFileDesc;
}

int io::duplicate_fd(int oldfd, int newfd) noexcept {
  if (int duped_fd{}; (duped_fd = ::dup3(oldfd, newfd, O_CLOEXEC)) != -1) {
    return duped_fd;
  }
  return io::InvalidFileDesc;
}

#elifdef _WIN32
int io::open(wchar_t const* path, int flags) noexcept;
#endif

fd& fd::operator=(fd const& rhs) noexcept {
  if (m_fd != rhs.m_fd && rhs) {
    if (*this) io::close(m_fd);
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

fd::operator bool() const noexcept {
  return m_fd != io::InvalidFileDesc;
}

fd::operator int() const noexcept {
  return m_fd;
}

int fd::release() noexcept {
  int tmp = m_fd;
  m_fd = io::InvalidFileDesc;
  return m_fd;
}
