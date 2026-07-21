export module Atl:XUtility;

import :Concepts;
import :Def;
import :Iterator;
import :Type;

import "Macros";

namespace Atl
{
  I256 tailMask32AVX2(Size size) noexcept
  {
    static constexpr UInt32 tailMasks[16]{~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0, 0, 0, 0};
    return _mm256_loadu_si256((I256*)((UInt8*)tailMasks + (32 - size)));
  }

  template <typename Type>
  Size mismatch(const Void* begin1, const Void* begin2, Size size) noexcept
  {
    ZeroupperOnExit guard;
    Size result{};
    Size size_bytes{size * sizeof(Type)};
    Size size_bytes_avx_full{size_bytes & 0xffffffffffffffe0};
    for (; result != size_bytes_avx_full; result += 0x20) {
      if (UInt32 mask{~(UInt32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(
          _mm256_loadu_si256((I256*)((UInt8*)begin1 + result)),
          _mm256_loadu_si256((I256*)((UInt8*)begin2 + result))))}; mask) {
        return (result + _tzcnt_u32(mask)) / sizeof(Type);
      }
    }
    Size size_tail{size_bytes & 0x1C};
    if (size_tail != 0) {
      I256 tailMask{tailMask32AVX2(size_tail)};
      if (UInt32 mask{~(UInt32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(
          _mm256_maskload_epi32((Int32*)((UInt8*)begin1 + result), tailMask),
          _mm256_maskload_epi32((Int32*)((UInt8*)begin2 + result), tailMask)))}; mask) {
        return (result + _tzcnt_u32(mask)) / sizeof(Type);
      }
      result += size_tail;
    }
    result /= sizeof(Type);
    if constexpr (sizeof(Type) >= 4) {
      return result;
    }
    for (; result != size; ++result) {
      if (((Type*)begin1)[result] != ((Type*)begin2)[result]) {
        break;
      }
    }
    return result;
  }

  template <typename Type>
  Size mismatchVectorized(Type* begin1, Type* begin2, Size size) noexcept
  {
    if constexpr (sizeof(Type) % 8 == 0) {
      return mismatch<UInt64>(begin1, begin2, size * (sizeof(Type) >> 3)) / (sizeof(Type) >> 3);
    } else if constexpr (sizeof(Type) % 4 == 0) {
      return mismatch<UInt32>(begin1, begin2, size * (sizeof(Type) >> 2)) / (sizeof(Type) >> 2);
    } else if constexpr (sizeof(Type) % 2 == 0) {
      return mismatch<UInt16>(begin1, begin2, size * (sizeof(Type) >> 1)) / (sizeof(Type) >> 1);
    } else {
      return mismatch<UInt8>(begin1, begin2, size * sizeof(Type)) / sizeof(Type);
    }
  }

  export template <typename Type, typename... Types>
    requires (!isUnboundedArray<Type>) && requires(Type* location, Types&&... arguments) {
      ::new ((void*)location) Type(forward<Types>(arguments)...);
    }
  constexpr Type* constructAt(Type* location, Types&&... arguments) noexcept
  {
    if constexpr (isArray<Type>) {
      ::new ((void*)location) Type();
      return __builtin_launder(location);
    } else {
      return ::new ((void*)location) Type(forward<Types>(arguments)...);
    }
  }

template <typename Type>
concept HasToAddress = requires(const Type& value) {
  typename std::pointer_traits<Type>;
  std::pointer_traits<Type>::to_address(value);
};

export template <typename Type>
[[nodiscard]] constexpr Type* toAddress(Type* value) noexcept
{
  return value;
}

export template <typename Ptr>
[[nodiscard]] constexpr auto toAddress(const Ptr& ptr) noexcept
{
  if constexpr (HasToAddress<Ptr>) {
    return std::pointer_traits<Ptr>::to_address(ptr);
  } else {
    return toAddress(ptr.operator->()); // plain pointer overload must come first
  }
}

export template <typename Type>
concept Incrementable = Regular<Type> && WeaklyIncrementable<Type> && requires(Type t) { { t++ } -> SameAs<Type>; };

template <Bool iteratorCategoryPresent>
struct IteratorConceptImpl2 {
  template <typename Iterator, typename Traits>
  using _Apply = Traits::iterator_category;
};

template <>
struct IteratorConceptImpl2<false> {
  template <typename Iterator, typename Traits> requires FromPrimary<std::iterator_traits<Iterator>>//t
  using _Apply = RandomAccessIteratorTag;
};

template <Bool iteratorConceptPresent>
struct IteratorConceptImpl1 {
  template <typename Iterator, typename Traits>
  using _Apply = Traits::iterator_concept;
};

template <>
struct IteratorConceptImpl1<false> {
  template <typename Iterator, typename Traits>
  using _Apply = IteratorConceptImpl2<HasMemberIteratorCategory<Traits>>::template _Apply<Iterator, Traits>;
};

template <typename Iterator, typename Traits = conditional<FromPrimary<std::iterator_traits<Iterator>>, Iterator, std::iterator_traits<Iterator>>>//t
using IteratorConcept = IteratorConceptImpl1<HasMemberIteratorConcept<Traits>>::template _Apply<Iterator, Traits>;

template <typename Iterator>
concept InputIterator = InputOrOutputIterator<Iterator> && std::indirectly_readable<Iterator> && requires {//t
  typename IteratorConcept<Iterator>;
} && DerivedFrom<IteratorConcept<Iterator>, InputIteratorTag>;

template <typename Iterator, typename Type>
concept OutputIterator = InputOrOutputIterator<Iterator> && std::indirectly_writable<Iterator, Type>//t
                       && requires(Iterator i, Type&& t) { *i++ = (Type &&)t; };

template <typename Iterator>
concept ForwardIterator = InputIterator<Iterator> && DerivedFrom<IteratorConcept<Iterator>, ForwardIteratorTag>
                        && Incrementable<Iterator> && SentinelFor<Iterator, Iterator>;

template <typename Iterator>
concept BidirectionalIterator =
  ForwardIterator<Iterator> && DerivedFrom<IteratorConcept<Iterator>, BidirectionalIteratorTag> && requires(Iterator i) {
    { --i } -> SameAs<Iterator&>;
    { i-- } -> SameAs<Iterator>;
  };

template <typename Iterator>
concept RandomAccessIterator =
    BidirectionalIterator<Iterator> && DerivedFrom<IteratorConcept<Iterator>, RandomAccessIteratorTag> && TotallyOrdered<Iterator>
    && SizedSentinelFor<Iterator, Iterator> && requires(Iterator i, const Iterator j, const iterDifference<Iterator> n) {
           { i += n } -> SameAs<Iterator&>;
           { j + n } -> SameAs<Iterator>;
           { n + j } -> SameAs<Iterator>;
           { i -= n } -> SameAs<Iterator&>;
           { j - n } -> SameAs<Iterator>;
           { j[n] } -> SameAs<iterReference<Iterator>>;
       };

export template <typename Iterator>
concept ContiguousIterator =
  RandomAccessIterator<Iterator> && DerivedFrom<IteratorConcept<Iterator>, ContiguousIteratorTag>
  && isLvalueReference<iterReference<Iterator>> && SameAs<iterValue<Iterator>, removeCVR<iterReference<Iterator>>>
  && requires(const Iterator& i) { { toAddress(i) } -> SameAs<Pointer<iterReference<Iterator>>>; };

template <typename Iter>
using IteratorCategory = typename IteratorTraits<Iter>::IteratorCategory;

template <typename Type>
concept IteratorType = requires { typename IteratorCategory<Type>; };

template <typename Type>
constexpr Bool isIterator{IteratorType<Type>};

template <typename Iter>
constexpr Bool isInputIterator{isConvertible<IteratorCategory<Iter>, InputIteratorTag>};

template <typename Iter>
constexpr Bool isForwardIterator{isConvertible<IteratorCategory<Iter>, ForwardIteratorTag>};

template <typename Iter>
constexpr Bool isBidirectionalIterator{isConvertible<IteratorCategory<Iter>, BidirectionalIteratorTag>};

template <typename Iter>
constexpr Bool isRandomIterator{isConvertible<IteratorCategory<Iter>, RandomAccessIteratorTag>};

template <typename Iter>
constexpr Bool isRangesInputIterator{InputIterator<Iter> && SentinelFor<Iter, Iter> || isInputIterator<Iter>};

template <typename Iter>
constexpr Bool isRangesForwardIterator{ForwardIterator<Iter> || isForwardIterator<Iter>};

template <typename Iter>
constexpr Bool isRangesBidirectionalIterator{BidirectionalIterator<Iter> || isBidirectionalIterator<Iter>};

template <typename Iter>
constexpr Bool isRangesRandomIterator{RandomAccessIterator<Iter> || isRandomIterator<Iter>};

template <Bool isIntegerClass>
struct MakeUnsignedLikeImplementation {
  template <typename Type>
  using _Apply = Type::UnsignedType;
};
template <>
struct MakeUnsignedLikeImplementation<false> {
  template <typename Type>
  using _Apply = UnsignedType<Type>;
};

template <typename Type>
using MakeUnsignedLike = MakeUnsignedLikeImplementation<isIntegerClass<Type>>::template _Apply<Type>;

template <IntegerType Type>
[[nodiscard]] constexpr MakeUnsignedLike<Type> toUnsignedType(Type value) noexcept
{
  return static_cast<MakeUnsignedLike<Type>>(value);
}

export template <typename Iter>
[[nodiscard]] constexpr iterDifference<Iter> distance(Iter begin, Iter end)
{
  if constexpr (isRangesRandomIterator<Iter>) {
    return end - begin;
  } else {
    iterDifference<Iter> offset{};
    while (begin++ != end) {
      ++offset;
    }
    return offset;
  }
}

}
