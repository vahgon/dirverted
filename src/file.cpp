#include "archie/file/file.hpp"

#include <filesystem>
#include <stdexcept>

#include "archie/file/buffer.hpp"
#include "archie/headers/lfh.hpp"

using path = std::filesystem::path;

void archie::file::set_file_info() {
  m_size = std::filesystem::file_size(m_path);
  archie::bytes::read_buffer(this);
}

void archie::file::determine_zip_filetype() {
  if (m_size / headers::ZIP_THRESHOLD >= 4) {
    throw std::runtime_error("Archie cannot handle 4GiB> files");
  }
}
