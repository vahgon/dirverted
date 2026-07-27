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

struct fd {
 public:
  fd() noexcept = default;

  explicit fd(char const* path) noexcept
  : m_fd{ io::open(path) } {}

  explicit fd(int t_fd) noexcept
  : m_fd{ t_fd } {}

  fd(fd const& rhs) noexcept
  : m_fd{ io::duplicate_fd(rhs.m_fd) } {}

  fd(fd&& rhs) noexcept
  : m_fd{ rhs.release() } {}

  ~fd() { if (m_fd != io::InvalidFileDesc) io::close(m_fd); };

 public:
  fd& operator=(fd const&) noexcept;

  fd& operator=(fd&&) noexcept;

  fd& operator=(int) noexcept;

  fd& operator=(char const*) noexcept;

  explicit operator bool() const noexcept;

  explicit operator int() const noexcept;

 public:
  [[nodiscard]] int raw() const noexcept { return m_fd; }

  [[nodiscard]] int release() noexcept;

 private:
  int m_fd{ io::InvalidFileDesc };
};

}  // namespace dvrt::detail
