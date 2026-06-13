#include "archie/internal/buffer.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace internal = archie::internal;

bool internal::read_file(std::filesystem::path const& path) {
  std::filebuf file;

  if (!file.open(path, std::ios::binary | std::ios::in)) {
    return false;
  }

  for (auto e{ file.in_avail() }; e > 0; --e) {
    std::cout << e << '\n';
  }

  file.close();

  return true;
}

template<typename T>
internal::file_handler<T>& internal::file_handler<T>::operator=(file_handler&& rhs) noexcept {
  if (this != &rhs) {
    m_file = std::move(rhs.m_file);
    m_buf = std::move(rhs.m_buf);
  } else {
    return *this;
  }
}

std::uint32_t internal::mtime(std::filesystem::path const& path) {
  const auto file_time{ std::chrono::clock_cast<std::chrono::system_clock>(
    std::filesystem::last_write_time(path))
  };

  const auto time_base{ std::chrono::floor<std::chrono::days>(file_time) };

  std::chrono::year_month_day date{ time_base };
  std::chrono::hh_mm_ss time{ std::chrono::floor<
    std::chrono::milliseconds>(file_time - time_base) };

  /* MS-DOS fit the 0-59 range for seconds into 5 bits by halving orig. val */
  const std::uint16_t mtime{ static_cast<std::uint16_t>(
    internal::time_to_int<std::int32_t>(time.hours()) << 11  |
    internal::time_to_int<std::int32_t>(time.minutes()) << 5 |
    internal::time_to_int<std::int32_t>(time.seconds()) / 2)
  };

  auto day{ ((std::uint32_t)date.day()) & 0x1f };
  auto mon{ ((std::uint32_t)date.month()) &0x0f };
  /* MS-DOS year epoch is 1980 */
  auto yea{ ((std::uint32_t)(std::int32_t)date.year() - 1980u) & 0x7f };

  std::uint16_t mdate{ static_cast<std::uint16_t>(
    (yea << 9) | (mon << 5) | day)
  };

  /* 16 bit date and times are combined into one 32 bit num */
  return static_cast<std::uint32_t>(mdate << 16) | mtime;
}
