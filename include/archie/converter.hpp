#ifndef ARCHIE_CONVERTER_HPP_
#define ARCHIE_CONVERTER_HPP_

#include <cstdint>
#include <filesystem>
#include <string>

namespace archie::convert {

uint32_t byte_time(const std::filesystem::path&);

[[nodiscard]] void* path_bytes(const std::filesystem::path&);

uint16_t path_size_bytes(const std::string&);

uint32_t mod_time_ext_timestamp(const std::filesystem::path&);

}  // namespace archie::convert

#endif  // ARCHIE_CONVERTER_HPP_
