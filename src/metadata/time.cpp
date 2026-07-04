#include "metadata/time.hpp"

#include<cstdint>
#include<filesystem>

std::uint16_t archie::time::mtime(std::filesystem::path const& path) {
  std::chrono::hh_mm_ss const time{ archie::time::time_hhmmss(path) };

  return static_cast<std::uint16_t>(
    archie::time::time_cast<std::int32_t>(time.hours())   << 11 |  // 5 bits
    archie::time::time_cast<std::int32_t>(time.minutes()) << 5  |  // 6 bits
    // 2^5 = 32 - halved to fit in 5 bit limit
    archie::time::time_cast<std::int32_t>(time.seconds())  / 2);   // 5 bits
}

std::uint16_t archie::time::mdate(std::filesystem::path const& path) {
  std::chrono::year_month_day const date{ archie::time::get_time_base(path) };
  auto const day{ archie::time::date_cast<std::uint32_t>(date.day())   & 0x1f };
  auto const mon{ archie::time::date_cast<std::uint32_t>(date.month()) & 0x0f };
  // MSDOS year epoch is 1980
  auto const yea{ (archie::time::date_cast<std::uint32_t>(
    static_cast<std::int32_t>(date.year())) - 1980u) & 0x7f };

  return static_cast<std::uint16_t>((yea << 9) | (mon << 5) | day);
}

std::uint32_t archie::time::msdos(std::filesystem::path const& path) {
  return static_cast<std::uint32_t>(
    archie::time::mdate(path) << 16) | archie::time::mtime(path);
}
