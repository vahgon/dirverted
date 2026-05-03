#ifndef ARCHIE_DIRVERT_HPP_
#define ARCHIE_DIRVERT_HPP_

#include <filesystem>
#include <thread>

namespace archie {

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

}  // namespace archie

#endif  // ARCHIE_DIRVERT_HPP_
