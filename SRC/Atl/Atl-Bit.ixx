export module Atl:Bit;

import :Def;

namespace Atl
{
  export
  {
    constexpr Byte hex[16]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    constexpr Byte HEX[16]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    [[nodiscard]] constexpr Uint convertEndian(Uint data) noexcept
    {
      return (data & 0xff000000) >> 24
           | (data & 0x00ff0000) >> 8
           | (data & 0x0000ff00) << 8
           | (data & 0x000000ff) << 24;
    }

    [[nodiscard]] constexpr Uint64 convertEndian64(Uint64 data) noexcept
    {
      return (data & 0xff00000000000000) >> 56
           | (data & 0x00ff000000000000) >> 40
           | (data & 0x0000ff0000000000) >> 24
           | (data & 0x000000ff00000000) >> 8
           | (data & 0x00000000ff000000) << 8
           | (data & 0x0000000000ff0000) << 24
           | (data & 0x000000000000ff00) << 40
           | (data & 0x00000000000000ff) << 56;
    }

    template<typename T>
    [[nodiscard]] constexpr T rotL(T data, Int n) noexcept { return std::rotl(data, n); }

    template<typename T>
    [[nodiscard]] constexpr T rotR(T data, Int n) noexcept { return std::rotr(data, n); }

    template<typename To, typename From>
    requires (sizeof(To) == sizeof(From))
    [[nodiscard]] constexpr To bitCast(const From& value) noexcept
    {
      return __builtin_bit_cast(To, value);
    }
  }
}
