export module Atl:Bit;

import :Concepts;
import :Def;
import :Limits;

namespace Atl
{
  template <typename Type>
  [[nodiscard]] constexpr Int lZero(Type value) noexcept {
    Type v = 0;

    UInt32 n = sizeof(Type) << 3;
    UInt32 c = sizeof(Type) << 2;
    do {
      v = value >> c;
      if (v) {
        n -= c;
        value = v;
      }
      c >>= 1;
    } while (c);
    return n - value;
  }

  export
  {
    constexpr UInt8 hex[0x10]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    constexpr UInt8 HEX[0x10]{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    template<StandardUnsignedIntegral Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr Type rotL(Type data, Int n) noexcept
    {
      if !consteval {
        if constexpr (sizeof(Type) == 1) {
          return _rotl8(data, n);
        } else if constexpr (sizeof(Type) == 2) {
          return _rotl16(data, n);
        } else if constexpr (sizeof(Type) == 4) {
          return _rotl(data, n);
        } else {
          return _rotl64(data, n);
        }
      }
      n %= sizeof(Type) << 3;
      return data << n | data >> (sizeof(Type) << 3) - n;
    }

    template<StandardUnsignedIntegral Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr Type rotR(Type data, Int n) noexcept
    {
      if !consteval {
        if constexpr (sizeof(Type) == 1) {
          return _rotr8(data, n);
        } else if constexpr (sizeof(Type) == 2) {
          return _rotr16(data, n);
        } else if constexpr (sizeof(Type) == 4) {
          return _rotr(data, n);
        } else {
          return _rotr64(data, n);
        }
      }
      n %= sizeof(Type) << 3;
      return data >> n | data << (sizeof(Type) << 3) - n;
    }

    template <StandardUnsignedIntegral Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr Int isolateLowestSetBit(Type value) noexcept { return value & -value; }

    template <StandardUnsignedIntegral Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr Int countLeadingZero(const Type value) noexcept
    {
      if !consteval {
        if constexpr (sizeof(Type) == 1) {
          return __lzcnt16(value) - 8;
        } else if constexpr (sizeof(Type) == 2) {
          return __lzcnt16(value);
        } else if constexpr (sizeof(Type) == 4) {
          return __lzcnt(value);
        } else {
          return __lzcnt64(value);
        }
      }
      return lZero(value);
    }

    template <StandardUnsignedIntegral Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr Int countTrailingZero(const Type value) noexcept
    {
      if !consteval {
        if constexpr (sizeof(Type) < 8) {
          return _tzcnt_u32(~Max<Type> | value);
        } else {
          return _tzcnt_u64(value);
        }
      }
      return (sizeof(Type) << 3) - lZero((Type)(~value & value - 1));
    }

    template <StandardUnsignedIntegral Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr Type swapByte(const Type value) noexcept
    {
      if constexpr (sizeof(Type) == 1) {
        return value;
      } else if constexpr (sizeof(Type) == 2) {
        return value << 8 | value >> 8;
      } else if constexpr (sizeof(Type) == 4) {
        return value << 24 | value << 8 & 0xff0000 | value >> 8 & 0xff00 | value >> 24;
      } else {
        return value << 56 | value << 40 & 0xff000000000000 | value << 24 & 0xff0000000000 | value << 8 & 0xff00000000
            | value >> 8 & 0xff000000 | value >> 24 & 0xff0000 | value >> 40 & 0xff00 | value >> 56;
      }
    }

    template<typename To, typename From>
    requires (sizeof(To) == sizeof(From) && isTriviallyCopyable<To> && isTriviallyCopyable<From>)
    [[msvc::forceinline]] [[nodiscard]] constexpr To bitCast(const From& value) noexcept
    {
      return __builtin_bit_cast(To, value);
    }
  }
}
