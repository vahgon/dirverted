#ifndef ARCHIE_FILE_HEADERS_HPP_
#define ARCHIE_FILE_HEADERS_HPP_

#include <cstdint>
#include <string_view>

namespace archie::headers {

/* 4GiB */
inline constexpr size_t   SizeThreshold{ 1024uz * 1024uz * 1024uz };
inline constexpr uint16_t VerZip{ 10 };
inline constexpr uint16_t VerZip64{ 45 };

/* Central Directory File Header (CDFH) */
class CentDirFileHead {
 public:
  CentDirFileHead();

  explicit CentDirFileHead(const std::string_view path_str)
  : m_path_len{ static_cast<uint16_t>(path_str.size()) } {}

  size_t cdfh_size(size_t var_size) const noexcept {
    return s_Size + var_size;
  }

  template<bool IsZip64>
  friend class EndOfCentDir;

 private:
  static constexpr size_t   s_Size{ 46uz };
  static constexpr uint32_t s_Signature{ 33639248 };

  uint32_t m_mtime{};
  uint32_t m_crc32{};
  uint32_t m_compressed_size{};
  uint32_t m_uncompressed_size{};
  uint16_t m_path_len{};
  uint16_t m_extra_field_len{};
  uint16_t m_comment_len{};
  uint32_t m_disk_start{};
  uint16_t m_internal_attrs{};
  uint32_t m_external_attrs{};
  uint32_t m_lfh_offset{};
};

template<bool IsZip64>
class EndOfCentDir;

template<>
class EndOfCentDir<true> {
 public:
  EndOfCentDir();

  explicit EndOfCentDir(CentDirFileHead& t_cdfh)
  : m_cdfh{ t_cdfh } {}

  uint64_t eocd_size(size_t size) const noexcept {
    return s_Size + size - 12uz;
  }

 private:
  static constexpr size_t   s_Size{ 56uz };
  static constexpr uint32_t s_Signature{ 101075792 };
  static constexpr uint32_t s_LocatorSignature{ 117853008 };

  CentDirFileHead&  m_cdfh;
  uint32_t          m_currdisk{};
  uint32_t          m_cdfh_disk{};
  uint64_t          m_total_cdfh_currdisk{};
  uint64_t          m_total_cdfh_alldisk{};
  uint64_t          m_sizeof_cdfh{};
  uint64_t          m_cd_offset{};  // relative to starting disk
  uint64_t          m_starting_disk{};
  // locator
};

template<>
class EndOfCentDir<false> {
 public:
  EndOfCentDir();

  explicit EndOfCentDir(CentDirFileHead& t_cdfh)
  : m_cdfh{ t_cdfh } {}

  size_t eocd_size(size_t var_size) {
    return s_Size + var_size;
  }

 private:
  static constexpr size_t   s_Size{ 22 };
  static constexpr uint32_t s_Signature{ 101010256 };

  CentDirFileHead& m_cdfh;
  uint16_t m_currdisk{};
  uint16_t m_cdfh_disk{};
  uint16_t m_total_cdfh_currdisk{};
  uint16_t m_total_cdfh_alldisk{};
  uint32_t m_sizeof_cdfh{};
  uint32_t m_cd_offset{};  // relative to starting disk
  uint16_t m_zip_comment_len{};
  // zip comment
};

/* Local File Header (LFH) */
class LocFileHead {
 public:
  explicit LocFileHead(const std::string_view path_str)
  : m_path_len{ static_cast<uint16_t>(path_str.size()) } {}

  template<bool IsZip64>
  friend class ExtraField;

 private:
  static constexpr size_t   s_Size{ 30uz };
  static constexpr uint32_t s_Signature{ 67324752 };
  static constexpr uint16_t s_GenPurposeFlags{ 0 };
  static constexpr uint16_t s_Compression{ 0 };

  uint32_t m_mtime{};
  uint32_t m_crc32{};
  uint32_t m_compressed_size{};
  uint32_t m_uncompressed_size{};
  uint16_t m_path_len{};
  uint16_t m_extra_field_len{};
};

template<bool IsZip64>
class ExtraField;

/* File >= 4GiB */
template<>
class ExtraField<true> {
 public:
  ExtraField();

  explicit ExtraField(LocFileHead& t_lfh)
  : m_lfh{ t_lfh } {}

 private:
  static constexpr size_t   s_FieldTotalSize{ 32 };
  static constexpr uint16_t s_Tag{ 1 };
  static constexpr uint16_t s_FieldSize{ 28 };

  LocFileHead& m_lfh;
};

/* File < 4GiB */
template<>
class ExtraField<false> {
 public:
  ExtraField();

  explicit ExtraField(headers::LocFileHead& t_lfh)
  : m_lfh{ t_lfh } {}

 private:
  static constexpr size_t   s_FieldTotalSize{ 9 };
  static constexpr uint16_t s_Tag{ 21589 };
  static constexpr uint16_t s_FieldSize{ 5 };
  static constexpr uint8_t  s_Flags{ 128 };  // only mtime bit set

  LocFileHead& m_lfh;
};

}  // namespace archie::headers

#endif  // ARCHIE_FILE_HEADERS_HPP_
