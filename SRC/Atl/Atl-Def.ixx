export module Atl:Def;

import std;

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


    using Int = Int32;

    using DiffPtr = Int64;

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
  }

  consteval UInt64 operator"" _KiB(UInt64 n) { return n * KiB; }
  consteval UInt64 operator"" _MiB(UInt64 n) { return n * MiB; }
  consteval UInt64 operator"" _GiB(UInt64 n) { return n * GiB; }
  consteval UInt64 operator"" _TiB(UInt64 n) { return n * TiB; }
  consteval UInt64 operator"" _PiB(UInt64 n) { return n * PiB; }
  consteval UInt64 operator"" _EiB(UInt64 n) { return n * EiB; }
}