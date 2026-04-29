#ifndef DVRT_DIRVERT_HPP_
#define DVRT_DIRVERT_HPP_

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
           std::vector<std::filesystem::path> > dir_map_;

  std::vector<dvrt::__buff::fbuff> fdata_;

  size_t t_count_{ std::thread::hardware_concurrency() };

 private:
  void delegate_work(const std::span<std::filesystem::directory_entry>&);

  void check_file(const std::filesystem::directory_entry) const;

  void recursively_iterate_root();

 public:
  explicit dirvert(const char*);

  dirvert(const char*, size_t);

  void determine_input();
};

}  // namespace dvrt

#endif  // DVRT_DIRVERT_HPP_
