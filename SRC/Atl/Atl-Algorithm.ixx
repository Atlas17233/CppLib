export module Atl:Algorithm;

import :Bit;
import :Common;
import :Def;

import <memory.h>;

namespace Atl
{
  template<typename T>
  [[nodiscard]] constexpr Bool isZero(const T& value) {
    if constexpr (sizeof(T) == 1) {
      return !bitCast<Uint8>(value);
    } else if constexpr (sizeof(T) == 2) {
      return !bitCast<Uint16>(value);
    } else if constexpr (sizeof(T) == 4) {
      return !bitCast<Uint>(value);
    } else if constexpr (sizeof(T) == 8) {
      return !bitCast<Uint64>(value);
    } else {
      ;
    }
  }

  export
  {
    template<typename Source, typename IterTarget>
    constexpr Void fill(IterTarget begin, IterTarget end, const Source& value) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*begin) == 1) {
          memset(begin, value, end - begin);
          return;
        } else if (isZero(value)) {
          memset(begin, 0, (const Byte*)end - (const Byte*)begin);
          return;
        }
      }
      while (begin < end) *begin++ = value;
    }

    template<typename Source, typename IterTarget>
    constexpr Void/*IterTarget*/ fill(IterTarget iTarget, Size size, const Source& value) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*iTarget) == 1) {
          memset(iTarget, value, size);
          return/* iTarget + size*/;
        } else if (isZero(value)) {
          memset(iTarget, 0, size * sizeof(*iTarget));
          return/* iTarget + size * sizeof(*iTarget)*/;
        }
      }
      while (size--) *iTarget++ = value;
      //return iTarget;
    }

    template<typename IterSource, typename IterTarget>
    constexpr Void/*IterTarget*/ copy(IterSource begin, IterSource end, IterTarget iTarget) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*begin) == sizeof(*iTarget)) {
          memcpy(iTarget, begin, (const Byte*)end - (const Byte*)begin);
          return/* iTarget + (end - begin)*/;
        }
      }
      while (begin < end) *iTarget++ = *begin++;
      //return iTarget;
    }

    template<typename IterSource, typename IterTarget>
    constexpr Void/*IterTarget*/ copy(IterSource iSource, Size size, IterTarget iTarget) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*iSource) == sizeof(*iTarget)) {
          memcpy(iTarget, iSource, size * sizeof(*iTarget));
          return/* iTarget + size*/;
        }
      }
      while (size--) *iTarget++ = *iSource++;
      //return iTarget;
    }
  }
}
