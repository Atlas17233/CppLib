export module Atl:Concepts;

import :Def;
import :Type;

import "Macros";

namespace Atl
{
  template <typename Type1, typename Type2>
  concept SameImpl = isSame<Type1, Type2>;

  export template <typename Type1, typename Type2>
  concept SameAs = SameImpl<Type1, Type2> && SameImpl<Type2, Type1>;

  export template <typename Derived, typename Base>
  concept DerivedFrom = isBaseOf<Base, Derived> && isConvertible<const volatile Derived*, const volatile Base*>;

  template <typename From, typename To>
  concept ImplicitlyConvertibleTo = isConvertible<From, To>;

  export template <typename Type1, typename Type2>
  concept CommonReferenceWith =
    requires {
      typename commonReference<Type1, Type2>;
      typename commonReference<Type2, Type1>;
    } && SameAs<commonReference<Type1, Type2>, commonReference<Type2, Type1>>
    && ConvertibleTo<Type1, commonReference<Type1, Type2>> && ConvertibleTo<Type2, commonReference<Type1, Type2>>;

  export template <typename Type1, typename Type2>
  concept CommonWith =
    requires {
      typename commonType<Type1, Type2>;
      typename commonType<Type2, Type1>;
    } && SameAs<commonType<Type1, Type2>, commonType<Type2, Type1>>
    && requires {
      (commonType<Type1, Type2>)declvalue<Type1>();
      (commonType<Type1, Type2>)declvalue<Type2>();
    } && CommonReferenceWith<lvalueReference<const Type1>, lvalueReference<const Type2>>
    && CommonReferenceWith<lvalueReference<commonType<Type1, Type2>>,
        commonReference<lvalueReference<const Type1>, lvalueReference<const Type2>>>;

  export template <typename Type>
  concept Integral = isIntegral<Type>;

  export template <typename Type>
  concept SignedIntegral = Integral<Type> && (Type)-1 < (Type)0;

  export template <typename Type>
  concept UnsignedIntegral = Integral<Type> && !SignedIntegral<Type>;

  export template <typename Type>
  concept FloatingPoint = isFloatingPoint<Type>;

  export template <typename LType, typename RType>
  concept AssignableFrom = isLvalueReference<LType> && CommonReferenceWith<const removeR<LType>&, const removeR<RType>&>
    && requires(LType left, RType&& right) { { left = (RType &&)right } -> SameAs<LType>; };

  export template <typename Type>
  concept Destructible = __is_nothrow_destructible(Type);

  export template <typename Type, typename... Types>
  concept ConstructibleFrom = Destructible<Type> && __is_constructible(Type, Types...);

  export template <typename Type>
  concept DefaultInitializable = ConstructibleFrom<Type> && requires {
    Type{};
    ::new ((Void*)nullptr) Type;
  };

  export template <typename Type>
  concept MoveConstructible = ConstructibleFrom<Type, Type> && ConvertibleTo<Type, Type>;

  template <typename Type>
  concept HasClassOrEnumType = __is_class(removeR<Type>) || __is_enum(removeR<Type>) || __is_union(removeR<Type>);

  namespace Ranges {
    namespace _Swap {
      template <typename Type>
      Void swap(Type&, Type&) = delete;

      template <typename Type1, typename Type2>
      concept UseADLSwap = (HasClassOrEnumType<Type1> || HasClassOrEnumType<Type2>)
        && requires(Type1&& t, Type2&& u) {
          swap((Type1&&)t, (Type2&&)u);
        };

      struct Cpo
      {
        template <typename Type1, typename Type2> requires UseADLSwap<Type1, Type2>
        static constexpr Void operator()(Type1&& t, Type2&& u) noexcept
        {
          swap((Type1&&)t, (Type2&&)u);
        }

        template <typename Type> requires (!UseADLSwap<Type&, Type&> && MoveConstructible<Type> && AssignableFrom<Type&, Type>)
        static constexpr Void operator()(Type& x, Type& y) noexcept
        {
          Type tmp((Type&&)x);
          x = (Type&&)y;
          y = (Type&&)tmp;
        }

        template <typename Type1, typename Type2, Size size>
        static constexpr Void operator()(Type1 (&t)[size], Type2 (&u)[size]) noexcept
          requires requires(Cpo fn) { fn(t[0], u[0]); }
        {
          if constexpr (isSame<Type1, Type2> && isTriviallySwappable<Type1>) {
            if !consteval {
              swapTrivialArrays(t, u);
              return;
            }
          }
          for (Size i{}; i < size; ++i) {
            operator()(t[i], u[i]);
          }
        }
      };
    }

    inline namespace _Cpos {
      inline constexpr _Swap::Cpo swap;
    }
  }

  export template <typename Type>
  concept Swappable = requires(Type& x, Type& y) { Ranges::swap(x, y); };

  export template <typename Type1, typename Type2>
  concept SwappableWith = CommonReferenceWith<Type1, Type2> && requires(Type1&& t, Type2&& u) {
    Ranges::swap((Type1&&)t, (Type1&&)t);
    Ranges::swap((Type2&&)u, (Type2&&)u);
    Ranges::swap((Type1&&)t, (Type2&&)u);
    Ranges::swap((Type2&&)u, (Type1&&)t);
  };

  export template <typename Type>
  concept CopyConstructible = MoveConstructible<Type> && ConstructibleFrom<Type, Type&> && ConvertibleTo<Type&, Type>
                           && ConstructibleFrom<Type, const Type&> && ConvertibleTo<const Type&, Type>
                           && ConstructibleFrom<Type, const Type> && ConvertibleTo<const Type, Type>;

  template <typename Type>
  concept BooleanTestableImpl = ConvertibleTo<Type, Bool>;

  template <typename Type>
  concept BooleanTestable = BooleanTestableImpl<Type> && requires(Type&& t) { { !(Type&&)t } -> BooleanTestableImpl; };

  template <typename Type1, typename Type2>
  concept HalfEqualityComparable = requires(const removeR<Type1>& x, const removeR<Type2>& y) {
    { x == y } -> BooleanTestable;
    { x != y } -> BooleanTestable;
  };

  template <typename Type1, typename Type2>
  concept WeaklyEqualityComparableWith = HalfEqualityComparable<Type1, Type2> && HalfEqualityComparable<Type2, Type1>;

  export template <typename Type>
  concept EqualityComparable = HalfEqualityComparable<Type, Type>;

  template <typename Type1, typename Type2, typename Ref = commonReference<const Type1&, const Type2&>>
  concept ComparisonCommonTypeWithImpl = SameAs<Ref, commonReference<const Type2&, const Type1&>> && requires {
    requires ConvertibleTo<const Type1&, const Ref&> || ConvertibleTo<Type1, const Ref&>;
    requires ConvertibleTo<const Type2&, const Ref&> || ConvertibleTo<Type2, const Ref&>;
  };

  template <typename Type1, typename Type2>
  concept ComparisonCommonTypeWith = ComparisonCommonTypeWithImpl<removeCVR<Type1>, removeCVR<Type2>>;

  export template <typename Type1, typename Type2>
  concept EqualityComparableWith =
    EqualityComparable<Type1> && EqualityComparable<Type2> && ComparisonCommonTypeWith<Type1, Type2>
    && EqualityComparable<commonReference<const removeR<Type1>&, const removeR<Type2>&>>
    && WeaklyEqualityComparableWith<Type1, Type2>;

  template <typename Type1, typename Type2>
  concept HalfOrdered = requires(const removeR<Type1>& t, const removeR<Type2>& u) {
    { t < u } -> BooleanTestable;
    { t > u } -> BooleanTestable;
    { t <= u } -> BooleanTestable;
    { t >= u } -> BooleanTestable;
  };

  template <typename Type1, typename Type2>
  concept PartiallyOrderedWith = HalfOrdered<Type1, Type2> && HalfOrdered<Type2, Type1>;

  export template <typename Type>
  concept TotallyOrdered = EqualityComparable<Type> && HalfOrdered<Type, Type>;

  export template <typename Type1, typename Type2>
  concept TotallyOrderedWith =
    TotallyOrdered<Type1> && TotallyOrdered<Type2> && EqualityComparableWith<Type1, Type2>
    && TotallyOrdered<commonReference<const removeR<Type1>&, const removeR<Type2>&>>
    && PartiallyOrderedWith<Type1, Type2>;

  export template <typename Type>
  concept Movable = isObject<Type> && MoveConstructible<Type> && AssignableFrom<Type&, Type> && Swappable<Type>;

  export template <typename Type>
  concept Copyable = CopyConstructible<Type> && Movable<Type> && AssignableFrom<Type&, Type&>
                  && AssignableFrom<Type&, const Type&> && AssignableFrom<Type&, const Type>;

  export template <typename Type>
  concept Semiregular = Copyable<Type> && DefaultInitializable<Type>;

  export template <typename Type>
  concept Regular = Semiregular<Type> && EqualityComparable<Type>;

  export template <typename FTy, typename... Types>
  concept Invocable = requires(
      FTy&& fn, Types&&... values) { std::invoke((FTy&&)fn, (Types&&)values...); };//t

  export template <typename FTy, typename... Types>
  concept RegularInvocable = Invocable<FTy, Types...>;

  export template <typename FTy, typename... Types>
  concept Predicate = RegularInvocable<FTy, Types...> && BooleanTestable<invokeResult<FTy, Types...>>;//t

  export template <typename FTy, typename Type1, typename Type2>
  concept Relation = Predicate<FTy, Type1, Type1> && Predicate<FTy, Type2, Type2> && Predicate<FTy, Type1, Type2>
                  && Predicate<FTy, Type2, Type1>;

  export template <typename FTy, typename Type1, typename Type2>
  concept EquivalenceRelation = Relation<FTy, Type1, Type2>;

  export template <typename FTy, typename Type1, typename Type2>
  concept StrictWeakOrder = Relation<FTy, Type1, Type2>;

  template <typename Type>
  struct Choice {
    Type strategy{};
    Bool noThrow{false};
  };
}
