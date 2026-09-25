#include "dvrt/path.hpp"

#include "io.hpp"

using path    = dvrt::path;
using fd_base = dvrt::detail::fd_base;
using fd      = dvrt::detail::fd;

fd_base::operator bool(this fd_base self) noexcept {
  return self.m_fd != io::detail::InvalidFileDesc;
}

fd_base::operator int(this fd_base self) noexcept {
  return self.m_fd;
}

int fd_base::raw(this fd_base self) noexcept {
  return self.m_fd;
}

fd::fd(char const* t_path) noexcept
: fd_base{ io::detail::open(t_path) } {}

fd::fd(fd const& rhs) noexcept
: fd_base{ io::detail::duplicate_fd(rhs.m_fd) } {}

fd::fd(fd&& rhs) noexcept
: fd_base{ rhs.release() } {}

fd::~fd() {
  if (m_fd != io::detail::InvalidFileDesc) {
    io::detail::close(m_fd);
  }
}

fd& fd::operator=(fd const& rhs) noexcept {
  if (*this && rhs) {
    if (m_fd != rhs.m_fd) {
      m_fd = io::detail::duplicate_fd(m_fd, rhs.m_fd);
    }
  } else if (rhs) {
    m_fd = io::detail::duplicate_fd(rhs.m_fd);
  }
  return *this;
}

fd& fd::operator=(fd&& rhs) noexcept {
  if (m_fd != rhs.m_fd && rhs) {
    if (*this) io::detail::close(m_fd);
    m_fd = rhs.release();
  }
  return *this;
}

fd& fd::operator=(int fd) noexcept {
  if (m_fd != io::detail::InvalidFileDesc) io::detail::close(m_fd);
  m_fd = fd;
  return *this;
}

fd& fd::operator=(char const* path) noexcept {
  if (m_fd != io::detail::InvalidFileDesc) io::detail::close(m_fd);
  m_fd = io::detail::open(path);
  return *this;
}

int fd::release() noexcept {
  int released_fd = m_fd;
  m_fd = io::detail::InvalidFileDesc;
  return released_fd;
}

bool path::is_directory() const noexcept {

}

bool path::is_reg_file() const noexcept {

}

bool path::is_symlink() const noexcept {

}

char const* path::extension() {

}

bool path::has_filename() const noexcept {
  return static_cast<bool>(m_path_fd);
}

path& path::replace_filename(path const&) {

}

path& path::replace_extension(path const&) {

}

auto path::absolute_path() const -> std::unique_ptr<char[], abs_path_deleter> {
  return std::unique_ptr<char[], abs_path_deleter>{ io::abs_path(m_path), abs_path_deleter{} };
}
