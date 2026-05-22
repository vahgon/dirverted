#include "archie/file/file.hpp"

#include <cassert>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <format>
#include <fstream>

#include "archie/file/headers.hpp"

namespace chrono = std::chrono;

std::string_view archie::file::buff_view() const noexcept {
  return std::string_view{ reinterpret_cast<const char*>(m_buff.data()), m_size };
}

void archie::file::deserialize_fs_path() {
  std::filebuf io_file;

  if (!io_file.open(m_path, std::ios::binary | std::ios::in))  {
    throw std::runtime_error(std::format("err opening {}", m_path.c_str()));
  } else {
    io_file.sgetn(reinterpret_cast<char*>(m_buff.data()),
                  static_cast<std::streamsize>(m_size));

    mtime();

    io_file.close();
  }
}

std::uint32_t archie::file::mtime() const {
  auto file_t{ chrono::clock_cast<chrono::system_clock>(
    std::filesystem::last_write_time(m_path))
  };

  auto base_t{ chrono::floor<chrono::days>(file_t) };

  chrono::year_month_day date{ base_t };
  chrono::hh_mm_ss time{ chrono::floor<chrono::milliseconds>(file_t - base_t) };

  /* MS-DOS fit the 0-59 range for seconds into 5 bits by halving orig. val */
  std::uint16_t mtime{ static_cast<std::uint16_t>(
    archie::time_convert<std::int32_t>(time.hours()) << 11  |
    archie::time_convert<std::int32_t>(time.minutes()) << 5 |
    archie::time_convert<std::int32_t>(time.seconds()) / 2)
  };

  auto day{ ((std::uint32_t)date.day()) & 0x1f };
  auto mon{ ((std::uint32_t)date.month()) &0x0f };
  /* MS-DOS year epoch is 1980 */
  auto yea{ ((std::uint32_t)(std::int32_t)date.year() - 1980u) & 0x7f };

  std::uint16_t mdate{ static_cast<std::uint16_t>(
    (yea << 9) | (mon << 5) | day) };

  /* 16 bit date and times are combined into one 32 bit num */
  return static_cast<std::uint32_t>(mdate << 16) | mtime;
}

std::uint32_t archie::file::mtime_ext() const {
  auto mtime{ chrono::clock_cast<chrono::system_clock>(
    std::filesystem::last_write_time(m_path))
  };

  return static_cast<std::uint32_t>(
    chrono::duration_cast<chrono::seconds>(
      mtime.time_since_epoch()).count());
}

template<>
std::size_t archie::set_headers<true>() {
  assert(1 >= headers::SizeThreshold);
  return 1;
}

template<>
std::size_t archie::set_headers<false>() {
  assert(1 < headers::SizeThreshold);
  return 1;
}
