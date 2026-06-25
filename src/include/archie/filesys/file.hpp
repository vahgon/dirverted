#ifndef ARCHIE_FILESYS_FILE_HPP_
#define ARCHIE_FILESYS_FILE_HPP_

#include <filesystem>
#include <span>
#include <type_traits>
#include <utility>

#include "archie/concepts.hpp"

namespace archie {

inline constexpr std::size_t Zip64Threshold{ 1024uz * 1024uz * 1024uz };
inline constexpr std::size_t StackThreshold{ 1024uz };

template<typename T>
using deduced_path_t = std::conditional_t<
  std::is_rvalue_reference_v<T>,
  decltype(std::declval<T>().m_path),
  decltype(std::forward_like<T>(std::declval<T&>().m_path))>;

class file {
 public:
  file() = default;

  explicit file(concepts::ArchieFileSrcType auto&& t_path)
  : m_path{ std::forward<decltype(t_path)>(t_path) }
  , m_size{ std::filesystem::file_size(m_path) } {}

  file(file const&) = delete;

  file(file&&) noexcept = default;

  file& operator=(file const&) = delete;

  file& operator=(file&&) = default;

  template<typename Self>
  auto&& operator=(this Self&& self, auto&& rhs)
    requires(!std::is_const_v<std::remove_reference_t<Self> >) &&
              concepts::ArchieFileSrcType<Self> {
    if (self.m_path != rhs) {
      self.m_path = std::forward_like<Self>(rhs);
    }
    return self;
  }

  ~file() = default;

  auto operator[](this file&& self, std::size_t&& idx) -> decltype(idx) {
    (void)self;
    return idx;
  }

  bool operator==(concepts::FileComparisonType auto&& rhs) noexcept {
    if constexpr (requires { rhs.m_size; }) {
      return m_size == rhs.m_size;
    } else {
      return m_size == rhs;
    }
  }

  bool operator!=(concepts::FileComparisonType auto&& rhs) noexcept {
    if constexpr (requires { rhs.m_size; }) {
      return !(m_size == rhs.m_size);
    } else {
      return !(m_size == rhs);
    }
  }

  bool operator<(concepts::FileComparisonType auto&& rhs) noexcept {
    if constexpr (requires { rhs.m_size; }) {
      return m_size < rhs.m_size;
    } else {
      return m_size < rhs;
    }
  }

  bool operator>(concepts::FileComparisonType auto&& rhs) noexcept {
    if constexpr (requires { rhs.m_size; }) {
      return !(m_size < rhs.m_size);
    } else {
      return !(m_size < rhs);
    }
  }

  bool operator<=(concepts::FileComparisonType auto&& rhs) noexcept {
    if constexpr (requires { rhs.m_size; }) {
      return m_size <= rhs.m_size;
    } else {
      return m_size <= rhs;
    }
  }

  bool operator>=(concepts::FileComparisonType auto&& rhs) noexcept {
    if constexpr (requires { rhs.m_size; }) {
      return !(m_size <= rhs.m_size);
    } else {
      return !(m_size <= rhs);
    }
  }

  friend std::ostream& operator<<(std::ostream&, file const&);

  std::span<std::byte const> gen_header(int const) const;

  template<typename ByteType>
  std::size_t putsbyte(ByteType byte) requires (sizeof(ByteType) == 1);

  std::size_t insert_at(void*, std::size_t const);
  std::size_t prepend(void*);
  std::size_t append(void*);

  std::size_t open();
  std::size_t save() const;

  template<typename Self>
  auto path(this Self&& self) -> deduced_path_t<Self> {
    if constexpr (std::is_rvalue_reference_v<decltype(self)>) {
      return std::remove_cvref_t<decltype(self.m_path)>(std::move(self.m_path));
    } else {
      return std::forward_like<Self>(self.m_path);
    }
  }

  std::size_t size() const noexcept { return m_size; }
  std::uint32_t crc32() const noexcept;

 private:
  std::filesystem::path             m_path{};
  std::size_t                       m_size{};
};

}  // namespace archie

#endif  // ARCHIE_FILESYS_FILE_HPP_
