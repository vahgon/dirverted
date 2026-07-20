#pragma once

#include <fcntl.h>
#include <sys/stat.h>

#include <concepts>
#include <cstdint>
#include <type_traits>

#include <archie/detail/concepts.hpp>
#include <archie/detail/types.hpp>

namespace archie::stat {

#ifdef __linux__

#ifdef STATX_TYPE
template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int stat(T in, int flags, uint32_t mask, StatType& st) noexcept {
  if constexpr (std::same_as<T, char const*>) {
    return !::statx(AT_FDCWD, in, flags, mask, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, mask, __builtin_addressof(st));
  }
}

template<typename StatType>
  requires file::concepts::is_statx<StatType>
inline int stat(int fd, char const* path, int flags, uint32_t mask, StatType& st) noexcept {
  return ::statx(fd, path, flags, mask, __builtin_addressof(st));
}
#endif

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::stat_supported<StatType>
inline int stat(T in, StatType& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
      return !::statx(AT_FDCWD, in, 0, STATX_BASIC_STATS, __builtin_addressof(st));
    } else {
      return !::statx(in, nullptr, AT_EMPTY_PATH, STATX_BASIC_STATS, __builtin_addressof(st));
    }
  } else if constexpr (file::concepts::is_stat<StatType>) {
    if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
      return !::stat(in, __builtin_addressof(st));
    } else {
      return !::fstat(in, __builtin_addressof(st));
    }
  }
}

#ifdef STATX_TYPE
// TODO: getters
#endif

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_sock(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFSOCK;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFSOCK;
  }
}

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_symlink(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFLNK;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFLNK;
  }
}

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_regular_file(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFREG;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFREG;
  }
}

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_block_device(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFBLK;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFBLK;
  }
}

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_directory(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFDIR;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFDIR;
  }
}

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_character_device(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFCHR;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFCHR;
  }
}

template<typename StatType>
  requires file::concepts::stat_supported<StatType>
inline bool is_fifo(StatType const& st) noexcept {
  if constexpr (file::concepts::is_statx<StatType>) {
    return (st.stx_mode & S_IFMT) == S_IFIFO;
  } else if constexpr (file::concepts::is_stat<StatType>) {
    return (st.st_mode & S_IFMT) == S_IFIFO;
  }
}

#endif

}  // namespace archie::stat
