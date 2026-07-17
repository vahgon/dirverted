#ifndef ARCHIE_DETAIL_FSCONFIG_HPP_
#define ARCHIE_DETAIL_FSCONFIG_HPP_

#if __linux__
#include <linux/limits.h>
#elif _WIN32
#include <windows.h>
#endif

#include "detail/concepts.hpp"

namespace archie::path {

#if __linux__
inline constexpr char path_separator = '/';
#elif _WIN32
inline constexpr char path_separator = '/' || '\\';
#endif

template<typename CharT>
  requires concepts::path_char_t<CharT>
inline constexpr bool is_path_separator(CharT const c) noexcept {
  if constexpr (::concepts::is_char_t<CharT>) {
    return c == path_separator;
  } else if constexpr (::concepts::is_wchar_t<CharT>) {
    return c == L'/' || c == L'\\';
  }
}

template<typename CharT>
  requires concepts::path_char_t<CharT>
inline constexpr bool is_absolute(CharT const c) noexcept {
  return is_path_separator(c);
}

#if __linux__
inline constexpr int MaxPathStr = PATH_MAX;
#elif _WIN32
inline constexpr int MaxPathStr = MAX_PATH;
#endif

}  // namespace archie::path

#ifdef __linux__
#include <fcntl.h>
#include <sys/stat.h>
#endif

namespace archie::file {

#if __linux__
inline constexpr int MaxFileStr = NAME_MAX;
#elif _WIN32
inline constexpr int MaxFileStr = NAME_MAX;
#endif

#if defined _LINUX_STAT_H && defined STATX__RESERVED
using file_stat  = struct stat;
using file_statx = struct statx;
#elif _LINUX_STAT_H
using file_stat = struct stact;
#endif

inline int archie_open(char const* file, int flags) noexcept {
#if __linux__
  return open(file, flags);
#else
  static_assert(false, "no archie_open(...) implementation for given platform");
#endif
}

inline int archie_stat(char const* file, file_stat* p_stat) noexcept {
#if __linux__
  return !stat(file, p_stat);
#else
  static_assert(false, "no archie_stat(...) implementation for given platform");
#endif
}

inline int archie_fstat(int fd, file_stat& p_stat) noexcept {
#if __linux__
  return !fstat(fd, &p_stat);
#else
  static_assert(false, "no archie_fstat(...) implementation for given platform");
#endif
}

inline bool is_sock(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFSOCK;
#else
  static_assert(false, "no is_sock(...) implementation for given platform");
#endif
}

inline bool is_symlink(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFLNK;
#else
  static_assert(false, "no is_symlink(...) implementation for given platform");
#endif
}

inline bool is_regular_file(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFREG;
#else
  static_assert(false, "no is_regular_file(...) implementation for given platform");
#endif
}

inline bool is_block_device(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFBLK;
#else
  static_assert(false, "no is_block_device(...) implementation for given platform");
#endif
}

inline bool is_directory(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFDIR;
#else
  static_assert(false, "no is_directory(...) implementation for given platform");
#endif
}

inline bool is_character_device(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFCHR;
#else
  static_assert(false, no is_character_device(...) implementation for given platform");
#endif
}

inline bool is_fifo(file_stat const& fs) noexcept {
#if __linux__
  return (fs.st_mode & S_IFMT) == S_IFIFO;
#else
  static_assert(false, no is_fifo(...) implementation for given platform");
#endif
}

}  // namespace archie::file

#endif  // ARCHIE_DETAIL_FSCONFIG_HPP_
