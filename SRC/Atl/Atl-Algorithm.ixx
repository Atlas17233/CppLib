export module Atl:Algorithm;

import :Def;
import :Type;

import "Macros";

namespace Atl
{
  template <typename Type>
  [[nodiscard]] constexpr Bool isZero(const Type& value) {
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
        } else {
          if (isZero(value)) {
            memset(begin, 0, (UInt8*)end - (UInt8*)begin);
            return end;
          }
        }
      }
      while (begin != end) {
        *begin++ = value;
      }
      return end;
    }

    template <typename Source, typename IterTarget>
    constexpr IterTarget fill(IterTarget begin, Size size, const Source& value) noexcept
    {
      IterTarget end{begin + size};
      if !consteval {
        if constexpr (sizeof(*begin) == 1) {
          memset(begin, value, size);
          return end;
        } else {
          if (isZero(value)) {
            memset(begin, 0, (UInt8*)end - (UInt8*)begin);
            return end;
          }
        }
      }
      while (begin != end) {
        *begin++ = value;
      }
      return end;
    }

    template <typename IterSource, typename IterTarget>
    constexpr IterTarget copy(IterSource begin, IterSource end, IterTarget iTarget) noexcept
    {
      if !consteval {
        if constexpr (sizeof(*begin) == sizeof(*iTarget)) {
          memcpy(iTarget, begin, (UInt8*)end - (UInt8*)begin);
          return iTarget + (end - begin);
        }
      }
      while (begin != end) {
        *iTarget++ = *begin++;
      }
      return iTarget;
    }

    template <typename IterSource, typename IterTarget>
    constexpr IterTarget copy(IterSource begin, Size size, IterTarget iTarget) noexcept
    {
      IterSource end{begin + size};
      if !consteval {
        if constexpr (sizeof(*begin) == sizeof(*iTarget)) {
          memcpy(iTarget, begin, size * sizeof(*iTarget));
          return iTarget + size;
        }
      }
      while (begin != end) {
        *iTarget++ = *begin++;
      }
      return iTarget;
    }

    /*template <typename Iter1, typename Iter2, typename Pred>
    [[nodiscard]] constexpr Bool equal(Iter1 begin1, Iter1 end1, Iter2 begin2, Pred pred) noexcept
    {
      if constexpr (_Equal_memcmp_is_safe<removeC<Iter1>, removeC<Iter2>, removeC<pred>>) {
        if !consteval {
          return !memcmp(&begin1, &begin2, (const Char*)end1 - (const Char*)begin1);
        }
      }
      for (; begin1 < end1; ++begin1, ++begin2) {
        if (!pred(*begin1, *begin2)) {
          return false;
        }
      }
      return true;
    }

    template <typename Iter1, typename Iter2>
    [[nodiscard]] constexpr Bool equal(Iter1 begin1, Iter1 end1, Iter2 begin2) noexcept
    {
      //return equal(begin1, end1, begin2, equalTo<>{});
      if constexpr (_Equal_memcmp_is_safe<decltype(begin1), decltype(begin2), _Pr>) {
        if !consteval {
          return !memcmp(&begin1, &begin2, (const Char*)end1 - (const Char*)begin1);
        }
      }
      for (; begin1 < end1; ++begin1, ++begin2) {
        if (*begin1 != *begin2) {
          return false;
        }
      }
      return true;
    }

    template <typename Iter1, typename Iter2, typename Pred>
    [[nodiscard]] constexpr Bool equal(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2, Pred pred) noexcept
    {
      if constexpr (_Is_ranges_random_iter_v<_InIt1> && _Is_ranges_random_iter_v<_InIt2>) {
        if (end1 - begin1 != end2 - begin2) {
          return false;
        }
        return equal(begin1, end1, begin2, pred);
      } else {
        for (;; ++begin1, ++begin2) {
          if (begin1 == end1) {
            return begin2 == end2;
          }
          if (begin2 == end2) {
            return false;
          }
          if (!pred(*begin1, *begin2)) {
            return false;
          }
        }
      }
    }

    template <class _InIt1, class _InIt2>
    [[nodiscard]] constexpr Bool equal(const _InIt1 _First1, const _InIt1 _Last1, const _InIt2 _First2, const _InIt2 _Last2)
    {
      return equal(_First1, _Last1, _First2, _Last2, equal_to<>{});
    }*/
  }
}
