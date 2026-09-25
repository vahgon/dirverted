#include "io.hpp"

#ifdef __linux__
# include <fcntl.h>
# include <syscall.h>
# include <unistd.h>
#elifdef _WIN32
#endif

#include <cstdlib>

namespace io = dvrt::io;

#ifdef __linux__
int io::open(char const* path) noexcept {
  return ::open(path, O_CLOEXEC);
}
# ifdef SYS_openat2
inline constexpr open_how how{
  .flags   = static_cast<decltype(how.flags)>(O_CLOEXEC),
  .resolve = static_cast<decltype(how.resolve)>(RESOLVE_IN_ROOT)
};

int io::openat(char const* path, int dirfd) noexcept {
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

char* io::abs_path(char const* rel_path) noexcept {
  return ::realpath(rel_path, nullptr);
}

int io::duplicate_fd(int fd_doner) noexcept {
  if (int dupe_fd{}; (dupe_fd = ::fcntl(fd_doner, F_DUPFD_CLOEXEC, 0)) != InvalidFileDesc) {
    return dupe_fd;
  } else {
    return InvalidFileDesc;
  }
}

int io::duplicate_fd(int fd_orig, int fd_doner) noexcept {
  if (int dupe_fd{}; (dupe_fd = ::dup3(fd_doner, fd_orig, O_CLOEXEC)) != InvalidFileDesc) {
    return dupe_fd;
  } else {
    return InvalidFileDesc;
  }
}

DIR* dir_open(int) noexcept {

}

#elifdef _WIN32
int io::open(wchar_t const* path, int flags) noexcept;
#endif
