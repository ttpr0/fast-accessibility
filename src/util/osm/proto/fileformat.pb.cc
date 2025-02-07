// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: fileformat.proto
// Protobuf C++ Version: 5.28.3

#include "fileformat.pb.h"

#include <algorithm>
#include <type_traits>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/generated_message_tctable_impl.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;
namespace OSMPBF {

inline constexpr BlobHeader::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        type_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        indexdata_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        datasize_{0} {}

template <typename>
PROTOBUF_CONSTEXPR BlobHeader::BlobHeader(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct BlobHeaderDefaultTypeInternal {
  PROTOBUF_CONSTEXPR BlobHeaderDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~BlobHeaderDefaultTypeInternal() {}
  union {
    BlobHeader _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 BlobHeaderDefaultTypeInternal _BlobHeader_default_instance_;

inline constexpr Blob::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        raw_size_{0},
        data_{},
        _oneof_case_{} {}

template <typename>
PROTOBUF_CONSTEXPR Blob::Blob(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct BlobDefaultTypeInternal {
  PROTOBUF_CONSTEXPR BlobDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~BlobDefaultTypeInternal() {}
  union {
    Blob _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 BlobDefaultTypeInternal _Blob_default_instance_;
}  // namespace OSMPBF
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_fileformat_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_fileformat_2eproto = nullptr;
const ::uint32_t
    TableStruct_fileformat_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        PROTOBUF_FIELD_OFFSET(::OSMPBF::Blob, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::OSMPBF::Blob, _internal_metadata_),
        ~0u,  // no _extensions_
        PROTOBUF_FIELD_OFFSET(::OSMPBF::Blob, _impl_._oneof_case_[0]),
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::OSMPBF::Blob, _impl_.raw_size_),
        ::_pbi::kInvalidFieldOffsetTag,
        ::_pbi::kInvalidFieldOffsetTag,
        ::_pbi::kInvalidFieldOffsetTag,
        ::_pbi::kInvalidFieldOffsetTag,
        ::_pbi::kInvalidFieldOffsetTag,
        ::_pbi::kInvalidFieldOffsetTag,
        PROTOBUF_FIELD_OFFSET(::OSMPBF::Blob, _impl_.data_),
        0,
        ~0u,
        ~0u,
        ~0u,
        ~0u,
        ~0u,
        ~0u,
        PROTOBUF_FIELD_OFFSET(::OSMPBF::BlobHeader, _impl_._has_bits_),
        PROTOBUF_FIELD_OFFSET(::OSMPBF::BlobHeader, _internal_metadata_),
        ~0u,  // no _extensions_
        ~0u,  // no _oneof_case_
        ~0u,  // no _weak_field_map_
        ~0u,  // no _inlined_string_donated_
        ~0u,  // no _split_
        ~0u,  // no sizeof(Split)
        PROTOBUF_FIELD_OFFSET(::OSMPBF::BlobHeader, _impl_.type_),
        PROTOBUF_FIELD_OFFSET(::OSMPBF::BlobHeader, _impl_.indexdata_),
        PROTOBUF_FIELD_OFFSET(::OSMPBF::BlobHeader, _impl_.datasize_),
        0,
        1,
        2,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, 16, -1, sizeof(::OSMPBF::Blob)},
        {23, 34, -1, sizeof(::OSMPBF::BlobHeader)},
};
static const ::_pb::Message* const file_default_instances[] = {
    &::OSMPBF::_Blob_default_instance_._instance,
    &::OSMPBF::_BlobHeader_default_instance_._instance,
};
const char descriptor_table_protodef_fileformat_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\020fileformat.proto\022\006OSMPBF\"\245\001\n\004Blob\022\020\n\010r"
    "aw_size\030\002 \001(\005\022\r\n\003raw\030\001 \001(\014H\000\022\023\n\tzlib_dat"
    "a\030\003 \001(\014H\000\022\023\n\tlzma_data\030\004 \001(\014H\000\022!\n\023OBSOLE"
    "TE_bzip2_data\030\005 \001(\014B\002\030\001H\000\022\022\n\010lz4_data\030\006 "
    "\001(\014H\000\022\023\n\tzstd_data\030\007 \001(\014H\000B\006\n\004data\"\?\n\nBl"
    "obHeader\022\014\n\004type\030\001 \002(\t\022\021\n\tindexdata\030\002 \001("
    "\014\022\020\n\010datasize\030\003 \002(\005B\017\n\rcrosby.binary"
};
static ::absl::once_flag descriptor_table_fileformat_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_fileformat_2eproto = {
    false,
    false,
    276,
    descriptor_table_protodef_fileformat_2eproto,
    "fileformat.proto",
    &descriptor_table_fileformat_2eproto_once,
    nullptr,
    0,
    2,
    schemas,
    file_default_instances,
    TableStruct_fileformat_2eproto::offsets,
    file_level_enum_descriptors_fileformat_2eproto,
    file_level_service_descriptors_fileformat_2eproto,
};
namespace OSMPBF {
// ===================================================================

class Blob::_Internal {
 public:
  using HasBits =
      decltype(std::declval<Blob>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(Blob, _impl_._has_bits_);
  static constexpr ::int32_t kOneofCaseOffset =
      PROTOBUF_FIELD_OFFSET(::OSMPBF::Blob, _impl_._oneof_case_);
};

Blob::Blob(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:OSMPBF.Blob)
}
inline PROTOBUF_NDEBUG_INLINE Blob::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::OSMPBF::Blob& from_msg)
      : _has_bits_{from._has_bits_},
        _cached_size_{0},
        data_{},
        _oneof_case_{from._oneof_case_[0]} {}

Blob::Blob(
    ::google::protobuf::Arena* arena,
    const Blob& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  Blob* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  _impl_.raw_size_ = from._impl_.raw_size_;
  switch (data_case()) {
    case DATA_NOT_SET:
      break;
      case kRaw:
        new (&_impl_.data_.raw_) decltype(_impl_.data_.raw_){arena, from._impl_.data_.raw_};
        break;
      case kZlibData:
        new (&_impl_.data_.zlib_data_) decltype(_impl_.data_.zlib_data_){arena, from._impl_.data_.zlib_data_};
        break;
      case kLzmaData:
        new (&_impl_.data_.lzma_data_) decltype(_impl_.data_.lzma_data_){arena, from._impl_.data_.lzma_data_};
        break;
      case kOBSOLETEBzip2Data:
        new (&_impl_.data_.obsolete_bzip2_data_) decltype(_impl_.data_.obsolete_bzip2_data_){arena, from._impl_.data_.obsolete_bzip2_data_};
        break;
      case kLz4Data:
        new (&_impl_.data_.lz4_data_) decltype(_impl_.data_.lz4_data_){arena, from._impl_.data_.lz4_data_};
        break;
      case kZstdData:
        new (&_impl_.data_.zstd_data_) decltype(_impl_.data_.zstd_data_){arena, from._impl_.data_.zstd_data_};
        break;
  }

  // @@protoc_insertion_point(copy_constructor:OSMPBF.Blob)
}
inline PROTOBUF_NDEBUG_INLINE Blob::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0},
        data_{},
        _oneof_case_{} {}

inline void Blob::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.raw_size_ = {};
}
Blob::~Blob() {
  // @@protoc_insertion_point(destructor:OSMPBF.Blob)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void Blob::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  if (has_data()) {
    clear_data();
  }
  _impl_.~Impl_();
}

void Blob::clear_data() {
// @@protoc_insertion_point(one_of_clear_start:OSMPBF.Blob)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  switch (data_case()) {
    case kRaw: {
      _impl_.data_.raw_.Destroy();
      break;
    }
    case kZlibData: {
      _impl_.data_.zlib_data_.Destroy();
      break;
    }
    case kLzmaData: {
      _impl_.data_.lzma_data_.Destroy();
      break;
    }
    case kOBSOLETEBzip2Data: {
      _impl_.data_.obsolete_bzip2_data_.Destroy();
      break;
    }
    case kLz4Data: {
      _impl_.data_.lz4_data_.Destroy();
      break;
    }
    case kZstdData: {
      _impl_.data_.zstd_data_.Destroy();
      break;
    }
    case DATA_NOT_SET: {
      break;
    }
  }
  _impl_._oneof_case_[0] = DATA_NOT_SET;
}


PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::MessageLite::ClassDataFull
    Blob::_class_data_ = {
        ::google::protobuf::Message::ClassData{
            &_Blob_default_instance_._instance,
            &_table_.header,
            nullptr,  // OnDemandRegisterArenaDtor
            nullptr,  // IsInitialized
            &Blob::MergeImpl,
#if defined(PROTOBUF_CUSTOM_VTABLE)
            ::google::protobuf::Message::GetDeleteImpl<Blob>(),
            ::google::protobuf::Message::GetNewImpl<Blob>(),
            ::google::protobuf::Message::GetClearImpl<Blob>(), &Blob::ByteSizeLong,
                &Blob::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
            PROTOBUF_FIELD_OFFSET(Blob, _impl_._cached_size_),
            false,
        },
        &Blob::kDescriptorMethods,
        &descriptor_table_fileformat_2eproto,
        nullptr,  // tracker
};
const ::google::protobuf::MessageLite::ClassData* Blob::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<0, 7, 0, 0, 2> Blob::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(Blob, _impl_._has_bits_),
    0, // no _extensions_
    7, 0,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967168,  // skipmap
    offsetof(decltype(_table_), field_entries),
    7,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::OSMPBF::Blob>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    // optional int32 raw_size = 2;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(Blob, _impl_.raw_size_), 0>(),
     {16, 0, 0, PROTOBUF_FIELD_OFFSET(Blob, _impl_.raw_size_)}},
  }}, {{
    65535, 65535
  }}, {{
    // bytes raw = 1;
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.data_.raw_), _Internal::kOneofCaseOffset + 0, 0,
    (0 | ::_fl::kFcOneof | ::_fl::kBytes | ::_fl::kRepAString)},
    // optional int32 raw_size = 2;
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.raw_size_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // bytes zlib_data = 3;
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.data_.zlib_data_), _Internal::kOneofCaseOffset + 0, 0,
    (0 | ::_fl::kFcOneof | ::_fl::kBytes | ::_fl::kRepAString)},
    // bytes lzma_data = 4;
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.data_.lzma_data_), _Internal::kOneofCaseOffset + 0, 0,
    (0 | ::_fl::kFcOneof | ::_fl::kBytes | ::_fl::kRepAString)},
    // bytes OBSOLETE_bzip2_data = 5 [deprecated = true];
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.data_.obsolete_bzip2_data_), _Internal::kOneofCaseOffset + 0, 0,
    (0 | ::_fl::kFcOneof | ::_fl::kBytes | ::_fl::kRepAString)},
    // bytes lz4_data = 6;
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.data_.lz4_data_), _Internal::kOneofCaseOffset + 0, 0,
    (0 | ::_fl::kFcOneof | ::_fl::kBytes | ::_fl::kRepAString)},
    // bytes zstd_data = 7;
    {PROTOBUF_FIELD_OFFSET(Blob, _impl_.data_.zstd_data_), _Internal::kOneofCaseOffset + 0, 0,
    (0 | ::_fl::kFcOneof | ::_fl::kBytes | ::_fl::kRepAString)},
  }},
  // no aux_entries
  {{
  }},
};

PROTOBUF_NOINLINE void Blob::Clear() {
// @@protoc_insertion_point(message_clear_start:OSMPBF.Blob)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.raw_size_ = 0;
  clear_data();
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* Blob::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const Blob& this_ = static_cast<const Blob&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* Blob::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const Blob& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:OSMPBF.Blob)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          // bytes raw = 1;
          if (this_.data_case() == kRaw) {
            const std::string& _s = this_._internal_raw();
            target = stream->WriteBytesMaybeAliased(1, _s, target);
          }

          cached_has_bits = this_._impl_._has_bits_[0];
          // optional int32 raw_size = 2;
          if (cached_has_bits & 0x00000001u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<2>(
                    stream, this_._internal_raw_size(), target);
          }

          switch (this_.data_case()) {
            case kZlibData: {
              const std::string& _s = this_._internal_zlib_data();
              target = stream->WriteBytesMaybeAliased(3, _s, target);
              break;
            }
            case kLzmaData: {
              const std::string& _s = this_._internal_lzma_data();
              target = stream->WriteBytesMaybeAliased(4, _s, target);
              break;
            }
            case kOBSOLETEBzip2Data: {
              const std::string& _s = this_._internal_obsolete_bzip2_data();
              target = stream->WriteBytesMaybeAliased(5, _s, target);
              break;
            }
            case kLz4Data: {
              const std::string& _s = this_._internal_lz4_data();
              target = stream->WriteBytesMaybeAliased(6, _s, target);
              break;
            }
            case kZstdData: {
              const std::string& _s = this_._internal_zstd_data();
              target = stream->WriteBytesMaybeAliased(7, _s, target);
              break;
            }
            default:
              break;
          }
          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:OSMPBF.Blob)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t Blob::ByteSizeLong(const MessageLite& base) {
          const Blob& this_ = static_cast<const Blob&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t Blob::ByteSizeLong() const {
          const Blob& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:OSMPBF.Blob)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

           {
            // optional int32 raw_size = 2;
            cached_has_bits =
                this_._impl_._has_bits_[0];
            if (cached_has_bits & 0x00000001u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_raw_size());
            }
          }
          switch (this_.data_case()) {
            // bytes raw = 1;
            case kRaw: {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_raw());
              break;
            }
            // bytes zlib_data = 3;
            case kZlibData: {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_zlib_data());
              break;
            }
            // bytes lzma_data = 4;
            case kLzmaData: {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_lzma_data());
              break;
            }
            // bytes OBSOLETE_bzip2_data = 5 [deprecated = true];
            case kOBSOLETEBzip2Data: {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_obsolete_bzip2_data());
              break;
            }
            // bytes lz4_data = 6;
            case kLz4Data: {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_lz4_data());
              break;
            }
            // bytes zstd_data = 7;
            case kZstdData: {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_zstd_data());
              break;
            }
            case DATA_NOT_SET: {
              break;
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void Blob::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<Blob*>(&to_msg);
  auto& from = static_cast<const Blob&>(from_msg);
  ::google::protobuf::Arena* arena = _this->GetArena();
  // @@protoc_insertion_point(class_specific_merge_from_start:OSMPBF.Blob)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    _this->_impl_.raw_size_ = from._impl_.raw_size_;
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  if (const uint32_t oneof_from_case = from._impl_._oneof_case_[0]) {
    const uint32_t oneof_to_case = _this->_impl_._oneof_case_[0];
    const bool oneof_needs_init = oneof_to_case != oneof_from_case;
    if (oneof_needs_init) {
      if (oneof_to_case != 0) {
        _this->clear_data();
      }
      _this->_impl_._oneof_case_[0] = oneof_from_case;
    }

    switch (oneof_from_case) {
      case kRaw: {
        if (oneof_needs_init) {
          _this->_impl_.data_.raw_.InitDefault();
        }
        _this->_impl_.data_.raw_.Set(from._internal_raw(), arena);
        break;
      }
      case kZlibData: {
        if (oneof_needs_init) {
          _this->_impl_.data_.zlib_data_.InitDefault();
        }
        _this->_impl_.data_.zlib_data_.Set(from._internal_zlib_data(), arena);
        break;
      }
      case kLzmaData: {
        if (oneof_needs_init) {
          _this->_impl_.data_.lzma_data_.InitDefault();
        }
        _this->_impl_.data_.lzma_data_.Set(from._internal_lzma_data(), arena);
        break;
      }
      case kOBSOLETEBzip2Data: {
        if (oneof_needs_init) {
          _this->_impl_.data_.obsolete_bzip2_data_.InitDefault();
        }
        _this->_impl_.data_.obsolete_bzip2_data_.Set(from._internal_obsolete_bzip2_data(), arena);
        break;
      }
      case kLz4Data: {
        if (oneof_needs_init) {
          _this->_impl_.data_.lz4_data_.InitDefault();
        }
        _this->_impl_.data_.lz4_data_.Set(from._internal_lz4_data(), arena);
        break;
      }
      case kZstdData: {
        if (oneof_needs_init) {
          _this->_impl_.data_.zstd_data_.InitDefault();
        }
        _this->_impl_.data_.zstd_data_.Set(from._internal_zstd_data(), arena);
        break;
      }
      case DATA_NOT_SET:
        break;
    }
  }
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void Blob::CopyFrom(const Blob& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:OSMPBF.Blob)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}


void Blob::InternalSwap(Blob* PROTOBUF_RESTRICT other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
        swap(_impl_.raw_size_, other->_impl_.raw_size_);
  swap(_impl_.data_, other->_impl_.data_);
  swap(_impl_._oneof_case_[0], other->_impl_._oneof_case_[0]);
}

::google::protobuf::Metadata Blob::GetMetadata() const {
  return ::google::protobuf::Message::GetMetadataImpl(GetClassData()->full());
}
// ===================================================================

class BlobHeader::_Internal {
 public:
  using HasBits =
      decltype(std::declval<BlobHeader>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_._has_bits_);
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000005) ^ 0x00000005) != 0;
  }
};

BlobHeader::BlobHeader(::google::protobuf::Arena* arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:OSMPBF.BlobHeader)
}
inline PROTOBUF_NDEBUG_INLINE BlobHeader::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility, ::google::protobuf::Arena* arena,
    const Impl_& from, const ::OSMPBF::BlobHeader& from_msg)
      : _has_bits_{from._has_bits_},
        _cached_size_{0},
        type_(arena, from.type_),
        indexdata_(arena, from.indexdata_) {}

BlobHeader::BlobHeader(
    ::google::protobuf::Arena* arena,
    const BlobHeader& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, _class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  BlobHeader* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  _impl_.datasize_ = from._impl_.datasize_;

  // @@protoc_insertion_point(copy_constructor:OSMPBF.BlobHeader)
}
inline PROTOBUF_NDEBUG_INLINE BlobHeader::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* arena)
      : _cached_size_{0},
        type_(arena),
        indexdata_(arena) {}

inline void BlobHeader::SharedCtor(::_pb::Arena* arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  _impl_.datasize_ = {};
}
BlobHeader::~BlobHeader() {
  // @@protoc_insertion_point(destructor:OSMPBF.BlobHeader)
  _internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  SharedDtor();
}
inline void BlobHeader::SharedDtor() {
  ABSL_DCHECK(GetArena() == nullptr);
  _impl_.type_.Destroy();
  _impl_.indexdata_.Destroy();
  _impl_.~Impl_();
}

PROTOBUF_CONSTINIT
PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::google::protobuf::MessageLite::ClassDataFull
    BlobHeader::_class_data_ = {
        ::google::protobuf::Message::ClassData{
            &_BlobHeader_default_instance_._instance,
            &_table_.header,
            nullptr,  // OnDemandRegisterArenaDtor
            BlobHeader::IsInitializedImpl,
            &BlobHeader::MergeImpl,
#if defined(PROTOBUF_CUSTOM_VTABLE)
            ::google::protobuf::Message::GetDeleteImpl<BlobHeader>(),
            ::google::protobuf::Message::GetNewImpl<BlobHeader>(),
            ::google::protobuf::Message::GetClearImpl<BlobHeader>(), &BlobHeader::ByteSizeLong,
                &BlobHeader::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
            PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_._cached_size_),
            false,
        },
        &BlobHeader::kDescriptorMethods,
        &descriptor_table_fileformat_2eproto,
        nullptr,  // tracker
};
const ::google::protobuf::MessageLite::ClassData* BlobHeader::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(_class_data_.tc_table);
  return _class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<2, 3, 0, 30, 2> BlobHeader::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_._has_bits_),
    0, // no _extensions_
    3, 24,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967288,  // skipmap
    offsetof(decltype(_table_), field_entries),
    3,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    _class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::OSMPBF::BlobHeader>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // required string type = 1;
    {::_pbi::TcParser::FastSS1,
     {10, 0, 0, PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_.type_)}},
    // optional bytes indexdata = 2;
    {::_pbi::TcParser::FastBS1,
     {18, 1, 0, PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_.indexdata_)}},
    // required int32 datasize = 3;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(BlobHeader, _impl_.datasize_), 2>(),
     {24, 2, 0, PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_.datasize_)}},
  }}, {{
    65535, 65535
  }}, {{
    // required string type = 1;
    {PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_.type_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kRawString | ::_fl::kRepAString)},
    // optional bytes indexdata = 2;
    {PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_.indexdata_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kBytes | ::_fl::kRepAString)},
    // required int32 datasize = 3;
    {PROTOBUF_FIELD_OFFSET(BlobHeader, _impl_.datasize_), _Internal::kHasBitsOffset + 2, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
  }},
  // no aux_entries
  {{
    "\21\4\0\0\0\0\0\0"
    "OSMPBF.BlobHeader"
    "type"
  }},
};

PROTOBUF_NOINLINE void BlobHeader::Clear() {
// @@protoc_insertion_point(message_clear_start:OSMPBF.BlobHeader)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      _impl_.type_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      _impl_.indexdata_.ClearNonDefaultToEmpty();
    }
  }
  _impl_.datasize_ = 0;
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::uint8_t* BlobHeader::_InternalSerialize(
            const MessageLite& base, ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) {
          const BlobHeader& this_ = static_cast<const BlobHeader&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::uint8_t* BlobHeader::_InternalSerialize(
            ::uint8_t* target,
            ::google::protobuf::io::EpsCopyOutputStream* stream) const {
          const BlobHeader& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(serialize_to_array_start:OSMPBF.BlobHeader)
          ::uint32_t cached_has_bits = 0;
          (void)cached_has_bits;

          cached_has_bits = this_._impl_._has_bits_[0];
          // required string type = 1;
          if (cached_has_bits & 0x00000001u) {
            const std::string& _s = this_._internal_type();
            ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(_s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormat::SERIALIZE,
                                        "OSMPBF.BlobHeader.type");
            target = stream->WriteStringMaybeAliased(1, _s, target);
          }

          // optional bytes indexdata = 2;
          if (cached_has_bits & 0x00000002u) {
            const std::string& _s = this_._internal_indexdata();
            target = stream->WriteBytesMaybeAliased(2, _s, target);
          }

          // required int32 datasize = 3;
          if (cached_has_bits & 0x00000004u) {
            target = ::google::protobuf::internal::WireFormatLite::
                WriteInt32ToArrayWithField<3>(
                    stream, this_._internal_datasize(), target);
          }

          if (PROTOBUF_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
            target =
                ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
                    this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
          }
          // @@protoc_insertion_point(serialize_to_array_end:OSMPBF.BlobHeader)
          return target;
        }

#if defined(PROTOBUF_CUSTOM_VTABLE)
        ::size_t BlobHeader::ByteSizeLong(const MessageLite& base) {
          const BlobHeader& this_ = static_cast<const BlobHeader&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
        ::size_t BlobHeader::ByteSizeLong() const {
          const BlobHeader& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
          // @@protoc_insertion_point(message_byte_size_start:OSMPBF.BlobHeader)
          ::size_t total_size = 0;

          ::uint32_t cached_has_bits = 0;
          // Prevent compiler warnings about cached_has_bits being unused
          (void)cached_has_bits;

          ::_pbi::Prefetch5LinesFrom7Lines(&this_);
           {
            // required string type = 1;
            cached_has_bits =
                this_._impl_._has_bits_[0];
            if (cached_has_bits & 0x00000001u) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                              this_._internal_type());
            }
          }
           {
            // optional bytes indexdata = 2;
            if (cached_has_bits & 0x00000002u) {
              total_size += 1 + ::google::protobuf::internal::WireFormatLite::BytesSize(
                                              this_._internal_indexdata());
            }
          }
           {
            // required int32 datasize = 3;
            if (cached_has_bits & 0x00000004u) {
              total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
                  this_._internal_datasize());
            }
          }
          return this_.MaybeComputeUnknownFieldsSize(total_size,
                                                     &this_._impl_._cached_size_);
        }

void BlobHeader::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<BlobHeader*>(&to_msg);
  auto& from = static_cast<const BlobHeader&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:OSMPBF.BlobHeader)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if (cached_has_bits & 0x00000007u) {
    if (cached_has_bits & 0x00000001u) {
      _this->_internal_set_type(from._internal_type());
    }
    if (cached_has_bits & 0x00000002u) {
      _this->_internal_set_indexdata(from._internal_indexdata());
    }
    if (cached_has_bits & 0x00000004u) {
      _this->_impl_.datasize_ = from._impl_.datasize_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void BlobHeader::CopyFrom(const BlobHeader& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:OSMPBF.BlobHeader)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool BlobHeader::IsInitializedImpl(
    const MessageLite& msg) {
  auto& this_ = static_cast<const BlobHeader&>(msg);
  if (_Internal::MissingRequiredFields(this_._impl_._has_bits_)) {
    return false;
  }
  return true;
}

void BlobHeader::InternalSwap(BlobHeader* PROTOBUF_RESTRICT other) {
  using std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.type_, &other->_impl_.type_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.indexdata_, &other->_impl_.indexdata_, arena);
        swap(_impl_.datasize_, other->_impl_.datasize_);
}

::google::protobuf::Metadata BlobHeader::GetMetadata() const {
  return ::google::protobuf::Message::GetMetadataImpl(GetClassData()->full());
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace OSMPBF
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::std::false_type
    _static_init2_ PROTOBUF_UNUSED =
        (::_pbi::AddDescriptors(&descriptor_table_fileformat_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
