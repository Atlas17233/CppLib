export module Atl:Algorithm;

import :Def;
import :Type;

namespace Atl
{
  template <typename Type>
  [[nodiscard]] constexpr Bool isAllBitsZero(const Type& value) {
    if constexpr (isNullptr<Type>) {
      return true;
    } else if constexpr (isIntegral<Type> || isPointer<Type>) {
      return !value;
    } else if constexpr (sizeof(Type) == 1) {
      return !bitCast<UInt8>(value);
    } else if constexpr (sizeof(Type) == 2) {
      return !bitCast<UInt16>(value);
    } else if constexpr (sizeof(Type) == 4) {
      return !bitCast<UInt32>(value);
    } else if constexpr (sizeof(Type) == 8) {
      return !bitCast<UInt64>(value);
    } else {
      static constexpr Type zero{};
      return !compare(&value, &zero, sizeof(Type));
    }
  }

  export
  {
    template <typename Iter1, typename Iter2>
    [[nodiscard]] constexpr Int compare(Iter1 iter1, Iter2 iter2, Size size)
    {
      if !consteval {
        if constexpr (sizeof(*iter1) == sizeof(*iter2)) {
          return memcmp(iter1, iter2, size);
        }
      }
      for (; size--; ++iter1, ++iter2) {
        if (*iter1 != *iter2) {
          return *iter1 < *iter2 ? -1 : 1;
        }
      }
      return 0;
    }

    template <typename Source, typename IterTarget>
    constexpr IterTarget fill(IterTarget begin, IterTarget end, const Source& value) noexcept
    {
      if !consteval {
        if constexpr (sizeof(*begin) == 1) {
          memset(begin, value, end - begin);
          return end;
        } else if (isZero(value)) {
          memset(begin, 0, (const UInt8*)end - (const UInt8*)begin);
          return end;
        }
      }
      while (begin < end) {
        *begin++ = value;
      }
      return end;
    }

    template <typename Source, typename IterTarget>
    constexpr IterTarget fill(IterTarget iTarget, Size size, const Source& value) noexcept
    {
      if !consteval {
        if constexpr (sizeof(*iTarget) == 1) {
          memset(iTarget, value, size);
          return iTarget + size;
        } else if (isZero(value)) {
          memset(iTarget, 0, size * sizeof(*iTarget));
          return iTarget + size * sizeof(*iTarget);
        }
      }
      while (size--) {
        *iTarget++ = value;
      }
      return iTarget;
    }

    template <typename IterSource, typename IterTarget>
    constexpr IterTarget copy(IterSource begin, IterSource end, IterTarget iTarget) noexcept
    {
      if !consteval {
        if constexpr (sizeof(*begin) == sizeof(*iTarget)) {
          memcpy(iTarget, begin, (const UInt8*)end - (const UInt8*)begin);
          return iTarget + (end - begin);
        }
      }
      while (begin < end) {
        *iTarget++ = *begin++;
      }
      return iTarget;
    }

    template <typename IterSource, typename IterTarget>
    constexpr IterTarget copy(IterSource iSource, Size size, IterTarget iTarget) noexcept
    {
      if !consteval {
        if constexpr (sizeof(*iSource) == sizeof(*iTarget)) {
          memcpy(iTarget, iSource, size * sizeof(*iTarget));
          return iTarget + size;
        }
      }
      while (size--) {
        *iTarget++ = *iSource++;
      }
      return iTarget;
    }
  }
}
