#ifndef SRC_METADATA_TIME_HPP_
#define SRC_METADATA_TIME_HPP_

#include <chrono>
#include <concepts>
#include <cstdint>
#include <filesystem>

namespace archie::time {

template<std::integral NumType>
NumType utc_mtime(std::filesystem::file_time_type mtime) {
  return static_cast<NumType>(
    std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::clock_cast<std::chrono::system_clock>(
        mtime).time_since_epoch()).count());
}

template<std::integral NumType>
constexpr NumType time_cast(auto time) {
  return static_cast<NumType>(time.count());
}

template<std::unsigned_integral NumType>
constexpr auto date_cast(auto date) {
  return static_cast<NumType>(date);
}

inline auto file_mtime_clock_cast(std::filesystem::path const& path) {
  return std::chrono::clock_cast<std::chrono::system_clock>(
      std::filesystem::last_write_time(path));
}

inline auto get_time_base(std::filesystem::path const& path) {
  return std::chrono::floor<std::chrono::days>(
    archie::time::file_mtime_clock_cast(path));
}

inline auto time_hhmmss(std::filesystem::path const& path) {

  return std::chrono::floor<std::chrono::milliseconds>(
    archie::time::file_mtime_clock_cast(path) - archie::time::get_time_base(path));
}

constexpr std::uint32_t msdos(std::uint16_t mdate, std::uint16_t mtime) {
  return static_cast<std::uint32_t>(mdate << 16) | mtime;
}

std::uint16_t mtime(std::filesystem::path const&);
std::uint16_t mdate(std::filesystem::path const&);
std::uint32_t msdos(std::filesystem::path const&);

struct msdos {
 public:
  msdos() = default;

  explicit msdos(std::filesystem::path const& path)
  : mtime{ archie::time::mtime(path) }
  , mdate{ archie::time::mdate(path) } {}

  void set_lastmod(this msdos self, std::filesystem::path const& path) {
    self.mtime = archie::time::mtime(path);
    self.mdate = archie::time::mdate(path);
  }

 public:
  std::uint16_t mtime{};
  std::uint16_t mdate{};
};

struct utc {
 public:
  utc() = default;

  explicit utc(std::filesystem::path const& path)
  : mtime{ archie::time::utc_mtime<std::uint32_t>(
    std::filesystem::last_write_time(path)) } {}

  void set_lastmod(this utc self, std::filesystem::path const& path) {
    self.mtime = archie::time::utc_mtime<std::uint32_t>(
      std::filesystem::last_write_time(path));
  }

 public:
  std::uint32_t mtime{};
};

}  // namespace archie::time

#endif  // SRC_METADATA_TIME_HPP_
