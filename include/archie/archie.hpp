#ifndef ARCHIE_DIRVERT_HPP_
#define ARCHIE_DIRVERT_HPP_

#include <filesystem>
#include <thread>

namespace archie {

class archive {
 public:
  explicit archive(std::string_view t_root_path)
  : m_root{ t_root_path } {}

  archive(std::string_view t_root_path, uint32_t t_thread_count)
  : m_root{ t_root_path }
  , m_usable_threads{ t_thread_count } {}

  void determine_root_type() const;

 private:
  const std::filesystem::path m_root;
  uint32_t m_usable_threads{ std::thread::hardware_concurrency() };

 private:
  void delegate_work(const std::span<std::filesystem::directory_entry>&) const;

  void check_file(const std::filesystem::directory_entry) const;

  void recursively_iterate_root() const;
};

}  // namespace archie

#endif  // ARCHIE_DIRVERT_HPP_
