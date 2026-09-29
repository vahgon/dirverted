#pragma once

#include <cstdint>
#include <memory>

namespace dvrt::detail {

class fd_base {
 protected:
  fd_base() noexcept = default;

  explicit fd_base(int t_fd) noexcept
  : m_fdesc_value{ t_fd } {}

 public:
  [[nodiscard]] int raw(this fd_base)  noexcept;

  explicit operator bool(this fd_base) noexcept;
  explicit operator  int(this fd_base) noexcept;

 protected:
  int m_fdesc_value{ -1 };  // (-1 == invalid fdesc)
};

class fd final : public fd_base {
 public:
  fd() noexcept = default;

  explicit fd(int t_fd) noexcept
  : fd_base{ t_fd } {}

  explicit fd(char const*) noexcept;

  fd(fd const&) noexcept;
  fd(fd&&)      noexcept;

  ~fd();

 public:
  fd& operator=(fd const&)    noexcept;
  fd& operator=(fd&&)         noexcept;

  fd& operator=(char const*)  noexcept;
  fd& operator=(int)          noexcept;

 public:
  [[nodiscard]] bool is_regfile()   const noexcept;
  [[nodiscard]] bool is_directory() const noexcept;
  [[nodiscard]] bool is_symlink()   const noexcept;
  [[nodiscard]] bool is_absolute()  const noexcept;
  [[nodiscard]] bool is_relative()  const noexcept;
  [[nodiscard]] bool is_fifo()      const noexcept;
  [[nodiscard]] bool is_open()      const noexcept;

 protected:
   [[nodiscard]] int release()    noexcept;
   [[nodiscard]] int set_fstats() noexcept;
};

}  // namespace dvrt::detail

namespace dvrt {

class path {
  struct abs_path_deleter {
    static void operator()(void* ptr) noexcept {
      std::free(ptr); };
  };

 public:
  path() = default;

  explicit path(char const*) noexcept;
  explicit path(int)         noexcept;

  path(path&&)      = default;
  path(path const&) = default;

  ~path() = default;

  path& operator=(path const&) = default;
  path& operator=(path&&)      = default;

 public:
  explicit operator bool() const noexcept { return static_cast<bool>(m_path_fd); }
  explicit operator int()  const noexcept { return m_path_fd.raw(); }

 public:
  path& append(char const*);
  path& replace_filename(path const&);
  path& replace_extension(path const&);

 public:
  [[nodiscard]] auto absolute_path()  const noexcept -> std::unique_ptr<char[], abs_path_deleter>;
  [[nodiscard]] bool is_absolute()    const noexcept;
  [[nodiscard]] bool is_relative()    const noexcept { return !is_absolute(); }
  [[nodiscard]] bool is_directory()   const noexcept;
  [[nodiscard]] bool is_chardevice()  const noexcept;
  [[nodiscard]] bool is_blockdevice() const noexcept;
  [[nodiscard]] bool is_regfile()     const noexcept;
  [[nodiscard]] bool is_fifo()        const noexcept;
  [[nodiscard]] bool is_symlink()     const noexcept;
  [[nodiscard]] bool is_sock()        const noexcept;

 private:
  char const* m_path{ nullptr };
  detail::fd  m_path_fd{ -1 };
  uint32_t    m_path_stats{};
};

}  // namespace dvrt
