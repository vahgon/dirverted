#include "archie/file/buffer.hpp"

#include <fcntl.h>
#include <sys/mman.h>

#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>

#include "archie/file/file.hpp"

namespace buffer = archie::bytes;

uint64_t buffer::read_buffer(archie::file* f) {
  std::filebuf file_stream;
  if (!file_stream.open(f->m_path, std::ios::binary | std::ios::in)) {
    throw std::runtime_error {
      std::format("err calling open for path {} on filebuf: {}",
                  f->m_path.string(), std::strerror(errno))
    };
  } else {
    f->m_buffer = static_cast<std::byte*>(malloc(f->m_size));
    return file_stream.sgetn(reinterpret_cast<char*>(f->m_buffer), f->m_size);
  }
}

[[nodiscard]] void* buffer::memory_mapped_file(archie::file* f) {
  int fd{ open(f->m_path.c_str(), O_RDONLY) };
  return mmap(0, f->m_size, PROT_READ, MAP_SHARED, fd, 0);
}

[[nodiscard]] void* buffer::path_str_to_bytes(file *f) {
  const std::string path_str{ f->m_path.string() };
  const size_t path_size{ path_str.size() };
  auto bytes{ std::malloc(path_size) };

  if (!bytes) {
    return nullptr;
  } else {
    return std::memcpy(bytes, path_str.data(), path_str.size());
  }
}
