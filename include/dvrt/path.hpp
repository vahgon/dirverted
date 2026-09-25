#pragma once

#include <memory>

namespace dvrt::detail {

class fd_base {
 protected:
  fd_base() noexcept = default;

  explicit fd_base(int t_fd) noexcept
  : m_fd{ t_fd } {}

 public:
  [[nodiscard]] int raw(this fd_base)  noexcept;

  explicit operator bool(this fd_base) noexcept;
  explicit operator  int(this fd_base) noexcept;

 protected:
  int m_fd{ -1 };  // (-1 == invalid fdesc)
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

 protected:
  [[nodiscard]] int release() noexcept;
};

}  // namespace dvrt::detail

namespace dvrt {

class path {
  struct abs_path_deleter {
    static void operator()(void* ptr) {
      std::free(ptr); };
  };

 public:
  path() = default;

  explicit path(char const* t_path)
  : m_path{ t_path }, m_path_fd{ t_path } {}

  explicit path(int t_file_descriptor)
  : m_path_fd{ t_file_descriptor } {}

  path(path&&)      = default;
  path(path const&) = default;

  ~path() = default;

  path& operator=(path const&) = default;
  path& operator=(path&&)      = default;

 public:
  explicit operator bool() const noexcept { return static_cast<bool>(m_path_fd); }
  explicit operator int()  const noexcept { return m_path_fd.raw(); }

 public:
  [[nodiscard]] bool is_directory() const noexcept;
  [[nodiscard]] bool is_reg_file()  const noexcept;
  [[nodiscard]] bool is_symlink()   const noexcept;
  [[nodiscard]] bool is_absolute()  const noexcept;
  [[nodiscard]] bool is_relative()  const noexcept;
  [[nodiscard]] bool is_open()      const noexcept;

 public:
  [[nodiscard]] char const* extension();

 public:
  path& append(char const*);
  path& replace_filename(path const&);
  path& replace_extension(path const&);

  auto absolute_path() const -> std::unique_ptr<char[], abs_path_deleter>;

 private:
  char const* m_path{ nullptr };
  detail::fd  m_path_fd{ -1 };
};

}  // namespace dvrt
