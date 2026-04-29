#ifndef DVRT_FBUFF_HPP_
#define DVRT_FBUFF_HPP_

#include <filesystem>
#include <memory>

namespace dvrt::__crc {

constexpr uint32_t gen_poly{ 0xedb88320 };
constexpr uint32_t init_crc{ 0xffffffff };

}  // namespace dvrt::__crc

namespace dvrt::__buff {

class fbuff {
 private:
  const std::filesystem::path fpath_;

  const size_t fsize_;

  std::shared_ptr<std::byte[]> bytes_;

 public:
  explicit fbuff(const std::filesystem::path);

  const std::shared_ptr<char[]>& get_bytes() const;

  void read_bytes();

  uint32_t crc32_intrinsic() noexcept;

  uint32_t crc32_lookup_table() noexcept;
};

}  // namespace dvrt::__buff

#endif  // DVRT_FBUFF_HPP_
