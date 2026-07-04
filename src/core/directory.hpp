#ifndef SRC_CORE_DIRECTORY_HPP_
#define SRC_CORE_DIRECTORY_HPP_

#include <filesystem>

namespace archie {

class directory {
 public:
  directory() = default;

  explicit directory(std::filesystem::path const t_path)
  : m_path{ t_path } {}

 private:
  std::filesystem::path m_path{};
  std::filesystem::path m_zip_path{};
};

}  // namespace archie

#endif  // SRC_CORE_DIRECTORY_HPP_
