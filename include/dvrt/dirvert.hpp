#ifndef DVRT_DIRVERT_HPP_
#define DVRT_DIRVERT_HPP_

#include <filesystem>
#include <thread>

namespace dvrt {

class dirvert {
 private:
  const std::filesystem::path root_;

  size_t thread_cnt_{ std::thread::hardware_concurrency() };

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
