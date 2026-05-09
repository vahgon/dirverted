#ifndef _ARCHIE_FILE_BUFFER_HPP_
#define _ARCHIE_FILE_BUFFER_HPP_

#include <cstdint>

namespace archie { class file; }

namespace archie::bytes {

[[nodiscard]] void* path_str_to_bytes(file* f);

uint64_t read_buffer(file* f);

[[nodiscard]] void* memory_mapped_file(file* f);

}  // namespace archie::bytes

#endif  // _ARCHIE_FILE_BUFFER_HPP_
