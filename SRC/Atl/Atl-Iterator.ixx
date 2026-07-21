export module Atl:Iterator;

import :Common;
import :Def;
import :Utility;

import "Macros";

namespace Atl
{
  template <typename Type, typename AllocType, typename = Void>
  struct HasAllocatorType : False {};

  template <typename Type, typename AllocType>
  struct HasAllocatorType<Type, AllocType, Vaild<typename Type::Alloc>>
    : ConstBool<isConvertible<AllocType, typename Type::Alloc>> {};

  export struct AllocatorArgument
  {
    explicit AllocatorArgument() = default;
  };

  export constexpr AllocatorArgument allocatorArgument{};

  export template <typename Type, typename AllocType>
  constexpr Bool usesAllocator{HasAllocatorType<Type, AllocType>::value};

  struct InputIteratorTag {};

  struct OutputIteratorTag {};

  struct ForwardIteratorTag : InputIteratorTag {};

  struct BidirectionalIteratorTag : ForwardIteratorTag {};

  struct RandomAccessIteratorTag : BidirectionalIteratorTag {};

  struct ContiguousIteratorTag : RandomAccessIteratorTag {};

  template <typename Type>
  using WithReference = Type&;

  template <typename Type>
  concept CanReference = requires { typename WithReference<Type>; };

  template <typename Type>
  concept Dereferenceable = requires(Type& t) { { *t } -> CanReference; };

  template <typename Type>
  concept HasMemberIteratorConcept = requires { typename Type::IteratorConcept; };

  template <typename Type>
  concept HasMemberIteratorCategory = requires { typename Type::IteratorCategory; };

  template <typename Type>
  concept HasMemberValueType = requires { typename Type::ValueType; };

  template <typename Type>
  concept HasMemberElementType = requires { typename Type::ElementType; };

  template <typename Type>
  concept HasMemberDifferenceType = requires { typename Type::DifferenceType; };

  template <typename Type>
  concept HasMemberPointer = requires { typename Type::Pointer; };

  template <typename Type>
  concept HasMemberReference = requires { typename Type::Reference; };

  export template <typename>
  struct IncrementableTraits {};

  template <typename Type> requires isObject<Type>
  struct IncrementableTraits<Type*> {
    using DifferenceType = PtrDiff;
  };

  template <typename Type>
  struct IncrementableTraits<const Type> : IncrementableTraits<Type> {};

  template <HasMemberDifferenceType Type>
  struct IncrementableTraits<Type> {
    using DifferenceType = Type::DifferenceType;
  };

  template <typename Type>
  concept CanDifference = requires(const Type& a, const Type& b) {
    { a - b } -> Integral;
  };

  template <typename Type> requires (!HasMemberDifferenceType<Type> && CanDifference<Type>)
  struct IncrementableTraits<Type> {
    using DifferenceType = SignedType<decltype(declvalue<Type>() - declvalue<Type>())>;
  };

  template <typename Type>
  concept FromPrimary = SameImpl<typename Type::_FromPrimary, Type>;//t

  export template <typename>
  struct IteratorTraits;

  export template <typename Type>
  using iterDifference = conditional<FromPrimary<IteratorTraits<removeCVR<Type>>>,
    IncrementableTraits<removeCVR<Type>>, IteratorTraits<removeCVR<Type>>>::DifferenceType;

  template <typename>
  struct CondValueType {};

  template <typename Type> requires isObject<Type>
  struct CondValueType<Type> {
    using ValueType = removeCV<Type>;
  };

  export template <typename>
  struct IndirectlyReadableTraits {};

  template <typename Type>
  struct IndirectlyReadableTraits<Type*> : CondValueType<Type> {};

  template <typename Type> requires isArray<Type>
  struct IndirectlyReadableTraits<Type> {
    using ValueType = removeCV<removeExtent<Type>>;
  };

  template <typename Type>
  struct IndirectlyReadableTraits<const Type> : IndirectlyReadableTraits<Type> {};

  template <HasMemberValueType Type>
  struct IndirectlyReadableTraits<Type> : CondValueType<typename Type::ValueType> {};

  template <HasMemberElementType Type>
  struct IndirectlyReadableTraits<Type> : CondValueType<typename Type::ElementType> {};

  template <HasMemberValueType Type> requires HasMemberElementType<Type>
  struct IndirectlyReadableTraits<Type> {};

  template <HasMemberValueType Type>
    requires HasMemberElementType<Type> && SameAs<removeCV<typename Type::ValueType>, removeCV<typename Type::ElementType>>
  struct IndirectlyReadableTraits<Type> : CondValueType<typename Type::ValueType> {};

  export template <typename Type>
  using iterValue = conditional<FromPrimary<IteratorTraits<removeCVR<Type>>>,
    IndirectlyReadableTraits<removeCVR<Type>>, IteratorTraits<removeCVR<Type>>>::ValueType;

  export template <Dereferenceable Type>
  using iterReference = decltype(*declvalue<Type&>());

  template <typename>
  struct IteratorTraitsBase {};

  template <typename Iterator>
  concept HasIteratorTypes = HasMemberDifferenceType<Iterator> && HasMemberValueType<Iterator> && HasMemberReference<Iterator>
                          && HasMemberIteratorCategory<Iterator>;

  template <Bool hasMemberTypedef>
  struct OldIterTraitsPointer {
    template <typename Iterator>
    using _Apply = Iterator::pointer;
  };

  template <>
  struct OldIterTraitsPointer<false> {
    template <typename>
    using _Apply = Void;
  };

  template <HasIteratorTypes Iterator>
  struct IteratorTraitsBase<Iterator> {
    using IteratorCategory = Iterator::IteratorCategory;
    using ValueType        = Iterator::ValueType;
    using DifferenceType   = Iterator::DifferenceType;
    using Pointer          = OldIterTraitsPointer<HasMemberPointer<Iterator>>::template _Apply<Iterator>;
    using Reference        = Iterator::Reference;
  };

  template <Bool hasMemberTypedef>
  struct IteratorTraitsDifference {
    template <typename Iterator>
    using _Apply = IncrementableTraits<Iterator>::DifferenceType;
  };

  template <>
  struct IteratorTraitsDifference<false> {
    template <typename>
    using _Apply = Void;
  };

  template <typename Iterator>
  concept Cpp17Iterator = requires(Iterator i) {
    { *i } -> CanReference;
    { ++i } -> SameAs<Iterator&>;
    { *i++ } -> CanReference;
  } && Copyable<Iterator>;

  template <typename Iterator>
  concept Cpp17InputIterator = Cpp17Iterator<Iterator> && EqualityComparable<Iterator>
    && HasMemberDifferenceType<IncrementableTraits<Iterator>> && HasMemberValueType<IndirectlyReadableTraits<Iterator>>
    && requires(Iterator i) {
      typename commonReference<iterReference<Iterator>&&, typename IndirectlyReadableTraits<Iterator>::ValueType&>;
      typename commonReference<decltype(*i++)&&, typename IndirectlyReadableTraits<Iterator>::ValueType&>;
      requires SignedIntegral<typename IncrementableTraits<Iterator>::DifferenceType>;
    };

  template <typename Iterator> requires (!HasIteratorTypes<Iterator> && Cpp17Iterator<Iterator> && !Cpp17InputIterator<Iterator>)
  struct IteratorTraitsBase<Iterator> {
    using IteratorCategory = OutputIteratorTag;
    using ValueType = Void;
    using DifferenceType =
      IteratorTraitsDifference<HasMemberDifferenceType<IncrementableTraits<Iterator>>>::template _Apply<Iterator>;
    using Pointer = Void;
    using Reference = Void;
  };

  enum class IteratorTraitsPointerStrategy { useVoid, useMember, useDecltype };

  template <IteratorTraitsPointerStrategy>
  struct IteratorTraitsPointer;

  template <>
  struct IteratorTraitsPointer<IteratorTraitsPointerStrategy::useVoid> {
      template <typename>
      using _Apply = Void;
  };

  template <>
  struct IteratorTraitsPointer<IteratorTraitsPointerStrategy::useMember> {
      template <typename Iterator>
      using _Apply = Iterator::Pointer;
  };

  template <>
  struct IteratorTraitsPointer<IteratorTraitsPointerStrategy::useDecltype> {
      template <typename Iterator>
      using _Apply = decltype(declvalue<Iterator&>().operator->());
  };

  template <typename Type>
  concept HasMemberArrow = requires(Type&& t) { (Type &&)t.operator->(); };

  template <Bool hasMemberTypedef>
  struct IteratorTraitsReference {
    template <typename Iterator>
    using _Apply = Iterator::Reference;
  };

  template <>
  struct IteratorTraitsReference<false> {
    template <typename Iterator>
    using _Apply = iterReference<Iterator>;
  };

  template <Bool isRandom>
  struct IteratorTraitsCategory4 {
    using Type = RandomAccessIteratorTag;
  };

  template <>
  struct IteratorTraitsCategory4<false> {
    using Type = BidirectionalIteratorTag;
  };

  template <typename Iterator>
  concept Cpp17RandomDelta = TotallyOrdered<Iterator>
    && requires(Iterator i, IncrementableTraits<Iterator>::DifferenceType n) {
      { i += n } -> SameAs<Iterator&>;
      { i -= n } -> SameAs<Iterator&>;
      { i + n } -> SameAs<Iterator>;
      { n + i } -> SameAs<Iterator>;
      { i - n } -> SameAs<Iterator>;
      { i - i } -> SameAs<decltype(n)>;
      { i[n] } -> ConvertibleTo<iterReference<Iterator>>;
    };

  template <Bool isBidi>
  struct IteratorTraitsCategory3 {
    template <typename Iterator>
    using _Apply = IteratorTraitsCategory4<Cpp17RandomDelta<Iterator>>::Type;
  };

  template <>
  struct IteratorTraitsCategory3<false> {
    template <typename>
    using _Apply = ForwardIteratorTag;
  };

  template <typename Iterator>
  concept Cpp17BidiDelta = requires(Iterator i) {
    { --i } -> SameAs<Iterator&>;
    { i-- } -> ConvertibleTo<const Iterator&>;
    requires SameAs<decltype(*i--), iterReference<Iterator>>;
  };

  template <Bool isForward>
  struct IteratorTraitsCategory2 {
    template <typename Iterator>
    using _Apply = IteratorTraitsCategory3<Cpp17BidiDelta<Iterator>>::template _Apply<Iterator>;
  };

  template <>
  struct IteratorTraitsCategory2<false> {
    template <typename>
    using _Apply = InputIteratorTag;
  };

  template <typename Iterator>
  concept Cpp17ForwardDelta =
    ConstructibleFrom<Iterator> && isReference<iterReference<Iterator>>
    && SameAs<removeCVR<iterReference<Iterator>>, typename IndirectlyReadableTraits<Iterator>::ValueType>
    && requires(Iterator i) {
      { i++ } -> ConvertibleTo<const Iterator&>;
      requires SameAs<decltype(*i++), iterReference<Iterator>>;
    };

  template <Bool hasMemberTypedef>
  struct IteratorTraitsCategory {
    template <typename Iterator>
    using _Apply = Iterator::IteratorCategory;
  };

  template <>
  struct IteratorTraitsCategory<false> {
    template <typename Iterator>
    using _Apply = IteratorTraitsCategory2<Cpp17ForwardDelta<Iterator>>::template _Apply<Iterator>;
  };

  template <typename Iterator> requires (!HasIteratorTypes<Iterator> && Cpp17InputIterator<Iterator>)
  struct IteratorTraitsBase<Iterator> {
    using IteratorCategory = IteratorTraitsCategory<HasMemberIteratorCategory<Iterator>>::template _Apply<Iterator>;
    using ValueType        = IndirectlyReadableTraits<Iterator>::ValueType;
    using DifferenceType   = IncrementableTraits<Iterator>::DifferenceType;
    using Pointer =
        IteratorTraitsPointer<(HasMemberPointer<Iterator>  ? IteratorTraitsPointerStrategy::useMember
                              : HasMemberArrow<Iterator&> ? IteratorTraitsPointerStrategy::useDecltype
                                                        : IteratorTraitsPointerStrategy::useVoid)>::template _Apply<Iterator>;
    using Reference = IteratorTraitsReference<HasMemberReference<Iterator>>::template _Apply<Iterator>;
  };

  export template <typename Type>
  struct IteratorTraits : IteratorTraitsBase<Type> {
    using _FromPrimary = IteratorTraits;
  };

  template <typename Type> requires isObject<Type>
  struct IteratorTraits<Type*> {
    using IteratorConcept  = ContiguousIteratorTag;
    using IteratorCategory = RandomAccessIteratorTag;
    using ValueType        = removeCV<Type>;
    using DifferenceType   = PtrDiff;
    using Pointer          = Type*;
    using Reference        = Type&;
  };

  template <typename Type>
  constexpr Bool isIntegerClass{requires {
    typename Type::SignedType;
    typename Type::UnsignedType;
  }};

  template <typename Type>
  concept IntegerType = isNonboolIntegral<Type> || isIntegerClass<Type>;

  template <typename Type>
  concept SignedIntegerType = IntegerType<Type> && (Type)-1 < (Type)0;

  export template <typename Type>
  concept WeaklyIncrementable = Movable<Type> && requires(Type i) {
    typename iterDifference<Type>;
    requires SignedIntegerType<iterDifference<Type>>;
    { ++i } -> SameAs<Type&>;
    i++;
  };

  export template <typename Iterator>
  concept InputOrOutputIterator = requires(Iterator i) { { *i } -> CanReference; } && WeaklyIncrementable<Iterator>;

  export template <typename Sentinel, typename Iterator>
  concept SentinelFor = Semiregular<Sentinel> && InputOrOutputIterator<Iterator> && WeaklyEqualityComparableWith<Sentinel, Iterator>;

  export template <typename Sentinel, typename Iterator>
  constexpr Bool DisableSizedSentinelFor{false};

  export template <typename Sentinel, typename Iterator>
  concept SizedSentinelFor = SentinelFor<Sentinel, Iterator> && !DisableSizedSentinelFor<removeCV<Sentinel>, removeCV<Iterator>>
                            && requires(const Iterator& i, const Sentinel& s) {
                              { s - i } -> SameAs<iterDifference<Iterator>>;
                              { i - s } -> SameAs<iterDifference<Iterator>>;
                            };

  export struct DefaultSentinel {};

  export constexpr DefaultSentinel defaultSentinel{};

  namespace Ranges {
    export enum class SubrangeKind : Bool { unsized, sized };

    export template <InputOrOutputIterator Iterator, SentinelFor<Iterator> Sentinel = Iterator,
      SubrangeKind Ki = SizedSentinelFor<Sentinel, Iterator> ? SubrangeKind::sized : SubrangeKind::unsized>
      requires (Ki == SubrangeKind::sized || !SizedSentinelFor<Sentinel, Iterator>)
    class Subrange;

    export template <Size i, typename Iterator, typename Sentinel, SubrangeKind Ki> 
      requires ((i == 0 && Copyable<Iterator>) || i == 1)
    [[nodiscard]] constexpr auto get(const Subrange<Iterator, Sentinel, Ki>& value);

    export template <Size i, typename Iterator, typename Sentinel, SubrangeKind Ki>
      requires (i < 2)
    [[nodiscard]] constexpr auto get(Subrange<Iterator, Sentinel, Ki>&& value);
  }

  export using Ranges::get;

  /*template <typename>//t
  constexpr Bool isSubrange = false;//t

  template <typename Iterator, typename Sentinel, Ranges::SubrangeKind Ki>
  constexpr Bool isSubrange<Ranges::Subrange<Iterator, Sentinel, Ki>>{true};

  template <typename Iterator, typename Sentinel, Ranges::SubrangeKind Ki>
  struct TupleSize<Ranges::Subrange<Iterator, Sentinel, Ki>> : ConstIntegral<Size, 2> {};

  template <typename Iterator, typename Sentinel, Ranges::SubrangeKind Ki>
  struct TupleElement<0, Ranges::Subrange<Iterator, Sentinel, Ki>> {
    using Type = Iterator;
  };

  template <typename Iterator, typename Sentinel, Ranges::SubrangeKind Ki>
  struct TupleElement<1, Ranges::Subrange<Iterator, Sentinel, Ki>> {
    using Type = Sentinel;
  };

  template <typename Iterator, typename Sentinel, Ranges::SubrangeKind Ki>
  struct TupleElement<0, const Ranges::Subrange<Iterator, Sentinel, Ki>> {
    using Type = Iterator;
  };

  template <typename Iterator, typename Sentinel, Ranges::SubrangeKind Ki>
  struct TupleElement<1, const Ranges::Subrange<Iterator, Sentinel, Ki>> {
    using Type = Sentinel;
  };

  constexpr Size metaNpos = ~Size{};

  constexpr Size metaFindIndex(const Bool* ptr, Size size, Size i = 0)
  {
    for (; i < size; ++i) {
      if (ptr[i]) {
        return i;
      }
    }
    return metaNpos;
  }

  template <typename List, typename T>
  struct MetaFindUniqueIndex {
    using Type = ConstIntegral<Size, metaNpos>;
  };

  template <typename List, typename T>
  using metaFindUniqueIndex = typename MetaFindUniqueIndex<List, T>::Type;

  constexpr Size metaFindUniqueIndex2(const Bool* ptr, Size size, Size first)
  {
    return first != metaNpos && MetaFindUniqueIndex(ptr, size, first + 1) == metaNpos ? first : metaNpos;
  }

  constexpr Size metaFindUniqueIndex1(const Bool* ptr, Size size)
  {
    return metaFindUniqueIndex2(ptr, size, MetaFindUniqueIndex(ptr, size));
  }

  template <template <typename...> typename List, typename First, typename... Rest, typename T>
  struct MetaFindUniqueIndex<List<First, Rest...>, T> {
    static constexpr Bool bools[] = {isSame<First, T>, isSame<Rest, T>...};
    using Type = ConstIntegral<Size, metaFindUniqueIndex1(bools, 1 + sizeof...(Rest))>;
  };*/
}
