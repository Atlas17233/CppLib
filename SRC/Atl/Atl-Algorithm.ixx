export module Atl:Algorithm;

import :Bit;
import :Common;
import :Def;
import :Type;

import <memory.h>;

namespace Atl
{
  template<typename T>
  [[nodiscard]] constexpr Bool isAllBitsZero(const T& value) {
    if constexpr (isNullPointer<T>) {
      return true;
    } else if constexpr (isIntegral<T> || isPointer<T>) {
      return !value;
    } else if constexpr (sizeof(T) == 1) {
      return !bit_cast<Uint8>(value);
    } else if constexpr (sizeof(T) == 2) {
      return !bit_cast<Uint16>(value);
    } else if constexpr (sizeof(T) == 4) {
      return !bit_cast<Uint32>(value);
    } else if constexpr (sizeof(T) == 8) {
      return !bit_cast<Uint64>(value);
    } else {
      static constexpr T zero{};
      return !compare(&value, &zero, sizeof(T));
    }
  }

  export
  {
    template<typename Iter1, typename Iter2>
    [[nodiscard]] constexpr Int compare(Iter1 iter1, Iter2 iter2, Size size)
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*iter1) == sizeof(*iter2)) return memcmp(iter1, iter2, size);
      }
      for (; size--; ++iter1, ++iter2)
      {
        if (*iter1 != *iter2) return *iter1 < *iter2 ? -1 : 1;
      }
      return 0;
    }
/*
template <class T>
using withReference = T&;

template <class T>
concept canReference = requires { typename withReference<T>; };

template <class>
constexpr bool isConst = false;

template <class T>
constexpr bool isConst<const T> = true;

template <class T>
constexpr bool isObject = isConst<const T> && !isVoid<T>;

template <class T>
concept isDestructible = __is_nothrow_destructible(T);

template <class T, class... Args>
constexpr Bool isConstructible = __is_constructible(T, Args...);

template <class T, class... Args>
concept isConstructibleFrom = isDestructible<T> && isConstructible(T, Args...);

template <class From, class To>
concept isConvertibleTo = __is_convertible_to(From, To)
    && requires { static_cast<To>(declvalue<From>()); };

template <class T>
concept isMoveConstructible = isConstructibleFrom<T, T> && isConvertibleTo<T, T>;

//可移动的
template <class T>
concept movable = isObject<T> && isMoveConstructible<T> && assignable_from<T&, T> && swappable<T>;

//弱的可增的
template <class T>
concept weakly_incrementable = movable<T> && requires(T __i) {
    typename iter_difference_t<T>;
    requires _Signed_integer_like<iter_difference_t<T>>;
    { ++__i } -> same_as<T&>;
    __i++;
};

//输入或输出迭代器
template <class T>
concept isInputOrOutputIterator = requires(T __i) {
    { *__i } -> canReference;
} && weakly_incrementable<T>;

//输入迭代器
template <class _It>
concept input_iterator = input_or_output_iterator<_It> && indirectly_readable<_It> && requires {
    typename _Iter_concept<_It>;
} && derived_from<_Iter_concept<_It>, input_iterator_tag>;

//输出迭代器
template <class _It, class T>
concept output_iterator = input_or_output_iterator<_It> && indirectly_writable<_It, T>
                       && requires(_It __i, T&& __t) { *__i++ = static_cast<T &&>(__t); };

//前向迭代器
template <class _It>
concept forward_iterator = input_iterator<_It> && derived_from<_Iter_concept<_It>, forward_iterator_tag>
                        && incrementable<_It> && sentinel_for<_It, _It>;

//双向迭代器
template <class _It>
concept bidirectional_iterator =
    forward_iterator<_It> && derived_from<_Iter_concept<_It>, bidirectional_iterator_tag> && requires(_It __i) {
        { --__i } -> same_as<_It&>;
        { __i-- } -> same_as<_It>;
    };

//随机访问迭代器
template <class _It>
concept random_access_iterator =
    bidirectional_iterator<_It> && derived_from<_Iter_concept<_It>, random_access_iterator_tag> && totally_ordered<_It>
    && sized_sentinel_for<_It, _It> && requires(_It __i, const _It __j, const iter_difference_t<_It> __n) {
           { __i += __n } -> same_as<_It&>;
           { __j + __n } -> same_as<_It>;
           { __n + __j } -> same_as<_It>;
           { __i -= __n } -> same_as<_It&>;
           { __j - __n } -> same_as<_It>;
           { __j[__n] } -> same_as<iter_reference_t<_It>>;
       };

//连续迭代器
template <class _It>
concept contiguous_iterator =
    random_access_iterator<_It> && derived_from<_Iter_concept<_It>, contiguous_iterator_tag>
    && is_lvalue_reference_v<iter_reference_t<_It>> && same_as<iter_value_t<_It>, remove_cvref_t<iter_reference_t<_It>>>
    && requires(const _It& __i) {
           { _STD to_address(__i) } -> same_as<add_pointer_t<iter_reference_t<_It>>>;
       };

template <class _FwdIt, class T, bool = contiguous_iterator<_FwdIt>>
constexpr bool _Fill_memset_is_safe = conjunction<is_scalar<T>,
    _Is_character_or_byte_or_bool<_Unwrap_enum_t<remove_reference_t<_Iter_ref_t<_FwdIt>>>>,
    negation<is_volatile<remove_reference_t<_Iter_ref_t<_FwdIt>>>>, is_assignable<_Iter_ref_t<_FwdIt>, const T&>>;

template <class _FwdIt, class T>
constexpr bool _Fill_memset_is_safe<_FwdIt, T, false> = false;
*/
    template<typename Source, typename IterTarget>
    constexpr IterTarget fill(IterTarget begin, IterTarget end, const Source& value) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*begin) == 1) {
          memset(begin, value, end - begin);
          return end;
        } else if (isZero(value)) {
          memset(begin, 0, (const Byte*)end - (const Byte*)begin);
          return end;
        }
      }
      while (begin < end) *begin++ = value;
      return end;
    }

    template<typename Source, typename IterTarget>
    constexpr IterTarget fill(IterTarget iTarget, Size size, const Source& value) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*iTarget) == 1) {
          memset(iTarget, value, size);
          return iTarget + size;
        } else if (isZero(value)) {
          memset(iTarget, 0, size * sizeof(*iTarget));
          return iTarget + size * sizeof(*iTarget);
        }
      }
      while (size--) *iTarget++ = value;
      return iTarget;
    }

    template<typename IterSource, typename IterTarget>
    constexpr IterTarget copy(IterSource begin, IterSource end, IterTarget iTarget) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*begin) == sizeof(*iTarget)) {
          memcpy(iTarget, begin, (const Byte*)end - (const Byte*)begin);
          return iTarget + (end - begin);
        }
      }
      while (begin < end) *iTarget++ = *begin++;
      return iTarget;
    }

    template<typename IterSource, typename IterTarget>
    constexpr IterTarget copy(IterSource iSource, Size size, IterTarget iTarget) noexcept
    {
      if (!isConstEval()) {
        if constexpr (sizeof(*iSource) == sizeof(*iTarget)) {
          memcpy(iTarget, iSource, size * sizeof(*iTarget));
          return iTarget + size;
        }
      }
      while (size--) *iTarget++ = *iSource++;
      return iTarget;
    }
  }
}
