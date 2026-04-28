#ifndef DVRT_DIRVERT_H_
#define DVRT_DIRVERT_H_

#include <filesystem>
#include <map>
#include <thread>
#include <vector>

#include "dvrt/fbuff.hpp"

namespace dvrt {

class dirvert {
 private:

  const std::filesystem::path root_;

  std::map<std::filesystem::directory_entry,
           std::vector<std::filesystem::path> > spanned_root_;

  std::vector<dvrt::__buff::fbuff> fdata_;

  unsigned int t_count_{ std::thread::hardware_concurrency() };

  size_t thread_cnt_;

 public:
  explicit dirvert(const char*);

  dirvert(const char*, size_t);

  void recursively_iterate_root();

  void calc_checksum() const;

 private:
  void delegate_work(std::vector<std::filesystem::directory_entry>&);

  void check_file(const std::filesystem::directory_entry) const;
};

}  // namespace dvrt

#endif  // DVRT_DIRVERT_H_
