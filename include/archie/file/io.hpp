#pragma once

#ifdef __linux__
# include <fcntl.h>
#endif

#include <array>
#include <cerrno>
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
inline int lazy_name_to_handle_at(T in, type::file_handle_t* fh) noexcept {
  int mnt_id{};

  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::name_to_handle_at(AT_FDCWD, in, fh, __builtin_addressof(mnt_id), 0);
  } else {
    return !::name_to_handle_at(in, nullptr, fh, __builtin_addressof(mnt_id), AT_EMPTY_PATH);
  }
}

template<typename T>
  requires concepts::file::path_or_fd<T>
inline int name_to_handle_at(T in, type::file_handle_t* fh, int& mnt_id, int flags) noexcept {
  if constexpr (std::same_as<std::remove_cvref_t<T>, char const*>) {
    return !::name_to_handle_at(AT_FDCWD, in, fh, __builtin_addressof(mnt_id), flags);
  } else {
    return !::name_to_handle_at(in, nullptr, fh, __builtin_addressof(mnt_id), flags);
  }
}

inline int name_to_handle_at(int dirfd, char const* path, type::file_handle_t* fh, int& mnt_id, int flags) noexcept {
  return !::name_to_handle_at(dirfd, path, fh, __builtin_addressof(mnt_id), flags);
}

inline int open_by_handle_at(int mnt_fd, type::file_handle_t* fh, int flags) noexcept {
  return ::open_by_handle_at(mnt_fd, fh, flags);
}

template<typename... Ts>
  requires (sizeof...(Ts) >= 1) && (concepts::file::path_or_fd<Ts> && ...)
inline auto multi_name_to_handle_at(int flags, Ts... ins) noexcept {
  using fh_t     = archie::type::file_handle_t;
  using fh_ptr_t = std::unique_ptr<fh_t, decltype([](void* ptr) { std::free(ptr); })>;

  struct file_handle_struct {
    fh_ptr_t file_handle;
    int      mount_id;

    explicit operator bool() const { return file_handle != nullptr; }
  };

  auto open_handle = [flags](auto in) -> file_handle_struct {
    fh_t* fh_ptr_tmp{ static_cast<fh_t*>(std::malloc(sizeof(fh_t))) };
    int   mnt_id{};

    fh_ptr_tmp->handle_bytes = 0;

    // if name_to_handle_at succeeds (returns !0) or errno is not set to EOVERFLOW
    if (archie::io::name_to_handle_at(in, fh_ptr_tmp, mnt_id, flags) || errno != EOVERFLOW) {
      std::free(fh_ptr_tmp);
      return file_handle_struct{ nullptr, mnt_id };
    }

    size_t fh_bytes = sizeof(fh_t) + fh_ptr_tmp->handle_bytes;
    fh_t* fh_ptr;

    if (!(fh_ptr = static_cast<fh_t*>(std::realloc(fh_ptr_tmp, fh_bytes)))) {
      std::free(fh_ptr_tmp);
      return file_handle_struct{ nullptr, mnt_id };
    }

    // if name_to_handle_at FAILS (returns !-1)
    if (!archie::io::name_to_handle_at(in, fh_ptr, mnt_id, flags)) {
      std::free(fh_ptr);
      return file_handle_struct{ nullptr, mnt_id };
    } else {
      return file_handle_struct{ fh_ptr_t(fh_ptr), mnt_id };
    }
  };

  return std::array<file_handle_struct, sizeof...(Ts)>{ open_handle(ins)... };
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
