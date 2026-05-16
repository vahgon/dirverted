#include "archie/file/file.hpp"

#include <cstring>
#include <fstream>
#include <stdexcept>

#include "archie/file/crc.hpp"
#include "archie/converter.hpp"
#include "archie/headers/lfh.hpp"

template<typename T>
void* write(void*, T&, std::size_t&) noexcept;

void* create_lfh() noexcept;

void archie::file::get_filebuf() {
  std::filebuf io_file;
  if (!io_file.open(m_path, std::ios::binary | std::ios::in)) {
    throw std::runtime_error("err");
  } else {
    io_file.sgetn(reinterpret_cast<char*>(m_raw_buffer.get()), m_size);
    io_file.close();
  }
}

void archie::file::set_file_attrs() {
  m_crc32 = archie::crc::crc32_lookup(m_raw_buffer.get(), m_size);
  m_ctime = archie::convert::byte_time(m_path);
  m_path_str_bytes = static_cast<uint16_t>(m_path_str.size());

  set_lfh_attrs();
}

void* archie::file::set_lfh_attrs() noexcept {

}

void* create_lfh() noexcept {
  return malloc(headers::lfh::LFH_SIZE);
}

template<typename T>
void* write(void* ptr, T& val, std::size_t& offset) noexcept {
  offset += sizeof(T);
  return std::memcpy(ptr, val, sizeof(T));
}
