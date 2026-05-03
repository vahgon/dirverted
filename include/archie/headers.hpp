#ifndef ARCHIE_HEADERS_HPP_
#define ARCHIE_HEADERS_HPP_

#include <cstdint>
#include <filesystem>

namespace archie::__header {

void eocd_locator();

void ext_info_field_zip64(size_t&, void*);

void local_file_header(const std::filesystem::path&, uint32_t&);

void cdfh();

void eocd();

void eocd_zip64();

}  // namespace archie::__header

namespace archie::__constants {

static constexpr uint16_t VERSION_NEEDED  { 0x2d00 };

static constexpr uint16_t GEN_PURPOSE_FLG { 0x0000 };

static constexpr uint16_t COMPR_METHOD    { 0x0000 };

}  // namespace archie::__constants

namespace archie::__constants::magic_bytes {

/* local file header */
static constexpr uint32_t LFH       { 0x504b0304 };

/* end of central directory locator */
static constexpr uint32_t EOCD_LOC  { 0x504b0607 };

/* central directory file header*/
static constexpr uint32_t CDFH      { 0x504b0102 };

/* end of central directory file header */
static constexpr uint32_t EOCDR     { 0x504b0606 };

}  // namespace archie::__constants::magic_bytes

namespace archie::__constants::zip_64 {

/* the zip64 signature in extra field of LFH */
static constexpr uint16_t SIGNATURE_ID      { 0x0001 };

/* minimum version needed for zip64 file format extensions */
static constexpr uint32_t VERSION_NEEDED    { 0x2d00 };

static constexpr uint16_t FILE_DISK_LOC     { 0xffff };

static constexpr uint16_t DISK_FILE_START   { 0xffff };

static constexpr uint32_t COMP_UNCOMP_FSIZE { 0xffffffff };

static constexpr uint32_t REL_OFFSET_LFH    { 0xffffffff };

static constexpr uint32_t REL_OFFSET_CDFH   { 0xffffffff };

}  // namespace archie::__constants::zip_64

#endif  // ARCHIE_HEADERS_HPP_
