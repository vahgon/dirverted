#pragma once

#ifdef __linux__
# include <fcntl.h>
#endif

#include <array>
#include <concepts>
#include <cstdlib>
#include <memory>
#include <type_traits>

#include <archie/detail/concepts.hpp>
#include <archie/file/flags.hpp>
#include <archie/detail/types.hpp>

namespace archie::io {

inline int open(char const* path, int flags) noexcept {
  return ::open(path, flags);
}

inline int openat(int fd, char const* path, int flags) noexcept {
  return ::openat(fd, path, flags);
}

template<typename T>
  requires concepts::file::path_or_fd<T>
inline int lazy_name_to_handle_at(T in, type::file_handle_t& fh) noexcept {
  int mnt_id{};

  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::name_to_handle_at(AT_FDCWD, in, __builtin_addressof(fh), __builtin_addressof(mnt_id), 0);
  } else {
    return !::name_to_handle_at(in, nullptr, __builtin_addressof(fh), __builtin_addressof(mnt_id), AT_EMPTY_PATH);
  }
}

template<typename T>
  requires concepts::file::path_or_fd<T>
inline int name_to_handle_at(T in, type::file_handle_t& fh, int& mnt_id, int flags) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::name_to_handle_at(AT_FDCWD, in, __builtin_addressof(fh), __builtin_addressof(mnt_id), flags);
  } else {
    return !::name_to_handle_at(in, nullptr, __builtin_addressof(fh), __builtin_addressof(mnt_id), flags);
  }
}

inline int name_to_handle_at(int dirfd, char const* path, type::file_handle_t& fh, int& mnt_id, int flags) noexcept {
  return !::name_to_handle_at(dirfd, path, __builtin_addressof(fh), __builtin_addressof(mnt_id), flags);
}

inline int open_by_handle_at(int mnt_fd, type::file_handle_t& fh, int flags) noexcept {
  return ::open_by_handle_at(mnt_fd, __builtin_addressof(fh), flags);
}

template<typename... Ts>
  requires (sizeof...(Ts) >= 1) && (..., concepts::file::path_or_fd<Ts>)
inline auto multi_name_to_handle_at(int flags, Ts... ins) noexcept(noexcept(type::file_handle_t{})) {
  struct file_handle_inf {
    std::unique_ptr<type::file_handle_t> file_handle;
    int  mount_id;
    bool err;
  };

  auto open_handle = [flags](concepts::file::path_or_fd auto& in) -> file_handle_inf {
    auto info = file_handle_inf{ std::make_unique<type::file_handle_t>(1), 0 , true };
    info.err = !!archie::io::name_to_handle_at(in, *info.file_handle, info.mount_id, flags);
    return info;
  };

  std::array<file_handle_inf, sizeof...(ins)> file_handles{ (...,(open_handle(ins))) };
  return file_handles;
}

}  // namespace archie::io

// __glibc_has_open_how will be defined if __has_include("linux/openat2.h")
#ifdef __glibc_has_open_how

namespace archie::io {

template<typename T>
  requires concepts::file::path_or_fd<T>
inline int openat2(int dfd, char const* filename, const struct open_how& how, size_t usize) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>){
    return ::openat2(dfd, filename, __builtin_addressof(how), usize);
  } else {
    return ::openat2(dfd, filename, __builtin_addressof(how), usize);
  }
}

}  // namespace archie::io

#endif
