#include "archie/file/file.hpp"

#include <cassert>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <format>
#include <fstream>

#include "archie/file/headers.hpp"

namespace chrono = std::chrono;

std::uint32_t archie::file::mtime() const {
  auto file_t{ chrono::clock_cast<chrono::system_clock>(
    std::filesystem::last_write_time(m_path))
  };

  auto base_t{ chrono::floor<chrono::days>(file_t) };

  chrono::year_month_day date{ base_t };
  chrono::hh_mm_ss time{ chrono::floor<chrono::milliseconds>(file_t - base_t) };

  /* MS-DOS fit the 0-59 range for seconds into 5 bits by halving orig. val */
  std::uint16_t mtime{ static_cast<std::uint16_t>(
    static_cast<int>(time.hours().count()) << 11  |
    static_cast<int>(time.minutes().count()) << 5 |
    static_cast<int>(time.seconds().count()) / 2)
  };

  // TODO(vahgon): bitmask
  // TODO(vahgon): year shouldn't be negative...?
  auto day{ static_cast<unsigned>(date.day()) };
  auto mon{ static_cast<unsigned>(date.month()) };
  auto yea{ static_cast<int>(date.year()) };

  std::uint16_t mdate{ static_cast<std::uint16_t>(
    (static_cast<unsigned>(yea) << 9) | (mon << 5) | day) };

  return static_cast<uint32_t>(mdate << 16) | mtime;
}

std::uint32_t archie::file::mtime_ext() const noexcept {
  return 1;
}

template<>
std::size_t archie::set_headers<true>(std::size_t size) {
  assert(size >= headers::SizeThreshold);
  return size;
}

template<>
std::size_t archie::set_headers<false>(std::size_t size) {
  assert(size < headers::SizeThreshold);
  return size;
}

void archie::deserialize_fs_path(const std::filesystem::path& path) {
  archie::file file{ path };
  std::filebuf io_file{};

  if (io_file.open(path, std::ios::binary | std::ios::in)) {
    auto file_buff{ const_cast<std::byte*>(file.buffer_bytes()) };

    io_file.sgetn(reinterpret_cast<char*>(file_buff), static_cast<long>(22));

    io_file.close();
  } else {
    throw std::runtime_error(std::format("err opening {}", path.string()));
  }
}
