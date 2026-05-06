export module Atl:Type;

import :Def;

namespace Atl
{
  export template <typename T, T v>
  struct ConstIntegral
  {
    static constexpr T value = v;

    using ValueType = T;
    using Type = ConstIntegral;

    constexpr operator ValueType() const noexcept {
      return value;
    }

    [[nodiscard]] constexpr ValueType operator()() const noexcept {
      return value;
    }
  };

  export template <Bool v>
  using ConstBool = ConstIntegral<Bool, v>;

  export using True  = ConstBool<true>;
  export using False = ConstBool<false>;

  template <Bool v, typename T = Void>
  struct EnableIf {};

  template <typename T>
  struct EnableIf<true, T>
  {
    using Type = T;
  };

  export template <Bool b, typename T = Void>
  using enableIf = EnableIf<b, T>::Type;

  export template <Bool b, typename T1, typename T2>
  struct Conditional
  {
    using Type = T1;
  };

  template <typename T1, typename T2>
  struct Conditional<false, T1, T2>
  {
    using Type = T2;
  };

  export template <Bool b, typename T1, typename T2>
  using conditional = Conditional<b, T1, T2>::Type;

  export template<typename, typename>
  constexpr Bool isSame{false};

  template<typename T>
  constexpr Bool isSame<T, T>{true};

  template<typename T>
  struct ConstRemover
  {
    using Type = T;
  };

  template<typename T>
  struct ConstRemover<const T>
  {
    using Type = T;
  };

  template<typename T>
  struct VolatileRemover
  {
    using Type = T;
  };

  template<typename T>
  struct VolatileRemover<volatile T>
  {
    using Type = T;
  };

  template<typename T>
  struct ConstVolatileRemover
  {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = F<T>;
  };

  template<typename T>
  struct ConstVolatileRemover<const T>
  {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = const F<T>;
  };

  template<typename T>
  struct ConstVolatileRemover<volatile T>
  {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = volatile F<T>;
  };

  template<typename T>
  struct ConstVolatileRemover<const volatile T>
  {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = const volatile F<T>;
  };

  export template<typename T>
  using removeC = ConstRemover<T>::Type;

  export template<typename T>
  using removeV = VolatileRemover<T>::Type;

  export template<typename T>
  using removeCV = ConstVolatileRemover<T>::Type;

  template<typename T, typename... Ts>
  constexpr Bool isAnyOf{(isSame<T, Ts> || ...)};

  [[nodiscard]] constexpr Bool isConstEval() noexcept { return __builtin_is_constant_evaluated(); }

  export template<typename T>
  constexpr Bool isIntegral{isAnyOf<removeCV<T>,
      Bool, Char, CharW, UTF8, UTF16, UTF32, Int8, Uint8, Int16, Uint16, Int32, Uint32, Int64, Uint64>};

template <typename T>
constexpr bool isNonboolIntegral = isIntegral<T> && !isSame<removeCV<T>, Bool>;

  export template<typename T>
  constexpr Bool isFloating{isAnyOf<removeCV<T>, Float, Double>};

  export template <typename T>
  constexpr Bool isArithmetic{isIntegral<T> || isFloating<T>};

  export template <typename T>
  struct ReferenceRemover
  {
    using Type = T;
    using ConstThroughReference = const T;
  };

  template <typename T>
  struct ReferenceRemover<T&>
  {
    using Type = T;
    using ConstThroughReference = const T&;
  };

  template <typename T>
  struct ReferenceRemover<T&&>
  {
    using Type = T;
    using ConstThroughReference = const T&&;
  };

  export template <typename T>
  using removeR = ReferenceRemover<T>::Type;

  export template <typename T>
  using removeCVR [[msvc::known_semantics]] = removeCV<removeR<T>>;

  template <typename T>
  using ConstThroughReference = ReferenceRemover<T>::ConstThroughReference;

  export template <Bool... rest>
  constexpr Bool allTrue{true};

  template <Bool... rest>
  constexpr Bool allTrue<false, rest...>{false};

  template <Bool... rest>
  constexpr Bool allTrue<true, rest...>{allTrue<rest...>};

  export template <typename T>
  constexpr Bool isVoid = isSame<removeCV<T>, Void>;

  export template <typename... Types>
  using Vaild = Void;

  export template <typename T>
  using addConst = const T;

  export template <typename T>
  using addVolatile = volatile T;

  export template <typename T>
  using addConstVolatile = const volatile T;

  template <typename T, typename = Void>
  struct ReferenceAdder
  {
    using Lvalue = T;
    using Rvalue = T;
  };

  template <typename T>
  struct ReferenceAdder<T, Vaild<T&>>
  {
    using Lvalue = T&;
    using Rvalue = T&&;
  };

  export template <typename T>
  using addLvalueReference = ReferenceAdder<T>::Lvalue;

  export template <typename T>
  using addRvalueReference = ReferenceAdder<T>::Rvalue;

  export template <typename T>
  addRvalueReference<T> declvalue() noexcept;

  export template <typename T>
  struct ExtentRemover
  {
    using Type = T;
  };

  template <typename T, Size i>
  struct ExtentRemover<T[i]>
  {
    using Type = T;
  };

  template <typename T>
  struct ExtentRemover<T[]>
  {
    using Type = T;
  };

  export template <typename T>
  using removeExtent = ExtentRemover<T>::Type;

  export template <typename T>
  struct AllExtentsRemover
  {
    using Type = T;
  };

  template <typename T, Size i>
  struct AllExtentsRemover<T[i]>
  {
    using Type = AllExtentsRemover<T>::Type;
  };

  template <typename T>
  struct AllExtentsRemover<T[]>
  {
    using Type = AllExtentsRemover<T>::Type;
  };

  export template <typename T>
  using removeAllExtents = AllExtentsRemover<T>::Type;

  export template <typename T>
  struct PointerRemover
  {
    using Type = T;
  };

  template <typename T>
  struct PointerRemover<T*>
  {
    using Type = T;
  };

  template <typename T>
  struct PointerRemover<T* const>
  {
    using Type = T;
  };

  template <typename T>
  struct PointerRemover<T* volatile>
  {
    using Type = T;
  };

  template <typename T>
  struct PointerRemover<T* const volatile>
  {
    using Type = T;
  };

  export template <typename T>
  using removePointer = PointerRemover<T>::Type;

  template <typename T, typename = Void>
  struct PointerAdded
  {
    using Type = T;
  };

  template <typename T>
  struct PointerAdded<T, Vaild<removeR<T>*>>
  {
    using Type = removeR<T>*;
  };

  export template <typename T>
  using addPointer = PointerAdded<T>::Type;

  export template <typename>
  constexpr Bool isArray{false};

  template <typename T, Size n>
  constexpr Bool isArray<T[n]>{true};

  template <typename T>
  constexpr Bool isArray<T[]>{true};

  export template <typename>
  constexpr Bool isBoundedArray{false};

  template <typename T, Size n>
  constexpr Bool isBoundedArray<T[n]>{true};

  export template <typename>
  constexpr Bool isUnboundedArray{false};

  template <typename T>
  constexpr Bool isUnboundedArray<T[]>{true};

  export template<typename>
  constexpr Bool isLvalueReference{false};

  template<typename T>
  constexpr Bool isLvalueReference<T&>{true};

  export template<typename>
  constexpr Bool isRvalueReference{false};

  template<typename T>
  constexpr Bool isRvalueReference<T&&>{true};

  export template<typename>
  constexpr Bool isReference{false};

  template<typename T>
  constexpr Bool isReference<T&>{true};

  template<typename T>
  constexpr Bool isReference<T&&>{true};

  export template<typename>
  constexpr Bool isPointer{false};

  template<typename T>
  constexpr Bool isPointer<T*>{true};

  template<typename T>
  constexpr Bool isPointer<T* const>{true};

  template<typename T>
  constexpr Bool isPointer<T* volatile>{true};

  template<typename T>
  constexpr Bool isPointer<T* const volatile>{true};

  export template<typename T>
  constexpr Bool isNullPointer{isSame<removeCV<T>, Nullptr>};

  export template <typename T>
  constexpr Bool isUnion{__is_union(T)};

  export template <typename T>
  constexpr Bool isClass{__is_class(T)};

  export template <typename T>
  constexpr Bool isFundamental{isArithmetic<T> || isVoid<T> || isNullPointer<T>};

  export template <typename From, typename To>
  constexpr Bool isConvertible{__is_convertible_to(From, To)};

  export template <typename T>
  constexpr Bool isEnum{__is_enum(T)};

  export template <typename T>
  constexpr Bool isScopedEnum{allTrue<isEnum<T>, !isConvertible<T, int>>};

  export template <typename T>
  constexpr Bool isCompound{!isFundamental<T>};

  export template <typename>
  constexpr Bool isConst{false};

  template <typename T>
  constexpr Bool isConst<const T>{true};

  export template <typename>
  constexpr Bool isVolatile{false};

  template <typename T>
  constexpr Bool isVolatile<volatile T>{true};

  export template <typename T>
  constexpr Bool isFunction{!isConst<const T> && !isReference<T>};

  export template <typename T>
  constexpr Bool isObject{isConst<const T> && !isVoid<T>};

  template<typename T>
  struct MemberPointer: False {};

  template<typename T, typename C>
  struct MemberPointer<T C::*>: True {};

  export template <typename T>
  constexpr Bool isMemberPointer{MemberPointer<removeCV<T>>::value};

  export template <typename T>
  constexpr Bool isMemberFunctionPointer{isMemberPointer<T> && isFunction<removeCV<T>>};

  export template <typename T>
  constexpr Bool isMemberObjectPointer{isMemberPointer<T> && !isFunction<removeCV<T>>};

  export template <typename T>
  constexpr Bool isScalar{isArithmetic<T> || isEnum<T> || isPointer<T> || isMemberPointer<T> || isNullPointer<T>};

  export template <typename T>
  constexpr Bool isEmpty{__is_empty(T)};

  export template <typename T>
  constexpr Bool isPolymorphic{__is_polymorphic(T)};

  export template <typename T>
  constexpr Bool isAbstract{__is_abstract(T)};

  export template <typename T>
  constexpr Bool isFinal{__is_final(T)};

  export template <typename T>
  constexpr Bool isStandardLayout{__is_standard_layout(T)};

  export template <typename T>
  constexpr Bool isTrivial{__is_trivial(T)};

  export template <typename T>
  constexpr Bool isTriviallyCopyable{__is_trivially_copyable(T)};

  export template <typename T>
  constexpr Bool hasVirtualDestructor{__has_virtual_destructor(T)};

  export template <typename T>
  constexpr Bool hasUniqueObjectRepresentations{__has_unique_object_representations(T)};

  export template <typename T>
  constexpr Bool isAggregate{isArray<T> || __is_aggregate(T)};

  export template <typename T, typename... Args>
  constexpr Bool isConstructible{__is_constructible(T, Args...)};

  export template <typename T>
  constexpr Bool isCopyConstructible{__is_constructible(T, addLvalueReference<const T>)};

  export template <typename T>
  constexpr Bool isDefaultConstructible{__is_constructible(T)};

  template <typename T, typename = Void>
  struct ImplicitlyDefaultConstructible: False {};

  template <typename T>
  Void ImplicitlyDefaultConstruct(const T&);

  template <typename T>
  struct ImplicitlyDefaultConstructible<T, Vaild<decltype(ImplicitlyDefaultConstruct<T>({}))>>: True {};

  export template <typename T>
  constexpr Bool isMoveConstructible{__is_constructible(T, T)};

  export template <typename To, typename From>
  constexpr Bool isAssignable{__is_assignable(To, From)};

  template <typename To, typename From>
  constexpr Bool isAssignableNoPreconditionCheck{isAssignable<To, From>};

  export template <typename T>
  constexpr Bool isCopyAssignable{__is_assignable(addLvalueReference<T>, addLvalueReference<const T>)};

  template <typename T>
  constexpr Bool isCopyAssignableNoPreconditionCheck{isCopyAssignable<T>};

  template <typename T>
  constexpr Bool isCopyAssignableUnchecked{isCopyAssignable<T>};

  export template <typename T>
  constexpr Bool isMoveAssignable{__is_assignable(addLvalueReference<T>, T)};

  template <typename T>
  constexpr Bool isMoveAssignableNoPreconditionCheck{isMoveAssignable<T>};

  template <typename T>
  constexpr Bool isMoveAssignableUnchecked{isMoveAssignable<T>};

  export template <typename T>
  constexpr Bool isDestructible{__is_destructible(T)};

  export template <typename T, typename... Args>
  constexpr Bool isTriviallyConstructible{__is_trivially_constructible(T, Args...)};

  export template <typename T>
  constexpr Bool isTriviallyCopyConstructible{__is_trivially_constructible(T, addLvalueReference<const T>)};

  export template <typename T>
  constexpr Bool isTriviallyDefaultConstructible{__is_trivially_constructible(T)};

  export template <typename T>
  constexpr Bool isTriviallyMoveConstructible{__is_trivially_constructible(T, T)};

  export template <typename To, typename From>
  constexpr Bool isTriviallyAssignable{__is_trivially_assignable(To, From)};

  export template <typename T>
  constexpr Bool isTriviallyCopyAssignable{__is_trivially_assignable(addLvalueReference<T>, addLvalueReference<const T>)};

  export template <typename T>
  constexpr Bool isTriviallyMoveAssignable{__is_trivially_assignable(addLvalueReference<T>, T)};

  export template <typename T>
  constexpr Bool isTriviallyDestructible{__is_trivially_destructible(T)};

  export template <typename T, typename... Args>
  constexpr Bool isNothrowConstructible{__is_nothrow_constructible(T, Args...)};

  export template <typename T>
  constexpr Bool isNothrowCopyConstructible{__is_nothrow_constructible(T, addLvalueReference<const T>)};

  export template <typename T>
  constexpr Bool isNothrowDefaultConstructible{__is_nothrow_constructible(T)};

  export template <typename T>
  constexpr Bool isNothrowMoveConstructible{__is_nothrow_constructible(T, T)};

  export template <typename To, typename From>
  constexpr Bool isNothrowAssignable{__is_nothrow_assignable(To, From)};

  export template <typename T>
  constexpr Bool isNothrowCopyAssignable{__is_nothrow_assignable(addLvalueReference<T>, addLvalueReference<const T>)};

  export template <typename T>
  constexpr Bool isNothrowMoveAssignable{__is_nothrow_assignable(addLvalueReference<T>, T)};

  export template <typename T>
  constexpr Bool isNothrowDestructible{__is_nothrow_destructible(T)};

  export template <typename T>
  constexpr Bool isSigned{isIntegral<T> ? removeCV<T>{-1} < removeCV<T>{0} : isFloating<T>};

  export template <typename T>
  constexpr Bool isUnsigned{isIntegral<T> && removeCV<T>{0} < removeCV<T>{-1}};

  //**********************************************************************************************************************************************************************************************************************************
  template <Size>
  struct _Make_signed2; // Choose make_signed strategy by type size

  template <>
  struct _Make_signed2<1> {
    using _Apply = Int8;
  };

  template <>
  struct _Make_signed2<2> {
    using _Apply = Int16;
  };

  template <>
  struct _Make_signed2<4> {
    using _Apply = Int32;
  };

  template <>
  struct _Make_signed2<8> {
    using _Apply = Int64;
  };

  template <typename T>
  using _Make_signed1 = _Make_signed2<sizeof(T)>::_Apply;

  export template <typename T>
  using makeSigned = ConstVolatileRemover<T>::template _Apply<_Make_signed1>; // isNonboolIntegral<T> || isEnum<T>

  template <Size>
  struct _Make_unsigned2;

  template <>
  struct _Make_unsigned2<1> {
    using _Apply = Uint8;
  };

  template <>
  struct _Make_unsigned2<2> {
    using _Apply = Uint16;
  };

  template <>
  struct _Make_unsigned2<4> {
    using _Apply = Uint32;
  };

  template <>
  struct _Make_unsigned2<8> {
    using _Apply = Uint64;
  };

  template <typename T>
  using _Make_unsigned1 = _Make_unsigned2<sizeof(T)>::_Apply;

  export template <typename T>
  using makeUnsigned = ConstVolatileRemover<T>::template _Apply<_Make_unsigned1>; // isNonboolIntegral<T> || isEnum<T>

  export template <typename T>
  constexpr Size alignOf{alignof(T)};

  template <typename T, Bool = isEnum<T>>
  struct UnderlyingType {
  using Type = __underlying_type(T);
  };

  template <typename T>
  struct UnderlyingType<T, false> {};

  export template <typename T>
  using underlyingType = UnderlyingType<T>::Type;

  export template <typename T>
  constexpr Size rank = 0;

  template <typename T, Size n>
  constexpr Size rank<T[n]> = rank<T> + 1;

  template <typename T>
  constexpr Size rank<T[]> = rank<T> + 1;

  export template <typename T, Uint32 i = 0>
  constexpr Size extent = 0;

  template <typename T, Size n>
  constexpr Size extent<T[n], 0> = n;

  template <typename T, Uint32 i, Size n>
  constexpr Size extent<T[n], i> = extent<T, i - 1>;

  template <typename T, Uint32 i>
  constexpr Size extent<T[], i> = extent<T, i - 1>;

  export template <typename Base, typename Derived>
  constexpr Bool isBaseOf = __is_base_of(Base, Derived);

  export template <typename T, typename U = removeR<T>>
  using decay = conditional<isArray<U>, addPointer<removeExtent<U>>, conditional<isFunction<U>, addPointer<U>, removeCV<U>>>;






template <typename T1, typename T2>
using conditionalType = decltype(false ? declvalue<T1>() : declvalue<T2>());

template <typename T1, typename T2>
struct ConstLvalueConditionalOperator {};

template <typename T1, typename T2>
  requires requires { typename conditionalType<const T1&, const T2&>; }
struct ConstLvalueConditionalOperator<T1, T2>
{
  using Type = removeCVR<conditionalType<const T1&, const T2&>>;
};

template <typename T1, typename T2, typename = Void>
struct DecayedConditionalOperator: ConstLvalueConditionalOperator<T1, T2> {};

template <typename T1, typename T2>
struct DecayedConditionalOperator<T1, T2, Vaild<conditionalType<T1, T2>>>
{
  using Type = decay<conditionalType<T1, T2>>;
};

export template <typename... T>
struct CommonType;

export template <typename... T>
using commonType = CommonType<T...>::Type;

template <>
struct CommonType<> {};

template <typename T1>
struct CommonType<T1> : CommonType<T1, T1> {};

template <typename T1, typename T2, typename Decayed1 = decay<T1>, typename Decayed2 = decay<T2>>
struct CommonType2 : CommonType<Decayed1, Decayed2> {};

template <typename T1, typename T2>
struct CommonType2<T1, T2, T1, T2> : DecayedConditionalOperator<T1, T2> {};

template <typename T1, typename T2>
struct CommonType<T1, T2> : CommonType2<T1, T2> {};

template <typename _Void, typename T1, typename T2, typename... Rest>
struct CommonType3 {};

template <typename T1, typename T2, typename... Rest>
struct CommonType3<Vaild<commonType<T1, T2>>, T1, T2, Rest...>
  : CommonType<commonType<T1, T2>, Rest...> {};

template <typename T1, typename T2, typename... Rest>
struct CommonType<T1, T2, Rest...> : CommonType3<void, T1, T2, Rest...> {};

template <typename T>
T returnsExactly() noexcept;

export template <typename, typename, template <typename> typename, template <typename> typename>
struct basic_common_reference {}; // specializations allowed by N5014 [meta.trans.other]/7

template <typename From>
struct _Copy_cv_impl {
  template <typename To>
  using _Apply = To;
};
template <typename From>
struct _Copy_cv_impl<const From> {
  template <typename To>
  using _Apply = const To;
};
template <typename From>
struct _Copy_cv_impl<volatile From> {
  template <typename To>
  using _Apply = volatile To;
};
template <typename From>
struct _Copy_cv_impl<const volatile From> {
  template <typename To>
  using _Apply = const volatile To;
};
template <typename From, typename To>
using _Copy_cv = // N4950 [meta.trans.other]/2.3
  _Copy_cv_impl<From>::template _Apply<To>;

template <typename T1>
struct _Add_qualifiers { // _Add_qualifiers<A>::template _Apply is XREF(A) from N4950 [meta.trans.other]/2.2
  template <typename T2>
  using _Apply = _Copy_cv<T1, T2>;
};
template <typename T1>
struct _Add_qualifiers<T1&> {
  template <typename T2>
  using _Apply = addLvalueReference<_Copy_cv<T1, T2>>;
};
template <typename T1>
struct _Add_qualifiers<T1&&> {
  template <typename T2>
  using _Apply = addRvalueReference<_Copy_cv<T1, T2>>;
};

template <typename T1, typename T2>
using _Cond_res = // N4950 [meta.trans.other]/2.4
  decltype(false ? returnsExactly<T1>() : returnsExactly<T2>());

export template <typename...>
struct common_reference;

export template <typename... Types>
using common_reference_t = common_reference<Types...>::type;

// N4950 [meta.trans.other]/5.1: "If sizeof...(T) is zero ..."
template <>
struct common_reference<> {};

// N4950 [meta.trans.other]/5.2: "...if sizeof...(T) is one ..."
template <typename T>
struct common_reference<T> {
  using type = T;
};

// N4950 [meta.trans.other]/5.3: "...if sizeof...(T) is two..."

// N4950 [meta.trans.other]/5.3.4: "if commonType<T1, T2> is well-formed..."
// N4950 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
template <typename T1, typename T2>
struct _Common_reference2C : CommonType<T1, T2> {};

// N4950 [meta.trans.other]/5.3.3: "if COND_RES(T1, T2) is well-formed..."
template <typename T1, typename T2>
  requires requires { typename _Cond_res<T1, T2>; }
struct _Common_reference2C<T1, T2> {
  using type = _Cond_res<T1, T2>;
};

// N4950 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is well-formed..."
template <typename T1, typename T2>
using _Basic_specialization = basic_common_reference<removeCVR<T1>, removeCVR<T2>,
  _Add_qualifiers<T1>::template _Apply, _Add_qualifiers<T2>::template _Apply>::type;

template <typename T1, typename T2>
struct _Common_reference2B : _Common_reference2C<T1, T2> {};

template <typename T1, typename T2>
  requires requires { typename _Basic_specialization<T1, T2>; }
struct _Common_reference2B<T1, T2> {
  using type = _Basic_specialization<T1, T2>;
};

// N4950 [meta.trans.other]/5.3.1: "Let R be COMMON-REF(T1, T2). If T1 and T2 are reference types, R is well-formed, and
// is_convertible_v<addPointer<T1>, addPointer<R>> && is_convertible_v<addPointer<T2>, addPointer<R>> is
// true, then the member typedef type denotes R."
template <typename T1, typename T2>
struct _Common_reference2A : _Common_reference2B<T1, T2> {};

template <typename T1, typename T2>
requires isLvalueReference<_Cond_res<_Copy_cv<T1, T2>&, _Copy_cv<T2, T1>&>>
using _LL_common_ref = _Cond_res<_Copy_cv<T1, T2>&, _Copy_cv<T2, T1>&>;

template <typename T1, typename T2>
struct _Common_reference2AX {};

template <typename T1, typename T2>
requires requires { typename _LL_common_ref<T1, T2>; }
struct _Common_reference2AX<T1&, T2&> {
  using type = _LL_common_ref<T1, T2>; // "both lvalues" case from N4950 [meta.trans.other]/2.5
};

template <typename T1, typename T2>
  requires isConvertible<T1&&, _LL_common_ref<const T1, T2>>
struct _Common_reference2AX<T1&&, T2&> {
  using type = _LL_common_ref<const T1, T2>; // "rvalue and lvalue" case from N4950 [meta.trans.other]/2.7
};

template <typename T1, typename T2>
  requires isConvertible<T2&&, _LL_common_ref<const T2, T1>>
struct _Common_reference2AX<T1&, T2&&> {
  using type = _LL_common_ref<const T2, T1>; // "lvalue and rvalue" case from N4950 [meta.trans.other]/2.8
};

template <typename T1, typename T2>
using _RR_common_ref = removeR<_LL_common_ref<T1, T2>>&&;

template <typename T1, typename T2>
  requires isConvertible<T1&&, _RR_common_ref<T1, T2>>
        && isConvertible<T2&&, _RR_common_ref<T1, T2>>
struct _Common_reference2AX<T1&&, T2&&> {
  using type = _RR_common_ref<T1, T2>; // "both rvalues" case from N4950 [meta.trans.other]/2.6
};

template <typename T1, typename T2>
using _Common_ref_2AX_t = _Common_reference2AX<T1, T2>::type;

template <typename T1, typename T2>
  requires isConvertible<addPointer<T1>, addPointer<_Common_ref_2AX_t<T1, T2>>>
        && isConvertible<addPointer<T2>, addPointer<_Common_ref_2AX_t<T1, T2>>>
struct _Common_reference2A<T1, T2> {
  using type = _Common_ref_2AX_t<T1, T2>;
};

template <typename T1, typename T2>
struct common_reference<T1, T2> : _Common_reference2A<T1, T2> {};

// N4950 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
template <typename T1, typename T2, typename T3, typename... Rest>
struct common_reference<T1, T2, T3, Rest...> {};
template <typename T1, typename T2, typename T3, typename... Rest>
  requires requires { typename common_reference_t<T1, T2>; }
struct common_reference<T1, T2, T3, Rest...> : common_reference<common_reference_t<T1, T2>, T3, Rest...> {
};

export template <typename T>
struct type_identity {
  using type = T;
};
export template <typename T>
using type_identity_t = type_identity<T>::type;

//1625
}
