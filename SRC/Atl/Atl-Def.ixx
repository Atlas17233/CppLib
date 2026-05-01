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

    using Uint8 = unsigned char;
    using Uint16 = unsigned short;
    using Uint32 = unsigned int;
    using Uint64 = unsigned long long;

    using Char = char;
    using CharW = wchar_t;
    using Char8 = char8_t;
    using Char16 = char16_t;
    using Char32 = char32_t;

    using Float = float;
    using Double = double;

    using Byte = unsigned char;

    using Int = int;

    using Size = unsigned long long;
    constexpr Size KiB{0x400};
    constexpr Size MiB{0x100000};
    constexpr Size GiB{0x40000000};

    using diffPtr = long long;

    template<typename T>
    class Data
    {
    public:
      constexpr Data(T* data, Size size) noexcept: data_{data}, size_{size} {}

      constexpr Void data(T* data) noexcept { data_ = data; }
      constexpr T* data() const noexcept { return data_; }
      constexpr Void size(Size size) noexcept { size_ = size; }
      constexpr Size size() const noexcept { return size_; }

    protected:
      T* data_;
      Size size_;
    };
  }
}
