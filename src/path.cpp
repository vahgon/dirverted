#include "dvrt/path.hpp"

#include "io.hpp"
#include "stat.hpp"
#include "detail.hpp"

using path    = dvrt::path;
using fd_base = dvrt::detail::fd_base;
using fd      = dvrt::detail::fd;

int fd_base::raw(this fd_base self) noexcept {
  return self.m_fdesc_value;
}

fd_base::operator bool(this fd_base self) noexcept {
  return self.m_fdesc_value != io::InvalidFileDesc;
}

fd_base::operator int(this fd_base self) noexcept {
  return self.m_fdesc_value;
}

fd::fd(char const* t_path) noexcept
: fd_base{ io::open(t_path) } {}

fd::fd(fd const& rhs) noexcept
: fd_base{ io::duplicate_fd(rhs.raw()) } {}

fd::fd(fd&& rhs) noexcept
: fd_base{ rhs.release() } {}

fd& fd::operator=(fd const& rhs) noexcept {
  if (*this && rhs) {
    if (m_fdesc_value != rhs.m_fdesc_value) {
      m_fdesc_value = io::duplicate_fd(m_fdesc_value, rhs.m_fdesc_value);
    }
  } else if (rhs) {
    m_fdesc_value = io::duplicate_fd(rhs.m_fdesc_value);
  }
  return *this;
}

fd& fd::operator=(fd&& rhs) noexcept {
  if (m_fdesc_value != rhs.m_fdesc_value && rhs) {
    if (*this) io::close(m_fdesc_value);
    m_fdesc_value = rhs.release();
  }
  return *this;
}

fd& fd::operator=(int fd) noexcept {
  if (m_fdesc_value != io::InvalidFileDesc) io::close(m_fdesc_value);
  m_fdesc_value = fd;
  return *this;
}

fd& fd::operator=(char const* path) noexcept {
  if (m_fdesc_value != io::InvalidFileDesc) io::close(m_fdesc_value);
  m_fdesc_value = io::open(path);
  return *this;
}

fd::~fd() {
  if (m_fdesc_value != io::InvalidFileDesc) {
    io::close(m_fdesc_value);
  }
}

int fd::release() noexcept {
  int released_fd = m_fdesc_value;
  m_fdesc_value = io::InvalidFileDesc;
  return released_fd;
}

path::path(char const* t_path) noexcept :
  m_path{ t_path },
  m_path_fd{ t_path },
  m_path_stats{ io::set_file_stats(m_path_fd.raw(), flags::statx::Mode) }
{}

path::path(int t_fd) noexcept :
  m_path_fd{ t_fd },
  m_path_stats{ io::set_file_stats(t_fd, flags::statx::Mode) }
{}

auto path::absolute_path() const noexcept -> std::unique_ptr<char[], abs_path_deleter> {
  return std::unique_ptr<char[], abs_path_deleter>{ io::abs_path(m_path), abs_path_deleter{} };
}

bool path::is_directory() const noexcept {
  return m_path_stats >> 28 == flags::file_type::dir;
}

bool path::is_chardevice() const noexcept {
  return m_path_stats >> 28 == flags::file_type::chr;
}

bool path::is_blockdevice() const noexcept {
  return m_path_stats >> 28 == flags::file_type::blk;
}

bool path::is_regfile() const noexcept {
  return m_path_stats >> 28 == flags::file_type::reg;
}

bool path::is_fifo() const noexcept {
  return m_path_stats >> 28 == flags::file_type::fifo;
}

bool path::is_symlink() const noexcept {
  return m_path_stats >> 28 == flags::file_type::link;
}

bool path::is_sock() const noexcept {
  return m_path_stats >> 28 == flags::file_type::sock;
}
