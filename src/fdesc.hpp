#ifndef SRC_FDESC_HPP_
#define SRC_FDESC_HPP_

#include <algorithm>
#include <array>
#include <cstdio>
#include <memory>
#include <type_traits>
#include <span>
#include <utility>

#include "detail.hpp"

namespace archie {

class fdesc {
  using fclose_t = decltype(&std::fclose);

 public:
  fdesc() = delete;

  explicit fdesc(int);

  template<std::size_t N>
  explicit fdesc(char const* p)
  : file{ std::fopen(p, "rb"), std::fclose } {}

  fdesc(fdesc&&) noexcept = default;
  fdesc(fdesc const&) = delete;

  fdesc& operator=(fdesc&&) noexcept = default;
  fdesc& operator=(fdesc const&) = delete;

  constexpr operator std::string_view() const;

  std::size_t set_size();
  std::size_t get_size() const noexcept { return size; };

  [[nodiscard]] std::FILE* get() noexcept { return file.get(); }

 private:
  std::unique_ptr<std::FILE, fclose_t> file{ nullptr, std::fclose };
  std::size_t                          size{};
};

template<detail::byte_sized T = std::byte, std::size_t N = 0>
struct file_descriptor {
  using byte_type = T;

 public:
  file_descriptor() = default;

  explicit constexpr file_descriptor(T const (&path)[N])
  : path_buffer{ std::to_array(path) } {}

  explicit constexpr file_descriptor(std::array<T, N> const& path)
  : path_buffer{ path } {}

  constexpr bool contains(byte_type b) const {
    return std::ranges::contains(path_buffer, b);
  }

  constexpr std::size_t size() const {
    return N;
  }

  constexpr auto data() {
    return path_buffer.data();
  }

 public:
  std::array<byte_type, N> path_buffer{};
};

}  // namespace archie

#endif  // SRC_FDESC_HPP_
