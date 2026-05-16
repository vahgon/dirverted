#include "archie/converter.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>

uint32_t archie::convert::byte_time(const std::filesystem::path& file_path) {
  /* MS-DOS byte-formatted time */
  uint16_t byte_t;
  /* MS-DOS byte-formatted date */
  uint16_t byte_d;

  time_t ctime {
    std::chrono::system_clock::to_time_t(
      std::chrono::clock_cast<std::chrono::system_clock>(
        std::filesystem::last_write_time(file_path)))
  };

  std::tm ltime;

  localtime_r(&ctime, &ltime);

  /* MS-DOS fit the 0-59 range for seconds into 5 bits by halving orig. val */
  byte_t = (ltime.tm_hour << 11) | (ltime.tm_min << 5) | (ltime.tm_sec / 2);

  /**
   *  std::tm.year gives year from 1900. zip format requires from 1980
   *  std::tm.month gives months from 0-11. zip format requires 1-12
   *
   *  AND used with bitmask to ensure year width is 7 bits, month is 4,
   *  and day is 5.
   **/
  uint16_t year  = static_cast<uint16_t>(ltime.tm_year - 80) & 0x7f;
  uint16_t month = static_cast<uint16_t>(ltime.tm_mon + 1)   & 0x0f;
  uint16_t day   = static_cast<uint16_t>(ltime.tm_mday)      & 0x1f;

  byte_d = (year << 9) | (month << 5) | day;

  return (static_cast<uint32_t>(byte_d) << 16) | byte_t;
}

uint32_t archie::convert::mod_time_ext_timestamp(const std::filesystem::path& file_path) {
  auto ctime {
    std::chrono::clock_cast<std::chrono::system_clock>(
      std::filesystem::last_write_time(file_path))
  };

  return static_cast<uint32_t>(
    std::chrono::duration_cast<std::chrono::seconds>(
      ctime.time_since_epoch()).count());
}
