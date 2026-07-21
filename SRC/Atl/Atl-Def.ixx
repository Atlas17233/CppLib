export module Atl:Def;

import std;

import "Macros";

namespace Atl
{
  export
  {
    using Void = void;

    using Nullptr = decltype(nullptr);

    using Bool = bool;

    using Int8 = signed char;
    using Int16 = short;
    using Int32 = int;
    using Int64 = long long;

    using UInt8 = unsigned char;
    using UInt16 = unsigned short;
    using UInt32 = unsigned int;
    using UInt64 = unsigned long long;

    using Char = char;
    using CharW = wchar_t;
    using UTF8 = char8_t;
    using UTF16 = char16_t;
    using UTF32 = char32_t;

    using Float = float;
    using Double = double;

    union __declspec(intrin_type) alignas(32) F256
    {
      Float m256_f32[8];
    };

    union __declspec(intrin_type) alignas(32) D256
    {
      Double m256d_f64[4];
    };

    union __declspec(intrin_type) alignas(32) I256
    {
      Int8 m256i_i8[32];
      Int16 m256i_i16[16];
      Int32 m256i_i32[8];
      Int64 m256i_i64[4];
      UInt8 m256i_u8[32];
      UInt16 m256i_u16[16];
      UInt32 m256i_u32[8];
      UInt64 m256i_u64[4];
    };


    using Int = Int32;

    using PtrDiff = Int64;

    using Size = UInt64;
    constexpr Size KiB{0x400};
    constexpr Size MiB{0x100000};
    constexpr Size GiB{0x40000000};
    constexpr Size TiB{0x10000000000};
    constexpr Size PiB{0x4000000000000};
    constexpr Size EiB{0x1000000000000000};

    template <typename Type, typename SizeType = Size>
    struct Data
    {
      Type* data_;
      SizeType size_;
    };

    template <typename Type, typename SizeType = Size>
    class DataClass: protected Data<Type, SizeType>
    {
    public:
      constexpr DataClass(Type* data, SizeType size) noexcept: Data<Type, SizeType>{data, size} {}

      constexpr Type* data() const noexcept { return Data<Type, SizeType>::data_; }
      constexpr SizeType size() const noexcept { return Data<Type, SizeType>::size_; }
    };
  }

  using Long = long;
  using ULong = unsigned long;

  extern "C" {
    UInt16 __lzcnt16(UInt16);
    UInt32 __lzcnt(UInt32);
    UInt64 __lzcnt64(UInt64);

    Void*    memset(Void*, Int, Size);
    Void*    memcpy(Void*, const Void*, Size);
    Void*    memmove(Void*, const Void*, Size);
    Int      memcmp(const Void*, const Void*, Size);


    UInt8   _rotl8(UInt8, UInt8);
    UInt16  _rotl16(UInt16, UInt8);
    UInt32  _rotl(UInt32, Int32);
    UInt64  _rotl64(UInt64, Int32);

    UInt8   _rotr8(UInt8, UInt8);
    UInt16  _rotr16(UInt16, UInt8);
    UInt32  _rotr(UInt32, Int32);
    UInt64  _rotr64(UInt64, Int32);

    Size     strlen(const Char*);

    UInt32  _tzcnt_u32(UInt32);
    UInt64  _tzcnt_u64(UInt64);

    I256  _mm256_cmpeq_epi8(I256, I256);
    I256  _mm256_loadu_si256(const I256*);
    Int32 _mm256_movemask_epi8(I256);
    Void  _mm256_zeroupper();
    I256  _mm256_maskload_epi32(const Int32*, I256);

  }

  consteval UInt64 operator"" _KiB(UInt64 n) { return n * KiB; }
  consteval UInt64 operator"" _MiB(UInt64 n) { return n * MiB; }
  consteval UInt64 operator"" _GiB(UInt64 n) { return n * GiB; }
  consteval UInt64 operator"" _TiB(UInt64 n) { return n * TiB; }
  consteval UInt64 operator"" _PiB(UInt64 n) { return n * PiB; }
  consteval UInt64 operator"" _EiB(UInt64 n) { return n * EiB; }
}
