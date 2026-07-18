#pragma once

#if __linux__
# include <limits.h>
#elif _WIN32
# include <windows.h>
#endif

#include "detail/concepts.hpp"

namespace archie::path {

#if __linux__
inline constexpr int  MaxPathStr     = PATH_MAX;
inline constexpr char path_separator = '/';
#elif _WIN32
inline constexpr int  MaxPathStr     = MAX_PATH;
inline constexpr char path_separator = '/' || '\\';
#endif

template<typename CharT>
  requires concepts::path_char_t<CharT>
constexpr bool is_path_separator(CharT const c) noexcept {
  if constexpr (::concepts::is_char_t<CharT>) {
    return c == path_separator;
  } else if constexpr (::concepts::is_wchar_t<CharT>) {
    return c == L'/' || c == L'\\';
  }
}

template<typename CharT>
  requires concepts::path_char_t<CharT>
constexpr bool is_absolute(CharT const c) noexcept {
  return is_path_separator(c);
}

}  // namespace archie::path

#ifdef __linux__
# include <fcntl.h>
# include <sys/stat.h>
#endif

namespace archie::io {

#if __linux__
inline constexpr int MaxFileStr = NAME_MAX;
#elif _WIN32
inline constexpr int MaxFileStr = NAME_MAX;
#endif

#if (defined _LINUX_STAT_H) || (defined _SYS_STAT_H)
using file_stat  = struct stat;
#endif

inline int archie_open(char const* file, int flags) noexcept {
  return open(file, flags);
}

inline int archie_stat(char const* file, file_stat& p_stat) noexcept {
  return !stat(file, __builtin_addressof(p_stat));
}

inline int archie_fstat(int fd, file_stat& p_stat) noexcept {
  return !fstat(fd, __builtin_addressof(p_stat));
}

inline bool is_sock(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFSOCK;
}

inline bool is_symlink(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFLNK;
}

inline bool is_regular_file(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFREG;
}

inline bool is_block_device(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFBLK;
}

inline bool is_directory(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFDIR;
}

inline bool is_character_device(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFCHR;
}

inline bool is_fifo(file_stat const& fs) noexcept {
  return (fs.st_mode & S_IFMT) == S_IFIFO;
}

}  // namespace archie::io
