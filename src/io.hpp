#pragma once

namespace dvrt::detail::io {

inline constexpr int InvalidFileDesc = -1;

[[nodiscard]] int open(char const*) noexcept;

[[nodiscard]] int openat(char const*, int) noexcept;

int close(int) noexcept;

#ifdef __linux__
[[nodiscard]] int duplicate_fd(int) noexcept;

[[nodiscard]] int duplicate_fd(int, int) noexcept;
#endif

#ifdef _WIN32
[[nodiscard]] int open(wchar_t const*, int) noexcept;
#endif

}  // namespace dvrt::detail::io

#ifdef __linux__
# include <dirent.h>
#endif 

namespace dvrt::detail {

#ifdef __linux__
DIR* fdiropen(int) noexcept;
#endif

}  // namespace dvrt::detail

namespace dvrt::detail {

struct fd_base {
 public:
  fd_base() noexcept = default;

  explicit fd_base(int t_fd) noexcept
  : m_fd{ t_fd } {}

  explicit operator bool(this fd_base) noexcept;

  explicit operator int(this fd_base) noexcept;

 public:
  [[nodiscard]] int raw(this fd_base) noexcept;

 protected:
  int m_fd{ io::InvalidFileDesc };
};

struct fd final : public fd_base {
 public:
  fd() noexcept = default;

  explicit fd(char const* path) noexcept
  : fd_base{ io::open(path) } {}

  explicit fd(int t_fd) noexcept
  : fd_base{ t_fd } {}

  fd(fd const& rhs) noexcept
  : fd_base{ io::duplicate_fd(rhs.m_fd) } {}

  fd(fd&& rhs) noexcept
  : fd_base{ rhs.release() } {}

  ~fd() { if (m_fd != io::InvalidFileDesc) io::close(m_fd); };

 public:
  fd& operator=(fd const&) noexcept;

  fd& operator=(fd&&) noexcept;

  fd& operator=(int) noexcept;

  fd& operator=(char const*) noexcept;

 public:
  [[nodiscard]] int release() noexcept;
};

}  // namespace dvrt::detail
