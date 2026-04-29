#ifndef DVRT_FBUFF_HPP_
#define DVRT_FBUFF_HPP_

#include <filesystem>
#include <memory>

namespace dvrt::__buff {

class fbuff {
 private:
  const std::filesystem::path fpath_;

  const size_t fsize_;

  std::shared_ptr<char[]> bytes_;

 public:
  explicit fbuff(const std::filesystem::path);

  const std::shared_ptr<char[]>& get_bytes() const;

  void read_bytes();

  uint32_t calc_checksum() const;
};

}  // namespace dvrt::__buff

#endif  // DVRT_FBUFF_HPP_
