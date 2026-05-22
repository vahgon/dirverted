#ifndef ARCHIE_FILE_HEADERS_HPP_
#define ARCHIE_FILE_HEADERS_HPP_

#include <cstdint>

// zip64 is used if file exceeds 4GiB (SIZE_THRESHOLD)
namespace headers { static constexpr std::size_t SizeThreshold{ 1024ULL * 1024ULL * 1024ULL }; }

namespace headers::lfh {

// does not include undeterministic sizes of extra-field and file name
static constexpr std::uint32_t  Signature{ 0x04034b50 };
static constexpr std::size_t    Size{ 30 };
static constexpr std::uint16_t  GenPurposeFlags{ 0 };
static constexpr std::uint16_t  Compression{ 0 };

}  // namespace headers::lfh

namespace headers::lfh::extra_field::zip {

static constexpr std::uint16_t  Id{ 21589 };
static constexpr std::size_t    Size{ 9 };
static constexpr std::uint16_t  FieldSize{ 5 };
static constexpr std::uint8_t   Flags{ 128 };  // only mtime bit set

}  // namespace headers::lfh::extra_field::zip

namespace headers::lfh::extra_field::zip64 {

static constexpr std::uint16_t  Id{ 1 };
static constexpr std::size_t    Size{ 32 };
static constexpr std::uint16_t  FieldSize{ 28 };

}  // namespace headers::lfh::extra_field::zip64

namespace headers::cdfh {

// does not include undeterministic sizes file name, extra-field, and comment
static constexpr std::uint32_t  Signature{ 1347092738 };
static constexpr std::size_t    Size{ 46 };

}  // namespace headers::cdfh

namespace headers::eocd {

static constexpr std::uint32_t  Signature{ 1347093766 };
static constexpr std::size_t    Size{ 22 };

}  // namespace headers::eocd

namespace headers::eocd::zip64 {

static constexpr std::uint32_t  Signature{ 1347094022 };
static constexpr std::size_t    Size{ 56 };

}  // namespace headers::eocd::zip64

namespace headers::eocd::zip64::locator {

static constexpr std::uint32_t  Signature{ 1347094023 };
static constexpr std::size_t    Size{ 16 };

}  // namespace headers::eocd::zip64::locator

#endif  // ARCHIE_FILE_HEADERS_HPP_
