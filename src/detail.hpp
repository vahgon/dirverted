#pragma once

#include <concepts>
#include <cstdint>

#ifdef __linux__
// forward declarations for stat & statx structs
struct stat;
struct statx;
#endif

namespace dvrt::constants {

#ifdef __linux__
inline constexpr int  MaxPathStr    = 4096;
inline constexpr int  MaxFileStr    = 255;
inline constexpr char PathSeparator = '/';

inline constexpr bool
# if __has_include(<bits/types/struct_statx.h>)
StatxSupport = true;
# else
StatxSupport = false;
# endif
#elifdef _WIN32
inline constexpr int  MaxPathStr    = 260;
inline constexpr int  MaxFileStr    = 255;
inline constexpr char PathSeparator = L'\\';
#endif

}  // namespace dvrt::constants

namespace dvrt::type {

#ifdef __linux__
using stat_t = std::conditional_t<constants::StatxSupport, ::statx, ::stat>;
#elifdef _WIN32
#endif

}  // namespace dvrt::type

namespace dvrt::concepts {

template<typename T>
concept IsStat = std::same_as<std::remove_cvref_t<T>, type::stat_t>;

template<typename T>
concept IsChar =
  std::same_as<std::remove_cvref_t<T>, char*> || std::same_as<std::remove_cvref_t<T>, char const*>;

template<typename T>
concept IsPath = requires (T str) {
  { str.c_str() } -> std::same_as<char const*>;
} || IsChar<T>;

template<typename T>
concept IsFileDescriptor =
  std::same_as<std::remove_cvref_t<T>, int>;

template<typename T>
concept IsPathFd = IsPath<T> || IsFileDescriptor<T>;

}  // namespace dvrt::concepts

#ifdef __linux__

namespace dvrt::flags {

inline constexpr int AtCurrWorkingDir  = -100;
inline constexpr int AtEmptyPath       = 0x1000;
inline constexpr int AtNoAutomount     = 0x800;
inline constexpr int AtSymlinkNoFollow = 0x100;
inline constexpr int AtSyncAsStat      = 0;
inline constexpr int AtForceSync       = 0x2000;
inline constexpr int AtDontSync        = 0x4000;

}  // namespace dvrt::flags

namespace dvrt::flags::open {

inline constexpr int Append      = 0x400;
inline constexpr int Async       = 0x2000;
inline constexpr int CloseOnExec = 0x80000;
inline constexpr int Create      = 0x40;
inline constexpr int Direct      = 0x4000;
inline constexpr int Directory   = 0x10000;
inline constexpr int DataSync    = 0x1000;
inline constexpr int LargeFile   = 0;
inline constexpr int NoATime     = 0x40000;
inline constexpr int NoCTTY      = 0x100;
inline constexpr int NoFollow    = 0x20000;
inline constexpr int NonBlock    = 0x800;
inline constexpr int NDelay      = 0x800;
inline constexpr int Path        = 0x200000;
inline constexpr int Sync        = 0x101000;
inline constexpr int TmpFile     = 0x410000;

}  // namespace dvrt::flags::open

namespace dvrt::flags::statx {

inline constexpr uint32_t Type           = 1;
inline constexpr uint32_t Mode           = 0x00000002U;
inline constexpr uint32_t NLink          = 0x00000004U;
inline constexpr uint32_t Uid            = 0x00000008U;
inline constexpr uint32_t Gid            = 0x00000010U;
inline constexpr uint32_t ATime          = 0x00000020U;
inline constexpr uint32_t MTime          = 0x00000040U;
inline constexpr uint32_t CTime          = 0x00000080U;
inline constexpr uint32_t Inode          = 0x00000100U;
inline constexpr uint32_t Size           = 0x00000200U;
inline constexpr uint32_t Blocks         = 0x00000400U;
inline constexpr uint32_t BasicStats     = 0x000007ffU;
inline constexpr uint32_t BTime          = 0x00000800U;
inline constexpr uint32_t All            = (BasicStats | BTime);
inline constexpr uint32_t MntID          = 0x00001000U;
inline constexpr uint32_t DioAlign       = 0x00002000U;
inline constexpr uint32_t MntIDUnique    = 0x00004000U;
inline constexpr uint32_t Subvol         = 0x00008000U;
inline constexpr uint32_t WriteAtomic    = 0x00010000U;
inline constexpr uint32_t DioReadAligned = 0x00020000U;

}  // namespace dvrt::flags::statx_mask


namespace dvrt::flags::file_type {

inline constexpr uint8_t dir  = 0b0100;
inline constexpr uint8_t chr  = 0b0010;
inline constexpr uint8_t blk  = 0b0110;
inline constexpr uint8_t reg  = 0b1000;
inline constexpr uint8_t fifo = 0b0001;
inline constexpr uint8_t link = 0b1010;
inline constexpr uint8_t sock = 0b1100;

}  // namespace dvrt::flags::file_type

#endif
