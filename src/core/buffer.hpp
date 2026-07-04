#ifndef SRC_CORE_BUFFER_HPP_
#define SRC_CORE_BUFFER_HPP_

#include <array>
#include <filesystem>
#include <fstream>
#include <utility>

namespace archie::io {

class buffer {
 public:
  buffer() = default;

  buffer(buffer const&) = delete;
  buffer(buffer&&) noexcept = default;

  ~buffer() = default;

  buffer& operator=(buffer const&) = delete;
  buffer& operator=(buffer&&) noexcept = default;

  template<typename Self>
  auto&& operator[](this Self&& self, std::size_t idx) {
    return std::forward_like<Self>(self.buffer[idx]);
  }

  bool fill_buff(std::uint8_t);

  template<typename... Args>
  std::size_t insert_into_buffer(auto data1, Args... data) {
    if (sizeof...(data) == 0) {}
  }

  void* open(std::filesystem::path const&);
  std::size_t save();

 private:
  std::filebuf                m_file{};
  std::array<std::byte, 1024> m_buff{};
  std::size_t                 m_blocks{};
};

}  // namespace archie::io

#endif  // SRC_CORE_BUFFER_HPP_
