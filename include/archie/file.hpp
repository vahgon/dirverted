#ifndef ARCHIE_FILE_HPP_
#define ARCHIE_FILE_HPP_

#include <filesystem>
#include <span>
#include <type_traits>
#include <utility>

#include "core/concepts.hpp"

namespace archie {

inline constexpr std::size_t Zip64Threshold{ 1024uz * 1024uz * 1024uz };
inline constexpr std::size_t StackThreshold{ 1024uz };

class file {
 public:
  file() = default;

  template<archie::concepts::ArchieFileSrcType SrcType>
  explicit file(SrcType&& t_path)
  : m_path{ std::forward<SrcType>(t_path) }
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

  auto operator[](this file& self, std::size_t idx) {
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

  std::span<std::byte const> gen_header(std::size_t const) const;
  std::size_t putsbyte(auto byte) requires (sizeof(decltype(byte)) == 1);

  std::size_t insert_at(void*, std::size_t const);
  std::size_t prepend(void*);
  std::size_t append(void*);

  std::size_t open();
  std::size_t save() const;

  std::size_t size() const noexcept { return m_size; }

  template<typename Self>
  auto path(this Self&& self) {
    if constexpr (std::is_rvalue_reference_v<Self>) {
      return std::remove_cvref_t<decltype(self.m_path)>(std::move(self.m_path));
    } else {
      return std::forward_like<Self>(self.m_path);
    }
  }

 private:
  std::filesystem::path m_path{};
  std::size_t           m_size{};
};

}  // namespace archie

#endif  // ARCHIE_FILE_HPP_
