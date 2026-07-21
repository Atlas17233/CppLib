export module Atl:Common;

import :Def;

import "Macros";

namespace Atl
{
  export template <typename T, T v>
  struct ConstIntegral
  {
    static constexpr T value = v;

    using ValueType = T;
    using Type = ConstIntegral;

    constexpr operator ValueType() const noexcept
    {
      return value;
    }

    [[nodiscard]] constexpr ValueType operator()() const noexcept
    {
      return value;
    }
  };

  export template <Bool value>
  using ConstBool = ConstIntegral<Bool, value>;

  export using True  = ConstBool<true>;
  export using False = ConstBool<false>;

  template <Bool value, typename Type = Void>
  struct EnableIf {};

  template <typename T>
  struct EnableIf<true, T>
  {
    using Type = T;
  };

  export template <Bool value, typename Type = Void>
  using enableIf = EnableIf<value, Type>::Type;

  export template <Bool value, typename Type1, typename Type2>
  struct Conditional
  {
    using Type = Type1;
  };

  template <typename Type1, typename Type2>
  struct Conditional<false, Type1, Type2>
  {
    using Type = Type2;
  };

  export template <Bool value, typename Type1, typename Type2>
  using conditional = Conditional<value, Type1, Type2>::Type;

  export template <typename, typename>
  constexpr Bool isSame{false};

  template <typename Type>
  constexpr Bool isSame<Type, Type>{true};

  template <typename T>
  struct RemoveConst
  {
    using Type = T;
  };

  template <typename T>
  struct RemoveConst<const T>
  {
    using Type = T;
  };

  template <typename T>
  struct RemoveVolatile
  {
    using Type = T;
  };

  template <typename T>
  struct RemoveVolatile<volatile T>
  {
    using Type = T;
  };

  template <typename T>
  struct RemoveConstVolatile
  {
    using Type = T;

    template <template <typename> typename Function>
    using _Apply = Function<T>;
  };

  template <typename T>
  struct RemoveConstVolatile<const T>
  {
    using Type = T;

    template <template <typename> typename Function>
    using _Apply = const Function<T>;
  };

  template <typename T>
  struct RemoveConstVolatile<volatile T>
  {
    using Type = T;

    template <template <typename> typename Function>
    using _Apply = volatile Function<T>;
  };

  template <typename T>
  struct RemoveConstVolatile<const volatile T>
  {
    using Type = T;

    template <template <typename> typename Function>
    using _Apply = const volatile Function<T>;
  };

  export template <typename Type>
  using removeC = RemoveConst<Type>::Type;

  export template <typename Type>
  using removeV = RemoveVolatile<Type>::Type;

  export template <typename Type>
  using removeCV = RemoveConstVolatile<Type>::Type;

  template <typename Type, typename... Types>
  constexpr Bool isAnyOf{(isSame<Type, Types> || ...)};

  template <typename Type>
  constexpr Bool isSignedIntegral{isAnyOf<removeCV<Type>, Char, Int8, Int16, Int32, Int64>};

  template <typename Type>
  constexpr Bool isUnsignedIntegral{isAnyOf<removeCV<Type>, Bool, CharW, UTF8, UTF16, UTF32, UInt8, UInt16, UInt32, UInt64>};

  export template <typename Type>
  constexpr Bool isIntegral{isSignedIntegral<Type> || isUnsignedIntegral<Type>};

  template <typename Type>
  constexpr Bool isNonboolIntegral = isIntegral<Type> && !isSame<removeCV<Type>, Bool>;

  export template <typename Type>
  constexpr Bool isFloatingPoint{isAnyOf<removeCV<Type>, Float, Double>};

  export template <typename Type>
  constexpr Bool isArithmetic{isIntegral<Type> || isFloatingPoint<Type>};

  export template <typename T>
  struct RemoveReference
  {
    using Type = T;
    using ConstThroughReference = const T;
  };

  template <typename T>
  struct RemoveReference<T&>
  {
    using Type = T;
    using ConstThroughReference = const T&;
  };

  template <typename T>
  struct RemoveReference<T&&>
  {
    using Type = T;
    using ConstThroughReference = const T&&;
  };

  export template <typename Type>
  using removeR = RemoveReference<Type>::Type;

  export template <typename Type>
  using removeCVR [[msvc::known_semantics]] = removeCV<removeR<Type>>;

  template <typename Type>
  using ConstThroughReference = RemoveReference<Type>::ConstThroughReference;
}
