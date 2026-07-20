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
template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_stx_type(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_TYPE, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_TYPE, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_stx_mode(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_MODE, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_MODE, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_stx_nlink(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_NLINK, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_NLINK, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_uid(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_UID, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_UID, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_gid(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_GID, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_GID, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_atime(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_ATIME, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_ATIME, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_mtime(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_MTIME, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_MTIME, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_ctime(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_CTIME, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_CTIME, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_ino(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_INO, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_INO, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_size(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_SIZE, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_SIZE, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_blocks(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_BLOCKS, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_BLOCKS, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_basic_stats(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_BASIC_STATS, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_BASIC_STATS, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_btime(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_BTIME, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_BTIME, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_all(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, (STATX_BASIC_STATS | STATX_BTIME), __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, (STATX_BASIC_STATS | STATX_BTIME), __builtin_addressof(st));
  }
}

template<typename T, typename StatType >
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_mnt_id(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_MNT_ID, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_MNT_ID, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_mnt_id_unique(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_MNT_ID_UNIQUE, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_MNT_ID_UNIQUE, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_subvol(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_SUBVOL, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_SUBVOL, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_write_atomic(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_WRITE_ATOMIC, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_WRITE_ATOMIC, __builtin_addressof(st));
  }
}

template<typename T, typename StatType>
  requires file::concepts::path_or_fd<T> && file::concepts::is_statx<StatType>
inline int set_statx_dio_read_align(T in, int flags, StatType& st) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::statx(AT_FDCWD, in, flags, STATX_DIO_READ_ALIGN, __builtin_addressof(st));
  } else {
    return !::statx(in, nullptr, flags, STATX_DIO_READ_ALIGN, __builtin_addressof(st));
  }
}
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
