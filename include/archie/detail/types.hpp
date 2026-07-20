#pragma once

#ifdef __linux__
# include <sys/stat.h>
#endif

namespace archie::type {

struct stat_t;

#ifdef __linux__
# ifdef STATX_TYPE
struct stat_t final : statx {
  static constexpr bool is_statx = true;
};
# elifndef _SYS_STAT_H
struct stat_t final : stat {
  static constexpr bool is_stat = true;
};
# else
struct stat_t {};
#   if __GNUC__
#     pragma GCC error "Could not find 'struct stat' or 'struct statx' in sys/stat.h"
#   endif
# endif
#endif

enum class fd_t : int {};

} // namespace archie::type
