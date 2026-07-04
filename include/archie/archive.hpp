#ifndef ARCHIE_ARCHIVE_HPP_
#define ARCHIE_ARCHIVE_HPP_

#include <filesystem>
#include <thread>

namespace archie {

class archive {
 public:
  explicit archive(std::string_view const t_root_path)
  : m_root{ t_root_path } {}

  archive(std::string_view const t_root_path, std::uint32_t t_thread_count)
  : m_root{ t_root_path }, m_usable_threads{ t_thread_count } {}

  void archive_root();

  void delegate_work(std::span<std::filesystem::directory_entry> const&) const;

  void check_file(std::filesystem::directory_entry const) const;

  void iterate_root();

 private:
  const std::string_view m_root{};
  std::uint32_t m_usable_threads{ std::thread::hardware_concurrency() };
};

}  // namespace archie

#endif  // ARCHIE_ARCHIVE_HPP_
