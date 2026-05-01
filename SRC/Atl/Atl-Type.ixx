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
  using removeConst = ConstRemover<T>::Type;

  export template<typename T>
  using removeVolatile = VolatileRemover<T>::Type;

  export template<typename T>
  using removeConstVolatile = ConstVolatileRemover<T>::Type;

  template<typename T, typename... Ts>
  constexpr Bool isAnyOf{(isSame<T, Ts> || ...)};

  [[nodiscard]] constexpr Bool isConstEval() noexcept { return __builtin_is_constant_evaluated(); }

  export template<typename T>
  constexpr Bool isIntegral{isAnyOf<removeConstVolatile<T>,
      Bool, Char, CharW, Char8, Char16, Char32, Int8, Uint8, Int16, Uint16, Int32, Uint32, Int64, Uint64>};

template <typename T>
constexpr bool isNonboolIntegral = isIntegral<T> && !isSame<removeConstVolatile<T>, Bool>;

  export template<typename T>
  constexpr Bool isFloatingPoint{isAnyOf<removeConstVolatile<T>, Float, Double>};

  export template <typename T>
  constexpr Bool isArithmetic{isIntegral<T> || isFloatingPoint<T>};

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
  using removeReference = ReferenceRemover<T>::Type;

  template <typename T>
  using ConstThroughReference = ReferenceRemover<T>::ConstThroughReference;

  export template <typename T>
  using removeConstVolatileReference [[msvc::known_semantics]] = removeConstVolatile<removeReference<T>>;

  export template <Bool... rest>
  constexpr Bool allTrue{true};

  template <Bool... rest>
  constexpr Bool allTrue<false, rest...>{false};

  template <Bool... rest>
  constexpr Bool allTrue<true, rest...>{allTrue<rest...>};

  export template <typename T>
  constexpr Bool isVoid = isSame<removeConstVolatile<T>, Void>;

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
  struct PointerAdded<T, Vaild<removeReference<T>*>>
  {
    using Type = removeReference<T>*;
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
  constexpr Bool isNullPointer{isSame<removeConstVolatile<T>, Nullptr>};

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
  constexpr Bool isMemberPointer{MemberPointer<removeConstVolatile<T>>::value};

  export template <typename T>
  constexpr Bool isMemberFunctionPointer{isMemberPointer<T> && isFunction<removeConstVolatile<T>>};

  export template <typename T>
  constexpr Bool isMemberObjectPointer{isMemberPointer<T> && !isFunction<removeConstVolatile<T>>};

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
  constexpr Bool isImplicitLifetime{__builtin_is_implicit_lifetime(T)};

  export template <typename T>
  constexpr Bool hasVirtualDestructor{__has_virtual_destructor(T)};

  export template <typename T>
  constexpr Bool hasUniqueObjectRepresentations{__has_unique_object_representations(T)};

  export template <typename T, typename U>
  constexpr Bool referenceConstructsFromTemporary{__reference_constructs_from_temporary(T, _Adjust_ref_binding_source_t<U>)};

  export template <typename T, typename U>
  constexpr Bool referenceConvertsFromTemporary{__reference_converts_from_temporary(T, _Adjust_ref_binding_source_t<U>)};

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
  constexpr Bool isSigned{isIntegral<T> ? removeConstVolatile<T>{-1} < removeConstVolatile<T>{0} : isFloatingPoint<T>};

  export template <typename T>
  constexpr Bool isUnsigned{isIntegral<T> && removeConstVolatile<T>{0} < removeConstVolatile<T>{-1}};

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
  using makeSigned = ConstVolatileRemover<T>::_Apply<_Make_signed1>; // isNonboolIntegral<T> || isEnum<T>

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
  using makeUnsigned = ConstVolatileRemover<T>::_Apply<_Make_unsigned1>; // isNonboolIntegral<T> || isEnum<T>

  export template <typename T>
  constexpr Size alignOf{alignof(T)};

  template <typename T, Bool = isEnum<T>>
  struct _Underlying_type {
  using Type = __underlying_type(T);
  };

  template <typename T>
  struct _Underlying_type<T, false> {};

  export template <typename T>
  using underlying_type_t = _Underlying_type<T>::Type;

  export template <typename T>
  constexpr Size rank = 0;

  template <typename T, Size n>
  constexpr Size rank<T[n]> = rank<T> + 1;

  template <typename T>
  constexpr Size rank<T[]> = rank<T> + 1;

  export template <typename T, unsigned int i = 0>
  constexpr Size extent = 0;

  template <typename T, Size n>
  constexpr Size extent<T[n], 0> = n;

  template <typename T, unsigned int i, Size n>
  constexpr Size extent<T[n], i> = extent<T, i - 1>;

  template <typename T, unsigned int i>
  constexpr Size extent<T[], i> = extent<T, i - 1>;

  export template <typename Base, typename Derived>
  constexpr bool isBaseOf = __is_base_of(Base, Derived);

  export template <typename T>
  struct decay { // determines decayed version of T
  using T1 = remove_reference_t<T>;
  using T2 = _Select<is_function_v<T1>>::template _Apply<add_pointer<T1>, removeConstVolatile<T1>>;
  using Type = _Select<is_array_v<T1>>::template _Apply<add_pointer<remove_extent_t<T1>>, T2>::type;
  };

  export template <typename T>
  using decay_t = decay<T>::Type;









template <typename T1, typename T2>
using _Conditional_type = decltype(false ? declval<T1>() : declval<T2>());

template <class T1, class T2>
struct _Const_lvalue_cond_oper {};

// N4950 [meta.trans.other]/3.3.4 (per the proposed resolution of LWG-3205): "Otherwise, if remove_cvref_t</**/> denotes
// a type..."
template <class T1, class T2>
    requires requires { typename _Conditional_type<const T1&, const T2&>; }
struct _Const_lvalue_cond_oper<T1, T2> {
    using type = remove_cvref_t<_Conditional_type<const T1&, const T2&>>;
};

template <class T1, class T2, class = void>
struct _Decayed_cond_oper : _Const_lvalue_cond_oper<T1, T2> {};

template <class T1, class T2>
struct _Decayed_cond_oper<T1, T2, Vaild<_Conditional_type<T1, T2>>> {
    using type = decay_t<_Conditional_type<T1, T2>>;
};

export template <typename...T>
struct common_type; // specializations allowed by N5014 [meta.trans.other]/5

export template <typename...T>
using common_type_t = typename common_type<T...>::type;

template <>
struct common_type<> {};

template <class T1>
struct common_type<T1> : common_type<T1, T1> {};

template <class T1, class T2, class _Decayed1 = decay_t<T1>, class _Decayed2 = decay_t<T2>>
struct _Common_type2 : common_type<_Decayed1, _Decayed2> {};

template <class T1, class T2>
struct _Common_type2<T1, T2, T1, T2> : _Decayed_cond_oper<T1, T2> {};

template <class T1, class T2>
struct common_type<T1, T2> : _Common_type2<T1, T2> {};

template <class _Void, class T1, class T2, typename..._Rest>
struct _Common_type3 {};

template <class T1, class T2, typename..._Rest>
struct _Common_type3<Vaild<common_type_t<T1, T2>>, T1, T2, _Rest...>
    : common_type<common_type_t<T1, T2>, _Rest...> {};

template <class T1, class T2, typename..._Rest>
struct common_type<T1, T2, _Rest...> : _Common_type3<void, T1, T2, _Rest...> {};

template <typename T>
T _Returns_exactly() noexcept; // not defined

#if _HAS_CXX20
export template <class, class, template <class> class, template <class> class>
struct basic_common_reference {}; // specializations allowed by N5014 [meta.trans.other]/7

template <class _From>
struct _Copy_cv_impl {
    template <class _To>
    using _Apply = _To;
};
template <class _From>
struct _Copy_cv_impl<const _From> {
    template <class _To>
    using _Apply = const _To;
};
template <class _From>
struct _Copy_cv_impl<volatile _From> {
    template <class _To>
    using _Apply = volatile _To;
};
template <class _From>
struct _Copy_cv_impl<const volatile _From> {
    template <class _To>
    using _Apply = const volatile _To;
};
template <class _From, class _To>
using _Copy_cv = // N4950 [meta.trans.other]/2.3
    _Copy_cv_impl<_From>::template _Apply<_To>;

template <class T1>
struct _Add_qualifiers { // _Add_qualifiers<A>::template _Apply is XREF(A) from N4950 [meta.trans.other]/2.2
    template <class T2>
    using _Apply = _Copy_cv<T1, T2>;
};
template <class T1>
struct _Add_qualifiers<T1&> {
    template <class T2>
    using _Apply = add_lvalue_reference_t<_Copy_cv<T1, T2>>;
};
template <class T1>
struct _Add_qualifiers<T1&&> {
    template <class T2>
    using _Apply = add_rvalue_reference_t<_Copy_cv<T1, T2>>;
};

template <class T1, class T2>
using _Cond_res = // N4950 [meta.trans.other]/2.4
    decltype(false ? _Returns_exactly<T1>() : _Returns_exactly<T2>());

export template <class...>
struct _NO_SPECIALIZATIONS_CITING("N5014 [meta.rqmts]/4; instead, you should specialize std::basic_common_reference "
                                  "as allowed by N5014 [meta.trans.other]/7") common_reference;

export template <typename...Tpes>
using common_reference_t = common_reference<Tpes...>::type;

// N4950 [meta.trans.other]/5.1: "If sizeof...(T) is zero ..."
template <>
struct common_reference<> {};

// N4950 [meta.trans.other]/5.2: "...if sizeof...(T) is one ..."
template <typename T>
struct common_reference<T> {
    using type = T;
};

// N4950 [meta.trans.other]/5.3: "...if sizeof...(T) is two..."

// N4950 [meta.trans.other]/5.3.4: "if common_type_t<T1, T2> is well-formed..."
// N4950 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
template <class T1, class T2>
struct _Common_reference2C : common_type<T1, T2> {};

// N4950 [meta.trans.other]/5.3.3: "if COND_RES(T1, T2) is well-formed..."
template <class T1, class T2>
    requires requires { typename _Cond_res<T1, T2>; }
struct _Common_reference2C<T1, T2> {
    using type = _Cond_res<T1, T2>;
};

// N4950 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is well-formed..."
template <class T1, class T2>
using _Basic_specialization = basic_common_reference<remove_cvref_t<T1>, remove_cvref_t<T2>,
    _Add_qualifiers<T1>::template _Apply, _Add_qualifiers<T2>::template _Apply>::type;

template <class T1, class T2>
struct _Common_reference2B : _Common_reference2C<T1, T2> {};

template <class T1, class T2>
    requires requires { typename _Basic_specialization<T1, T2>; }
struct _Common_reference2B<T1, T2> {
    using type = _Basic_specialization<T1, T2>;
};

// N4950 [meta.trans.other]/5.3.1: "Let R be COMMON-REF(T1, T2). If T1 and T2 are reference types, R is well-formed, and
// is_convertible_v<add_pointer_t<T1>, add_pointer_t<R>> && is_convertible_v<add_pointer_t<T2>, add_pointer_t<R>> is
// true, then the member typedef type denotes R."
template <class T1, class T2>
struct _Common_reference2A : _Common_reference2B<T1, T2> {};

template <class T1, class T2>
    requires is_lvalue_reference_v<_Cond_res<_Copy_cv<T1, T2>&, _Copy_cv<T2, T1>&>>
using _LL_common_ref = _Cond_res<_Copy_cv<T1, T2>&, _Copy_cv<T2, T1>&>;

template <class T1, class T2>
struct _Common_reference2AX {};

template <class T1, class T2>
    requires requires { typename _LL_common_ref<T1, T2>; }
struct _Common_reference2AX<T1&, T2&> {
    using type = _LL_common_ref<T1, T2>; // "both lvalues" case from N4950 [meta.trans.other]/2.5
};

template <class T1, class T2>
    requires is_convertible_v<T1&&, _LL_common_ref<const T1, T2>>
struct _Common_reference2AX<T1&&, T2&> {
    using type = _LL_common_ref<const T1, T2>; // "rvalue and lvalue" case from N4950 [meta.trans.other]/2.7
};

template <class T1, class T2>
    requires is_convertible_v<T2&&, _LL_common_ref<const T2, T1>>
struct _Common_reference2AX<T1&, T2&&> {
    using type = _LL_common_ref<const T2, T1>; // "lvalue and rvalue" case from N4950 [meta.trans.other]/2.8
};

template <class T1, class T2>
using _RR_common_ref = remove_reference_t<_LL_common_ref<T1, T2>>&&;

template <class T1, class T2>
    requires is_convertible_v<T1&&, _RR_common_ref<T1, T2>>
          && is_convertible_v<T2&&, _RR_common_ref<T1, T2>>
struct _Common_reference2AX<T1&&, T2&&> {
    using type = _RR_common_ref<T1, T2>; // "both rvalues" case from N4950 [meta.trans.other]/2.6
};

template <class T1, class T2>
using _Common_ref_2AX_t = _Common_reference2AX<T1, T2>::type;

template <class T1, class T2>
    requires is_convertible_v<add_pointer_t<T1>, add_pointer_t<_Common_ref_2AX_t<T1, T2>>>
          && is_convertible_v<add_pointer_t<T2>, add_pointer_t<_Common_ref_2AX_t<T1, T2>>>
struct _Common_reference2A<T1, T2> {
    using type = _Common_ref_2AX_t<T1, T2>;
};

template <class T1, class T2>
struct common_reference<T1, T2> : _Common_reference2A<T1, T2> {};

// N4950 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
template <class T1, class T2, class T3, typename..._Rest>
struct common_reference<T1, T2, T3, _Rest...> {};
template <class T1, class T2, class T3, typename..._Rest>
    requires requires { typename common_reference_t<T1, T2>; }
struct common_reference<T1, T2, T3, _Rest...> : common_reference<common_reference_t<T1, T2>, T3, _Rest...> {
};

export template <typename T>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS type_identity {
    using type = T;
};
export template <typename T>
using type_identity_t = type_identity<T>::type;
#endif // _HAS_CXX20

template <class Tpe, template <class...> class _Template>
constexpr bool _Is_specialization_v = false; // true if and only if Tpe is a specialization of _Template
template <template <class...> class _Template, typename...Tpes>
constexpr bool _Is_specialization_v<_Template<Tpes...>, _Template> = true;

template <class Tpe, template <class...> class _Template>
struct _Is_specialization : bool_constant<_Is_specialization_v<Tpe, _Template>> {};

export template <typename T>
[[nodiscard]] [[msvc::intrinsic]] constexpr T&& forward(remove_reference_t<T>& _Arg) noexcept {
    return static_cast<T&&>(_Arg);
}

export template <typename T>
[[nodiscard]] [[msvc::intrinsic]] constexpr T&& forward(remove_reference_t<T>&& _Arg) noexcept {
    static_assert(!is_lvalue_reference_v<T>, "bad forward call");
    return static_cast<T&&>(_Arg);
}

export template <typename T>
[[nodiscard]] [[msvc::intrinsic]] constexpr remove_reference_t<T>&& move(T&& _Arg) noexcept {
    return static_cast<remove_reference_t<T>&&>(_Arg);
}

export template <typename T>
[[nodiscard]] [[msvc::intrinsic]] constexpr //
    conditional_t<!is_nothrow_move_constructible_v<T> && is_copy_constructible_v<T>, const T&, T&&>
    move_if_noexcept(T& _Arg) noexcept {
    return _STD move(_Arg);
}

export template <typename T>
[[nodiscard]] constexpr T* addressof(T& _Val) noexcept {
    return __builtin_addressof(_Val);
}

export template <typename T>
const T* addressof(const T&&) = delete;

#pragma warning(push)
#pragma warning(disable : 5215) // '%s' a function parameter with a volatile qualified type is deprecated in C++20
#pragma warning(disable : 5216) // '%s' a volatile qualified return type is deprecated in C++20

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-volatile"
#endif // defined(__clang__)

template <typename T>
[[nodiscard]] T _Fake_copy_init(T) noexcept;
// _Fake_copy_init<T>(E):
// (1) has type T [decay_t<decltype((E))> if T is deduced],
// (2) is well-formed if and only if E is implicitly convertible to T and T is destructible, and
// (3) is non-throwing if and only if both conversion from decltype((E)) to T and destruction of T are non-throwing.

#ifdef __clang__
#pragma clang diagnostic pop
#endif // defined(__clang__)

#pragma warning(pop)

export template <typename T>
class reference_wrapper;

// std::invoke isn't constexpr in C++17, and normally implementers are forbidden from "strengthening" constexpr
// (N4950 [constexpr.functions]/1), yet both std::apply and std::visit are required to be constexpr and have
// invoke-like behavior. As a result, we've chosen to apply the part of P1065R2 resolving LWG-2894 as a defect report.

enum class _Invoker_strategy {
    _Functor,
    _Pmf_object,
    _Pmf_refwrap,
    _Pmf_pointer,
    _Pmd_object,
    _Pmd_refwrap,
    _Pmd_pointer
};

struct _Invoker_functor {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Functor;

    template <class _Callable, typename...Tpes>
    static constexpr auto _Call(_Callable&& _Obj, Tpes&&... _Args)
        noexcept(noexcept(static_cast<_Callable&&>(_Obj)(static_cast<Tpes&&>(_Args)...))) //
        -> decltype(static_cast<_Callable&&>(_Obj)(static_cast<Tpes&&>(_Args)...)) {
        return static_cast<_Callable&&>(_Obj)(static_cast<Tpes&&>(_Args)...);
    }
};

struct _Invoker_pmf_object {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_object;

    template <class _Decayed, class T1, typename...Tpes2>
    static constexpr auto _Call(_Decayed _Pmf, T1&& _Arg1, Tpes2&&... _Args2)
        noexcept(noexcept((static_cast<T1&&>(_Arg1).*_Pmf)(static_cast<Tpes2&&>(_Args2)...))) //
        -> decltype((static_cast<T1&&>(_Arg1).*_Pmf)(static_cast<Tpes2&&>(_Args2)...)) {
        return (static_cast<T1&&>(_Arg1).*_Pmf)(static_cast<Tpes2&&>(_Args2)...);
    }
};

struct _Invoker_pmf_refwrap {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_refwrap;

    template <class _Decayed, class _Refwrap, typename...Tpes2>
    static constexpr auto _Call(_Decayed _Pmf, _Refwrap _Rw, Tpes2&&... _Args2)
        noexcept(noexcept((_Rw.get().*_Pmf)(static_cast<Tpes2&&>(_Args2)...))) //
        -> decltype((_Rw.get().*_Pmf)(static_cast<Tpes2&&>(_Args2)...)) {
        return (_Rw.get().*_Pmf)(static_cast<Tpes2&&>(_Args2)...);
    }
};

struct _Invoker_pmf_pointer {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmf_pointer;

    template <class _Decayed, class T1, typename...Tpes2>
    static constexpr auto _Call(_Decayed _Pmf, T1&& _Arg1, Tpes2&&... _Args2)
        noexcept(noexcept(((*static_cast<T1&&>(_Arg1)).*_Pmf)(static_cast<Tpes2&&>(_Args2)...))) //
        -> decltype(((*static_cast<T1&&>(_Arg1)).*_Pmf)(static_cast<Tpes2&&>(_Args2)...)) {
        return ((*static_cast<T1&&>(_Arg1)).*_Pmf)(static_cast<Tpes2&&>(_Args2)...);
    }
};

struct _Invoker_pmd_object {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_object;

    template <class _Decayed, class T1>
    static constexpr auto _Call(_Decayed _Pmd, T1&& _Arg1) noexcept -> decltype(static_cast<T1&&>(_Arg1).*_Pmd) {
        return static_cast<T1&&>(_Arg1).*_Pmd;
    }
};

struct _Invoker_pmd_refwrap {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_refwrap;

    template <class _Decayed, class _Refwrap>
    static constexpr auto _Call(_Decayed _Pmd, _Refwrap _Rw) noexcept -> decltype(_Rw.get().*_Pmd) {
        return _Rw.get().*_Pmd;
    }
};

struct _Invoker_pmd_pointer {
    static constexpr _Invoker_strategy _Strategy = _Invoker_strategy::_Pmd_pointer;

    template <class _Decayed, class T1>
    static constexpr auto _Call(_Decayed _Pmd, T1&& _Arg1) noexcept(noexcept((*static_cast<T1&&>(_Arg1)).*_Pmd))
        -> decltype((*static_cast<T1&&>(_Arg1)).*_Pmd) {
        return (*static_cast<T1&&>(_Arg1)).*_Pmd;
    }
};

template <class _Callable, class T1, class _Removed_cvref = _Remove_cvref_t<_Callable>,
    bool _Is_pmf = is_member_function_pointer_v<_Removed_cvref>,
    bool _Is_pmd = is_member_object_pointer_v<_Removed_cvref>>
struct _Invoker1;

template <class _Callable, class T1, class _Removed_cvref>
struct _Invoker1<_Callable, T1, _Removed_cvref, true, false>
    : conditional_t<is_same_v<typename _Is_memfunptr<_Removed_cvref>::_Class_type, _Remove_cvref_t<T1>>
                        || is_base_of_v<typename _Is_memfunptr<_Removed_cvref>::_Class_type, _Remove_cvref_t<T1>>,
          _Invoker_pmf_object,
          conditional_t<_Is_specialization_v<_Remove_cvref_t<T1>, reference_wrapper>, _Invoker_pmf_refwrap,
              _Invoker_pmf_pointer>> {}; // pointer to member function

template <class _Callable, class T1, class _Removed_cvref>
struct _Invoker1<_Callable, T1, _Removed_cvref, false, true>
    : conditional_t<
          is_same_v<typename _Is_member_object_pointer<_Removed_cvref>::_Class_type, _Remove_cvref_t<T1>>
              || is_base_of_v<typename _Is_member_object_pointer<_Removed_cvref>::_Class_type, _Remove_cvref_t<T1>>,
          _Invoker_pmd_object,
          conditional_t<_Is_specialization_v<_Remove_cvref_t<T1>, reference_wrapper>, _Invoker_pmd_refwrap,
              _Invoker_pmd_pointer>> {}; // pointer to member data

template <class _Callable, class T1, class _Removed_cvref>
struct _Invoker1<_Callable, T1, _Removed_cvref, false, false> : _Invoker_functor {};

export template <class _Callable>
constexpr auto invoke(_Callable&& _Obj) noexcept(noexcept(static_cast<_Callable&&>(_Obj)()))
    -> decltype(static_cast<_Callable&&>(_Obj)()) {
    return static_cast<_Callable&&>(_Obj)();
}

export template <class _Callable, class T1, typename...Tpes2>
constexpr auto invoke(_Callable&& _Obj, T1&& _Arg1, Tpes2&&... _Args2)
    noexcept(noexcept(_Invoker1<_Callable, T1>::_Call(
        static_cast<_Callable&&>(_Obj), static_cast<T1&&>(_Arg1), static_cast<Tpes2&&>(_Args2)...))) //
    -> decltype(_Invoker1<_Callable, T1>::_Call(
        static_cast<_Callable&&>(_Obj), static_cast<T1&&>(_Arg1), static_cast<Tpes2&&>(_Args2)...)) {
    if constexpr (_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Functor) {
        return static_cast<_Callable&&>(_Obj)(static_cast<T1&&>(_Arg1), static_cast<Tpes2&&>(_Args2)...);
    } else if constexpr (_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Pmf_object) {
        return (static_cast<T1&&>(_Arg1).*_Obj)(static_cast<Tpes2&&>(_Args2)...);
    } else if constexpr (_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Pmf_refwrap) {
        return (_Arg1.get().*_Obj)(static_cast<Tpes2&&>(_Args2)...);
    } else if constexpr (_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Pmf_pointer) {
        return ((*static_cast<T1&&>(_Arg1)).*_Obj)(static_cast<Tpes2&&>(_Args2)...);
    } else if constexpr (_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Pmd_object) {
        return static_cast<T1&&>(_Arg1).*_Obj;
    } else if constexpr (_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Pmd_refwrap) {
#if defined(__clang__) || defined(__EDG__) // TRANSITION, VSO-1956799
        return _Arg1.get().*_Obj;
#else // ^^^ no workaround / workaround vvv
        auto& _Ref = _Arg1.get();
        return _Ref.*_Obj;
#endif // ^^^ workaround ^^^
    } else {
        _STL_INTERNAL_STATIC_ASSERT(_Invoker1<_Callable, T1>::_Strategy == _Invoker_strategy::_Pmd_pointer);
        return (*static_cast<T1&&>(_Arg1)).*_Obj;
    }
}

#pragma warning(push)
#pragma warning(disable : 4242) // '%s': conversion from '%s' to '%s', possible loss of data (/Wall)
#pragma warning(disable : 4244) // '%s': conversion from '%s' to '%s', possible loss of data (Yes, duplicated message.)
#pragma warning(disable : 4365) // '%s': conversion from '%s' to '%s', signed/unsigned mismatch (/Wall)

template <class _From, class _To, bool = is_convertible_v<_From, _To>, bool = is_void_v<_To>>
constexpr bool _Is_nothrow_convertible_v = noexcept(_STD _Fake_copy_init<_To>(_STD declval<_From>()));

#pragma warning(pop)

template <class _From, class _To, bool _IsVoid>
constexpr bool _Is_nothrow_convertible_v<_From, _To, false, _IsVoid> = false;

template <class _From, class _To>
constexpr bool _Is_nothrow_convertible_v<_From, _To, true, true> = true;

template <class _From, class _To>
struct _Is_nothrow_convertible : bool_constant<_Is_nothrow_convertible_v<_From, _To>> {
    // determine whether _From is nothrow-convertible to _To
};

#if _HAS_CXX20
export template <class _From, class _To>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_nothrow_convertible_v = _Is_nothrow_convertible_v<_From, _To>;

export template <class _From, class _To>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_nothrow_convertible
    : bool_constant<_Is_nothrow_convertible_v<_From, _To>> {};
#endif // _HAS_CXX20

template <class _From, class _To, class = void>
struct _Invoke_convertible : false_type {};

#if _HAS_CXX23
#ifdef __cpp_lib_reference_from_temporary // TRANSITION
template <class _To, class _From>
using _Not_reference_converts_from_temporary = bool_constant<!reference_converts_from_temporary_v<_To, _From>>;
#else // ^^^ no workaround / workaround vvv
template <class _To, class _From>
using _Not_reference_converts_from_temporary = true_type;
#endif // ^^^ workaround ^^^
#else // ^^^ _HAS_CXX23 / !_HAS_CXX23 vvv
template <class _To, class _From>
using _Not_reference_converts_from_temporary = true_type;
#endif // ^^^ !_HAS_CXX23 ^^^

template <class _From, class _To>
struct _Invoke_convertible<_From, _To, Vaild<decltype(_STD _Fake_copy_init<_To>(_STD _Returns_exactly<_From>()))>>
    : _Not_reference_converts_from_temporary<_To, _From> {};

template <class _From, class _To>
struct _Invoke_nothrow_convertible
    : bool_constant<noexcept(_STD _Fake_copy_init<_To>(_STD _Returns_exactly<_From>()))> {};

template <class _Result, bool _Nothrow>
struct _Invoke_traits_common {
    using type                  = _Result;
    using _Is_invocable         = true_type;
    using _Is_nothrow_invocable = bool_constant<_Nothrow>;
    template <class _Rx>
    using _Is_invocable_r = bool_constant<disjunction_v<is_void<_Rx>, _Invoke_convertible<type, _Rx>>>;
    template <class _Rx>
    using _Is_nothrow_invocable_r = bool_constant<conjunction_v<_Is_nothrow_invocable,
        disjunction<is_void<_Rx>,
            conjunction<_Invoke_convertible<type, _Rx>, _Invoke_nothrow_convertible<type, _Rx>>>>>;
};

template <class _Void, class _Callable>
struct _Invoke_traits_zero {
    // selected when _Callable isn't callable with zero _Args
    using _Is_invocable         = false_type;
    using _Is_nothrow_invocable = false_type;
    template <class _Rx>
    using _Is_invocable_r = false_type;
    template <class _Rx>
    using _Is_nothrow_invocable_r = false_type;
};

template <class _Callable>
using _Decltype_invoke_zero = decltype(_STD declval<_Callable>()());

template <class _Callable>
struct _Invoke_traits_zero<Vaild<_Decltype_invoke_zero<_Callable>>, _Callable>
    : _Invoke_traits_common<_Decltype_invoke_zero<_Callable>, noexcept(_STD declval<_Callable>()())> {};

template <class _Void, typename...Tpes>
struct _Invoke_traits_nonzero {
    // selected when _Callable isn't callable with nonzero _Args
    using _Is_invocable         = false_type;
    using _Is_nothrow_invocable = false_type;
    template <class _Rx>
    using _Is_invocable_r = false_type;
    template <class _Rx>
    using _Is_nothrow_invocable_r = false_type;
};

template <class _Callable, class T1, typename...Tpes2>
using _Decltype_invoke_nonzero = decltype(_Invoker1<_Callable, T1>::_Call(
    _STD declval<_Callable>(), _STD declval<T1>(), _STD declval<Tpes2>()...));

template <class _Callable, class T1, typename...Tpes2>
struct _Invoke_traits_nonzero<Vaild<_Decltype_invoke_nonzero<_Callable, T1, Tpes2...>>, _Callable, T1,
    Tpes2...> : _Invoke_traits_common<_Decltype_invoke_nonzero<_Callable, T1, Tpes2...>,
                      noexcept(_Invoker1<_Callable, T1>::_Call(
                          _STD declval<_Callable>(), _STD declval<T1>(), _STD declval<Tpes2>()...))> {};

template <class _Callable, typename..._Args>
using _Select_invoke_traits = conditional_t<sizeof...(_Args) == 0, _Invoke_traits_zero<void, _Callable>,
    _Invoke_traits_nonzero<void, _Callable, _Args...>>;

#if _HAS_DEPRECATED_RESULT_OF
export template <class _Fty>
struct _CXX17_DEPRECATE_RESULT_OF _NO_SPECIALIZATIONS_OF_TYPE_TRAITS result_of { // explain usage
    static_assert(_Always_false<_Fty>, "result_of<CallableType> is invalid; use "
                                       "result_of<CallableType(zero or more argument types)> instead.");
};

#define _RESULT_OF(CALL_OPT, X1, X2, X3)                                                                    \
    template <class _Callable, typename..._Args>                                                              \
    struct _CXX17_DEPRECATE_RESULT_OF result_of<_Callable CALL_OPT(_Args...)>                               \
        : _Select_invoke_traits<_Callable, _Args...> { /* template to determine result of call operation */ \
    };

_NON_MEMBER_CALL(_RESULT_OF, X1, X2, X3)
#undef _RESULT_OF

_STL_DISABLE_DEPRECATED_WARNING
export template <typename T>
using result_of_t _CXX17_DEPRECATE_RESULT_OF = typename result_of<T>::type;
_STL_RESTORE_DEPRECATED_WARNING
#endif // _HAS_DEPRECATED_RESULT_OF

template <class _Callable, typename..._Args>
using _Invoke_result_t = typename _Select_invoke_traits<_Callable, _Args...>::type;

template <class _Rx, class _Callable, typename..._Args>
using _Is_invocable_r_ = typename _Select_invoke_traits<_Callable, _Args...>::template _Is_invocable_r<_Rx>;

template <class _Rx, class _Callable, typename..._Args>
struct _Is_invocable_r : _Is_invocable_r_<_Rx, _Callable, _Args...> {
    // determines whether _Callable is callable with _Args and return type _Rx
};

#if _HAS_CXX17
export template <class _Callable, typename..._Args>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS invoke_result : _Select_invoke_traits<_Callable, _Args...> {
    // determine the result type of invoking _Callable with _Args
};

export template <class _Callable, typename..._Args>
using invoke_result_t = typename _Select_invoke_traits<_Callable, _Args...>::type;

export template <class _Callable, typename..._Args>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_invocable : _Select_invoke_traits<_Callable, _Args...>::_Is_invocable {
    // determines whether _Callable is callable with _Args
};

export template <class _Callable, typename..._Args>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_invocable_v =
    _Select_invoke_traits<_Callable, _Args...>::_Is_invocable::value;

export template <class _Callable, typename..._Args>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_nothrow_invocable
    : _Select_invoke_traits<_Callable, _Args...>::_Is_nothrow_invocable {
    // determines whether _Callable is nothrow-callable with _Args
};

export template <class _Callable, typename..._Args>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_nothrow_invocable_v =
    _Select_invoke_traits<_Callable, _Args...>::_Is_nothrow_invocable::value;

export template <class _Rx, class _Callable, typename..._Args>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_invocable_r : _Is_invocable_r_<_Rx, _Callable, _Args...> {
    // determines whether _Callable is callable with _Args and return type _Rx
};

export template <class _Rx, class _Callable, typename..._Args>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_invocable_r_v = _Is_invocable_r_<_Rx, _Callable, _Args...>::value;

export template <class _Rx, class _Callable, typename..._Args>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_nothrow_invocable_r
    : _Select_invoke_traits<_Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx> {
    // determines whether _Callable is nothrow-callable with _Args and return type _Rx
};

export template <class _Rx, class _Callable, typename..._Args>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_nothrow_invocable_r_v =
    _Select_invoke_traits<_Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx>::value;
#endif // _HAS_CXX17

#if _HAS_CXX20
export template <class T1, class T2>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_layout_compatible : bool_constant<__is_layout_compatible(T1, T2)> {};

export template <class T1, class T2>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_layout_compatible_v = __is_layout_compatible(T1, T2);

#ifndef __clang__ // TRANSITION, LLVM-135273
export template <class _Base, class _Derived>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_pointer_interconvertible_base_of
    : bool_constant<__is_pointer_interconvertible_base_of(_Base, _Derived)> {};

export template <class _Base, class _Derived>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_pointer_interconvertible_base_of_v =
    __is_pointer_interconvertible_base_of(_Base, _Derived);
#else // ^^^ no workaround / workaround vvv
export template <class _Base, class _Derived>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_pointer_interconvertible_base_of
    : bool_constant<__is_pointer_interconvertible_base_of(remove_cv_t<_Base>, remove_cv_t<_Derived>)> {};

export template <class _Base, class _Derived>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_pointer_interconvertible_base_of_v =
    __is_pointer_interconvertible_base_of(remove_cv_t<_Base>, remove_cv_t<_Derived>);
#endif // ^^^ workaround ^^^

#ifndef __clang__ // TRANSITION, LLVM-48860
export template <class _ClassTy, class _MemberTy>
[[nodiscard]] _NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_pointer_interconvertible_with_class(
    _MemberTy _ClassTy::* _Pm) noexcept {
    return __is_pointer_interconvertible_with_class(_ClassTy, _Pm);
}

export template <class _ClassTy1, class _ClassTy2, class _MemberTy1, class _MemberTy2>
[[nodiscard]] _NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_corresponding_member(
    _MemberTy1 _ClassTy1::* _Pm1, _MemberTy2 _ClassTy2::* _Pm2) noexcept {
    return __is_corresponding_member(_ClassTy1, _ClassTy2, _Pm1, _Pm2);
}
#endif // ^^^ no workaround ^^^
#endif // _HAS_CXX20

template <typename T>
struct _Function_args {}; // determine whether T is a function

#define _FUNCTION_ARGS(CALL_OPT, CV_OPT, REF_OPT, NOEXCEPT_OPT)                                           \
    template <class _Ret, typename...Tpes>                                                                \
    struct _Function_args<_Ret CALL_OPT(Tpes...) CV_OPT REF_OPT NOEXCEPT_OPT> : _Arg_types<Tpes...> { \
        using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = _Ret;                                 \
    };

_NON_MEMBER_CALL_CV_REF_NOEXCEPT(_FUNCTION_ARGS)
#undef _FUNCTION_ARGS

#define _FUNCTION_ARGS_ELLIPSIS(CV_REF_NOEXCEPT_OPT)                                                            \
    template <class _Ret, typename...Tpes>                                                                      \
    struct _Function_args<_Ret(Tpes..., ...) CV_REF_NOEXCEPT_OPT> { /* no calling conventions for ellipsis */ \
        using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = _Ret;                                       \
    };

_CLASS_DEFINE_CV_REF_NOEXCEPT(_FUNCTION_ARGS_ELLIPSIS)
#undef _FUNCTION_ARGS_ELLIPSIS

template <class T, class = void>
struct _Weak_result_type {}; // default definition

_STL_DISABLE_DEPRECATED_WARNING
template <typename T>
struct _Weak_result_type<T, Vaild<typename T::result_type>> { // defined if T::result_type exists
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = typename T::result_type;
};
_STL_RESTORE_DEPRECATED_WARNING

template <class T, class = void>
struct _Weak_argument_type : _Weak_result_type<T> {}; // default definition

_STL_DISABLE_DEPRECATED_WARNING
template <typename T>
struct _Weak_argument_type<T, Vaild<typename T::argument_type>> : _Weak_result_type<T> {
    // defined if T::argument_type exists
    using _ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = typename T::argument_type;
};
_STL_RESTORE_DEPRECATED_WARNING

template <class T, class = void>
struct _Weak_binary_args : _Weak_argument_type<T> {}; // default definition

_STL_DISABLE_DEPRECATED_WARNING
template <typename T>
struct _Weak_binary_args<T, Vaild<typename T::first_argument_type,
                                  typename T::second_argument_type>>
    : _Weak_argument_type<T> { // defined if both types exist
    using _FIRST_ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS  = typename T::first_argument_type;
    using _SECOND_ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = typename T::second_argument_type;
};
_STL_RESTORE_DEPRECATED_WARNING

template <typename T>
using _Weak_types = conditional_t<is_function_v<remove_pointer_t<T>>, _Function_args<remove_pointer_t<T>>,
    conditional_t<is_member_function_pointer_v<T>, _Is_memfunptr<remove_cv_t<T>>, _Weak_binary_args<T>>>;

template <typename T>
void _Refwrap_ctor_fun(_Identity_t<T&>) noexcept; // not defined
template <typename T>
void _Refwrap_ctor_fun(_Identity_t<T&&>) = delete;

template <class T, class _Uty, class = void>
struct _Refwrap_has_ctor_from : false_type {};

template <class T, class _Uty>
struct _Refwrap_has_ctor_from<T, _Uty, Vaild<decltype(_STD _Refwrap_ctor_fun<T>(_STD declval<_Uty>()))>>
    : true_type {}; // _STD _Refwrap_ctor_fun is qualified: avoid ADL, handle incomplete types

export template <typename T>
class reference_wrapper
#if !_HAS_CXX20
    : public _Weak_types<T>
#endif // !_HAS_CXX20
{
public:
    static_assert(is_object_v<T> || is_function_v<T>,
        "reference_wrapper<T> requires T to be an object type or a function type.");

    using type = T;

    template <class _Uty, enable_if_t<conjunction_v<negation<is_same<_Remove_cvref_t<_Uty>, reference_wrapper>>,
                                          _Refwrap_has_ctor_from<T, _Uty>>,
                              int> = 0>
    _CONSTEXPR20 reference_wrapper(_Uty&& _Val) noexcept(
        noexcept(_STD _Refwrap_ctor_fun<T>(_STD declval<_Uty>()))) { // qualified: avoid ADL, handle incomplete types
        T& _Ref = static_cast<_Uty&&>(_Val);
        _Ptr      = _STD addressof(_Ref);
    }

    _CONSTEXPR20 operator T&() const noexcept {
        return *_Ptr;
    }

    [[nodiscard]] _CONSTEXPR20 T& get() const noexcept {
        return *_Ptr;
    }

private:
    T* _Ptr{};

public:
    template <typename...Tpes>
    _CONSTEXPR20 auto operator()(Tpes&&... _Args) const
        noexcept(noexcept(_STD invoke(*_Ptr, static_cast<Tpes&&>(_Args)...))) //
        -> decltype(_STD invoke(*_Ptr, static_cast<Tpes&&>(_Args)...)) {
        return _STD invoke(*_Ptr, static_cast<Tpes&&>(_Args)...);
    }
};

#if _HAS_CXX17
template <typename T>
reference_wrapper(T&) -> reference_wrapper<T>;
#endif // _HAS_CXX17

export template <typename T>
[[nodiscard]] _CONSTEXPR20 reference_wrapper<T> ref(T& _Val) noexcept {
    return reference_wrapper<T>(_Val);
}

export template <typename T>
void ref(const T&&) = delete;

export template <typename T>
[[nodiscard]] _CONSTEXPR20 reference_wrapper<T> ref(reference_wrapper<T> _Val) noexcept {
    return _Val;
}

export template <typename T>
[[nodiscard]] _CONSTEXPR20 reference_wrapper<const T> cref(const T& _Val) noexcept {
    return reference_wrapper<const T>(_Val);
}

export template <typename T>
void cref(const T&&) = delete;

export template <typename T>
[[nodiscard]] _CONSTEXPR20 reference_wrapper<const T> cref(reference_wrapper<T> _Val) noexcept {
    return _Val;
}

#if _HAS_CXX20
export template <typename T>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS unwrap_reference {
    using type = T;
};
template <typename T>
struct unwrap_reference<reference_wrapper<T>> {
    using type = T&;
};
export template <typename T>
using unwrap_reference_t = unwrap_reference<T>::type;

export template <typename T>
using unwrap_ref_decay_t = unwrap_reference_t<decay_t<T>>;
export template <typename T>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS unwrap_ref_decay {
    using type = unwrap_ref_decay_t<T>;
};
#endif // _HAS_CXX20

template <typename T>
struct _Is_swappable;

template <typename T>
struct _Is_nothrow_swappable;

#if _HAS_CXX17
export template <class T, enable_if_t<is_move_constructible_v<T> && is_move_assignable_v<T>, int> = 0>
#else // ^^^ _HAS_CXX17 / !_HAS_CXX17 vvv
template <class T, int _Enabled = 0>
#endif // ^^^ !_HAS_CXX17 ^^^
_CONSTEXPR20 void swap(T&, T&) noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_move_assignable_v<T>);

export template <class T, size_t _Size, enable_if_t<_Is_swappable<T>::value, int> = 0>
_CONSTEXPR20 void swap(T (&)[_Size], T (&)[_Size]) noexcept(_Is_nothrow_swappable<T>::value);

template <class T1, class T2, class = void>
struct _Swappable_with_helper : false_type {}; // swap(declval<T1>(), declval<T2>()) is not valid

template <class T1, class T2>
struct _Swappable_with_helper<T1, T2, Vaild<decltype(swap(_STD declval<T1>(), _STD declval<T2>()))>>
    : true_type {}; // swap(declval<T1>(), declval<T2>()) is valid

template <class T1, class T2>
struct _Is_swappable_with
    : bool_constant<conjunction_v<_Swappable_with_helper<T1, T2>, _Swappable_with_helper<T2, T1>>> {
    // Determine if expressions with type and value category T1 and T2 can be swapped (and vice versa)
};

template <typename T>
struct _Is_swappable : _Is_swappable_with<add_lvalue_reference_t<T>, add_lvalue_reference_t<T>>::type {
    // Determine if T lvalues satisfy is_swappable_with
};

template <class T1, class T2>
struct _Swap_cannot_throw : bool_constant<noexcept(swap(_STD declval<T1>(), _STD declval<T2>()))
                                          && noexcept(swap(_STD declval<T2>(), _STD declval<T1>()))> {
    // Determine if expressions with type and value category T1 and T2
    // (presumed to satisfy is_swappable_with) can be swapped without emitting exceptions
};

template <class T1, class T2>
struct _Is_nothrow_swappable_with
    : bool_constant<conjunction_v<_Is_swappable_with<T1, T2>, _Swap_cannot_throw<T1, T2>>> {
    // Determine if expressions with type and value category T1 and T2
    // satisfy is_swappable_with, and can be swapped without emitting exceptions
};

template <typename T>
struct _Is_nothrow_swappable
    : _Is_nothrow_swappable_with<add_lvalue_reference_t<T>, add_lvalue_reference_t<T>>::type {
    // Determine if T lvalues satisfy is_nothrow_swappable_with
};

#if _HAS_CXX17
export template <class T1, class T2>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_swappable_with : _Is_swappable_with<T1, T2>::type {
    // Determine if expressions with type and value category T1 and T2
    // can be swapped (and vice versa)
};

export template <class T1, class T2>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_swappable_with_v =
    conjunction_v<_Swappable_with_helper<T1, T2>, _Swappable_with_helper<T2, T1>>;

export template <typename T>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_swappable : _Is_swappable<T>::type {
    // Determine if T lvalues satisfy is_swappable_with
};

export template <typename T>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_swappable_v = _Is_swappable<T>::value;

export template <class T1, class T2>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_nothrow_swappable_with : _Is_nothrow_swappable_with<T1, T2>::type {
    // Determine if expressions with type and value category T1 and T2
    // satisfy is_swappable_with, and can be swapped without emitting exceptions
};

export template <class T1, class T2>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_nothrow_swappable_with_v =
    _Is_nothrow_swappable_with<T1, T2>::value;

export template <typename T>
struct _NO_SPECIALIZATIONS_OF_TYPE_TRAITS is_nothrow_swappable : _Is_nothrow_swappable<T>::type {
    // Determine if T lvalues satisfy is_nothrow_swappable_with
};

export template <typename T>
_NO_SPECIALIZATIONS_OF_TYPE_TRAITS constexpr bool is_nothrow_swappable_v = _Is_nothrow_swappable<T>::value;
#endif // _HAS_CXX17

namespace _Has_ADL_swap_detail {
#if defined(__clang__) || defined(__EDG__) // TRANSITION, VSO-1681199
    void swap() = delete; // Block unqualified name lookup
#else // ^^^ no workaround / workaround vvv
    void swap();
#endif // ^^^ workaround ^^^

    template <class, class = void>
    struct _Has_ADL_swap : false_type {};
    template <typename T>
    struct _Has_ADL_swap<T, Vaild<decltype(swap(_STD declval<T&>(), _STD declval<T&>()))>> : true_type {};
} // namespace _Has_ADL_swap_detail
using _Has_ADL_swap_detail::_Has_ADL_swap;

template <typename T>
constexpr bool _Is_trivially_swappable_v = conjunction_v<is_trivially_destructible<T>,
    is_trivially_move_constructible<T>, is_trivially_move_assignable<T>, negation<_Has_ADL_swap<T>>>;

#ifdef __cpp_lib_byte
template <>
inline constexpr bool _Is_trivially_swappable_v<byte> = true;
#endif // defined(__cpp_lib_byte)

template <typename T>
struct _Is_trivially_swappable : bool_constant<_Is_trivially_swappable_v<T>> {
    // true_type if and only if it is valid to swap two T lvalues by exchanging object representations.
};

#if _HAS_CXX20
export template <class _From, class _To>
concept convertible_to = __is_convertible_to(_From, _To) && requires { static_cast<_To>(_STD declval<_From>()); };

template <class _RefWrap, class T, class _RefWrapQ, class TQ>
concept _Ref_wrap_common_reference_exists_with = _Is_specialization_v<_RefWrap, reference_wrapper> && requires {
    typename common_reference_t<typename _RefWrap::type&, TQ>;
} && convertible_to<_RefWrapQ, common_reference_t<typename _RefWrap::type&, TQ>>;

template <class _RefWrap, class T, template <class> class _RefWrapQual, template <class> class TQual>
    requires (_Ref_wrap_common_reference_exists_with<_RefWrap, T, _RefWrapQual<_RefWrap>, TQual<T>>
              && !_Ref_wrap_common_reference_exists_with<T, _RefWrap, TQual<T>, _RefWrapQual<_RefWrap>>)
struct basic_common_reference<_RefWrap, T, _RefWrapQual, TQual> {
    using type = common_reference_t<typename _RefWrap::type&, TQual<T>>;
};

template <class T, class _RefWrap, template <class> class TQual, template <class> class _RefWrapQual>
    requires (_Ref_wrap_common_reference_exists_with<_RefWrap, T, _RefWrapQual<_RefWrap>, TQual<T>>
              && !_Ref_wrap_common_reference_exists_with<T, _RefWrap, TQual<T>, _RefWrapQual<_RefWrap>>)
struct basic_common_reference<T, _RefWrap, TQual, _RefWrapQual> {
    using type = common_reference_t<typename _RefWrap::type&, TQual<T>>;
};
#endif // _HAS_CXX20

#define _BITMASK_OPS(_MAYBE_EXPORT, _BITMASK)                                                         \
    _MAYBE_EXPORT [[nodiscard]] constexpr _BITMASK operator&(_BITMASK _Left, _BITMASK _Right) noexcept { \
        using _IntTy = _STD underlying_type_t<_BITMASK>;                                              \
        return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) & static_cast<_IntTy>(_Right));       \
    }                                                                                                 \
                                                                                                      \
    _MAYBE_EXPORT [[nodiscard]] constexpr _BITMASK operator|(_BITMASK _Left, _BITMASK _Right) noexcept { \
        using _IntTy = _STD underlying_type_t<_BITMASK>;                                              \
        return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) | static_cast<_IntTy>(_Right));       \
    }                                                                                                 \
                                                                                                      \
    _MAYBE_EXPORT [[nodiscard]] constexpr _BITMASK operator^(_BITMASK _Left, _BITMASK _Right) noexcept { \
        using _IntTy = _STD underlying_type_t<_BITMASK>;                                              \
        return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) ^ static_cast<_IntTy>(_Right));       \
    }                                                                                                 \
                                                                                                      \
    _MAYBE_EXPORT constexpr _BITMASK& operator&=(_BITMASK& _Left, _BITMASK _Right) noexcept {         \
        return _Left = _Left & _Right;                                                                \
    }                                                                                                 \
                                                                                                      \
    _MAYBE_EXPORT constexpr _BITMASK& operator|=(_BITMASK& _Left, _BITMASK _Right) noexcept {         \
        return _Left = _Left | _Right;                                                                \
    }                                                                                                 \
                                                                                                      \
    _MAYBE_EXPORT constexpr _BITMASK& operator^=(_BITMASK& _Left, _BITMASK _Right) noexcept {         \
        return _Left = _Left ^ _Right;                                                                \
    }                                                                                                 \
                                                                                                      \
    _MAYBE_EXPORT [[nodiscard]] constexpr _BITMASK operator~(_BITMASK _Left) noexcept {                  \
        using _IntTy = _STD underlying_type_t<_BITMASK>;                                              \
        return static_cast<_BITMASK>(~static_cast<_IntTy>(_Left));                                    \
    }

template <class _Bitmask>
[[nodiscard]] constexpr bool _Bitmask_includes_any(_Bitmask _Left, _Bitmask _Elements) noexcept {
    return (_Left & _Elements) != _Bitmask{};
}

template <class _Bitmask>
[[nodiscard]] constexpr bool _Bitmask_includes_all(_Bitmask _Left, _Bitmask _Elements) noexcept {
    return (_Left & _Elements) == _Elements;
}

// These FNV-1a utility functions are extremely performance sensitive,
// check examples like that in VSO-653642 before making changes.
#if defined(_WIN64)
_INLINE_VAR constexpr size_t _FNV_offset_basis = 14695981039346656037ULL;
_INLINE_VAR constexpr size_t _FNV_prime        = 1099511628211ULL;
#else // ^^^ defined(_WIN64) / !defined(_WIN64) vvv
_INLINE_VAR constexpr size_t _FNV_offset_basis = 2166136261U;
_INLINE_VAR constexpr size_t _FNV_prime        = 16777619U;
#endif // ^^^ !defined(_WIN64) ^^^

[[nodiscard]] inline size_t _Fnv1a_append_bytes(size_t _Val, const unsigned char* const _First,
    const size_t _Count) noexcept { // accumulate range [_First, _First + _Count) into partial FNV-1a hash _Val
    for (size_t _Idx = 0; _Idx < _Count; ++_Idx) {
        _Val ^= static_cast<size_t>(_First[_Idx]);
        _Val *= _FNV_prime;
    }

    return _Val;
}

template <typename T>
[[nodiscard]] size_t _Fnv1a_append_range(const size_t _Val, const T* const _First,
    const T* const _Last) noexcept { // accumulate range [_First, _Last) into partial FNV-1a hash _Val
    static_assert(is_trivially_copyable_v<T>, "Only trivially copyable types can be directly hashed.");
    const auto _Firstb = reinterpret_cast<const unsigned char*>(_First);
    const auto _Lastb  = reinterpret_cast<const unsigned char*>(_Last);
    return _Fnv1a_append_bytes(_Val, _Firstb, static_cast<size_t>(_Lastb - _Firstb));
}

template <class _Kty>
[[nodiscard]] size_t _Fnv1a_append_value(
    const size_t _Val, const _Kty& _Keyval) noexcept { // accumulate _Keyval into partial FNV-1a hash _Val
    static_assert(is_trivially_copyable_v<_Kty>, "Only trivially copyable types can be directly hashed.");
    return _Fnv1a_append_bytes(_Val, &reinterpret_cast<const unsigned char&>(_Keyval), sizeof(_Kty));
}

template <class _Kty>
[[nodiscard]] size_t _Hash_representation(const _Kty& _Keyval) noexcept { // bitwise hashes the representation of a key
    return _Fnv1a_append_value(_FNV_offset_basis, _Keyval);
}

template <class _Kty>
[[nodiscard]] size_t _Hash_array_representation(
    const _Kty* const _First, const size_t _Count) noexcept { // bitwise hashes the representation of an array
    static_assert(is_trivially_copyable_v<_Kty>, "Only trivially copyable types can be directly hashed.");
    return _Fnv1a_append_bytes(
        _FNV_offset_basis, reinterpret_cast<const unsigned char*>(_First), _Count * sizeof(_Kty));
}

export template <class _Kty>
struct hash;

template <class _Kty, bool _Enabled>
struct _Conditionally_enabled_hash { // conditionally enabled hash base
    using _ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = _Kty;
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS   = size_t;

    [[nodiscard]] _STATIC_CALL_OPERATOR size_t operator()(const _Kty& _Keyval) _CONST_CALL_OPERATOR
        noexcept(noexcept(hash<_Kty>::_Do_hash(_Keyval))) /* strengthened */ {
        return hash<_Kty>::_Do_hash(_Keyval);
    }
};

template <class _Kty>
struct _Conditionally_enabled_hash<_Kty, false> { // conditionally disabled hash base
    _Conditionally_enabled_hash()                                              = delete;
    _Conditionally_enabled_hash(const _Conditionally_enabled_hash&)            = delete;
    _Conditionally_enabled_hash(_Conditionally_enabled_hash&&)                 = delete;
    _Conditionally_enabled_hash& operator=(const _Conditionally_enabled_hash&) = delete;
    _Conditionally_enabled_hash& operator=(_Conditionally_enabled_hash&&)      = delete;
};

export template <class _Kty>
struct hash : _Conditionally_enabled_hash<_Kty, !is_const_v<_Kty> && !is_volatile_v<_Kty>
                                                    && (is_enum_v<_Kty> || is_integral_v<_Kty> || is_pointer_v<_Kty>)> {
    // hash functor primary template (handles enums, integrals, and pointers)
    static size_t _Do_hash(const _Kty& _Keyval) noexcept {
        return _Hash_representation(_Keyval);
    }
};

template <>
struct hash<float> {
    using _ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = float;
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS   = size_t;
    [[nodiscard]] _STATIC_CALL_OPERATOR size_t operator()(const float _Keyval) _CONST_CALL_OPERATOR noexcept {
        return _Hash_representation(_Keyval == 0.0F ? 0.0F : _Keyval); // map -0 to 0
    }
};

template <>
struct hash<double> {
    using _ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = double;
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS   = size_t;
    [[nodiscard]] _STATIC_CALL_OPERATOR size_t operator()(const double _Keyval) _CONST_CALL_OPERATOR noexcept {
        return _Hash_representation(_Keyval == 0.0 ? 0.0 : _Keyval); // map -0 to 0
    }
};

template <>
struct hash<long double> {
    using _ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = long double;
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS   = size_t;
    [[nodiscard]] _STATIC_CALL_OPERATOR size_t operator()(const long double _Keyval) _CONST_CALL_OPERATOR noexcept {
        return _Hash_representation(_Keyval == 0.0L ? 0.0L : _Keyval); // map -0 to 0
    }
};

template <>
struct hash<nullptr_t> {
    using _ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = nullptr_t;
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS   = size_t;
    [[nodiscard]] _STATIC_CALL_OPERATOR size_t operator()(nullptr_t) _CONST_CALL_OPERATOR noexcept {
        void* _Null{};
        return _Hash_representation(_Null);
    }
};

template <class _Kty, class = void>
struct _Is_nothrow_hashable : false_type {}; // tests if std::hash can hash _Kty with noexcept

template <class _Kty>
struct _Is_nothrow_hashable<_Kty, Vaild<decltype(hash<_Kty>{}(_STD declval<const _Kty&>()))>>
    : bool_constant<noexcept(hash<_Kty>{}(_STD declval<const _Kty&>()))> {};

export template <class T = void>
struct less {
    using _FIRST_ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS  = T;
    using _SECOND_ARGUMENT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS = T;
    using _RESULT_TYPE_NAME _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS          = bool;

    [[nodiscard]] constexpr bool operator()(const T& _Left, const T& _Right) const
        noexcept(noexcept(_STD _Fake_copy_init<bool>(_Left < _Right))) /* strengthened */ {
        return _Left < _Right;
    }
};

template <>
struct less<void> {
    template <class T1, class T2>
    [[nodiscard]] constexpr auto operator()(T1&& _Left, T2&& _Right) const
        noexcept(noexcept(static_cast<T1&&>(_Left) < static_cast<T2&&>(_Right))) // strengthened
        -> decltype(static_cast<T1&&>(_Left) < static_cast<T2&&>(_Right)) {
        return static_cast<T1&&>(_Left) < static_cast<T2&&>(_Right);
    }

    using is_transparent = int;
};

// vvvvvvvvvv DERIVED FROM corecrt_internal_fltintrn.h vvvvvvvvvv

template <class _FloatingType>
struct _Floating_type_traits;

template <>
struct _Floating_type_traits<float> {
    static constexpr int32_t _Mantissa_bits           = 24; // FLT_MANT_DIG
    static constexpr int32_t _Exponent_bits           = 8; // sizeof(float) * CHAR_BIT - FLT_MANT_DIG
    static constexpr int32_t _Maximum_binary_exponent = 127; // FLT_MAX_EXP - 1
    static constexpr int32_t _Minimum_binary_exponent = -126; // FLT_MIN_EXP - 1
    static constexpr int32_t _Exponent_bias           = 127;
    static constexpr int32_t _Sign_shift              = 31; // _Exponent_bits + _Mantissa_bits - 1
    static constexpr int32_t _Exponent_shift          = 23; // _Mantissa_bits - 1

    using _Uint_type = uint32_t;

    static constexpr uint32_t _Exponent_mask             = 0x000000FFu; // (1u << _Exponent_bits) - 1
    static constexpr uint32_t _Normal_mantissa_mask      = 0x00FFFFFFu; // (1u << _Mantissa_bits) - 1
    static constexpr uint32_t _Denormal_mantissa_mask    = 0x007FFFFFu; // (1u << (_Mantissa_bits - 1)) - 1
    static constexpr uint32_t _Special_nan_mantissa_mask = 0x00400000u; // 1u << (_Mantissa_bits - 2)
    static constexpr uint32_t _Shifted_sign_mask         = 0x80000000u; // 1u << _Sign_shift
    static constexpr uint32_t _Shifted_exponent_mask     = 0x7F800000u; // _Exponent_mask << _Exponent_shift

    static constexpr float _Minimum_value = 0x1.000000p-126f; // FLT_MIN
    static constexpr float _Maximum_value = 0x1.FFFFFEp+127f; // FLT_MAX
};

template <>
struct _Floating_type_traits<double> {
    static constexpr int32_t _Mantissa_bits           = 53; // DBL_MANT_DIG
    static constexpr int32_t _Exponent_bits           = 11; // sizeof(double) * CHAR_BIT - DBL_MANT_DIG
    static constexpr int32_t _Maximum_binary_exponent = 1023; // DBL_MAX_EXP - 1
    static constexpr int32_t _Minimum_binary_exponent = -1022; // DBL_MIN_EXP - 1
    static constexpr int32_t _Exponent_bias           = 1023;
    static constexpr int32_t _Sign_shift              = 63; // _Exponent_bits + _Mantissa_bits - 1
    static constexpr int32_t _Exponent_shift          = 52; // _Mantissa_bits - 1

    using _Uint_type = uint64_t;

    static constexpr uint64_t _Exponent_mask             = 0x00000000000007FFu; // (1ULL << _Exponent_bits) - 1
    static constexpr uint64_t _Normal_mantissa_mask      = 0x001FFFFFFFFFFFFFu; // (1ULL << _Mantissa_bits) - 1
    static constexpr uint64_t _Denormal_mantissa_mask    = 0x000FFFFFFFFFFFFFu; // (1ULL << (_Mantissa_bits - 1)) - 1
    static constexpr uint64_t _Special_nan_mantissa_mask = 0x0008000000000000u; // 1ULL << (_Mantissa_bits - 2)
    static constexpr uint64_t _Shifted_sign_mask         = 0x8000000000000000u; // 1ULL << _Sign_shift
    static constexpr uint64_t _Shifted_exponent_mask     = 0x7FF0000000000000u; // _Exponent_mask << _Exponent_shift

    static constexpr double _Minimum_value = 0x1.0000000000000p-1022; // DBL_MIN
    static constexpr double _Maximum_value = 0x1.FFFFFFFFFFFFFp+1023; // DBL_MAX
};

template <>
struct _Floating_type_traits<long double> : _Floating_type_traits<double> {};

// ^^^^^^^^^^ DERIVED FROM corecrt_internal_fltintrn.h ^^^^^^^^^^

template <class _To, class _From,
    enable_if_t<conjunction_v<bool_constant<sizeof(_To) == sizeof(_From)>, is_trivially_copyable<_To>,
                    is_trivially_copyable<_From>>,
        int> = 0>
[[nodiscard]] constexpr _To _Bit_cast(const _From& _Val) noexcept {
    return __builtin_bit_cast(_To, _Val);
}

template <class T, enable_if_t<is_floating_point_v<T>, int> = 0>
[[nodiscard]] constexpr auto _Float_abs_bits(const T& _Xx) noexcept {
    using _Traits    = _Floating_type_traits<T>;
    using _Uint_type = typename _Traits::_Uint_type;
    const auto _Bits = _Bit_cast<_Uint_type>(_Xx);
    return _Bits & ~_Traits::_Shifted_sign_mask;
}

template <class T, enable_if_t<is_floating_point_v<T>, int> = 0>
[[nodiscard]] constexpr T _Float_abs(const T _Xx) noexcept { // constexpr floating-point abs()
    return _Bit_cast<T>(_Float_abs_bits(_Xx));
}

template <class T, enable_if_t<is_floating_point_v<T>, int> = 0>
[[nodiscard]] constexpr bool _Is_nan(const T _Xx) noexcept { // constexpr isnan()
    using _Traits = _Floating_type_traits<T>;
    return _Float_abs_bits(_Xx) > _Traits::_Shifted_exponent_mask;
}

template <class T, enable_if_t<is_floating_point_v<T>, int> = 0>
[[nodiscard]] constexpr bool _Is_finite(const T _Xx) noexcept { // constexpr isfinite()
    using _Traits = _Floating_type_traits<T>;
    return _Float_abs_bits(_Xx) < _Traits::_Shifted_exponent_mask;
}

template <bool _IsConst, class T>
using _Maybe_const = conditional_t<_IsConst, const T, T>;

template <class T, size_t _Size>
void _Swap_trivial_arrays(T (&_Left)[_Size], T (&_Right)[_Size]) noexcept {
    // Swap arrays using temporary buffers of 64 bytes, followed by the tail.
    // In optimized builds, the stack usage optimizes away, and vector registers are used.
    // The 64-byte size has desirable properties:
    // * Big enough to engage AVX-512.
    // * Small enough to be handled by SSE2 (using eight registers).
    // * Avoids excessive stack consumption for non-optimized builds.
    auto _Left_ptr  = reinterpret_cast<unsigned char*>(&_Left);
    auto _Right_ptr = reinterpret_cast<unsigned char*>(&_Right);

    constexpr size_t _Part_size_bytes = 64;
    constexpr size_t _Size_bytes      = _Size * sizeof(T);
    constexpr size_t _Size_tail       = _Size_bytes % _Part_size_bytes;
    constexpr size_t _Size_parts      = _Size_bytes - _Size_tail;

    if constexpr (_Size_parts != 0) {
        const auto _Stop = _Left_ptr + _Size_parts;
        do {
            struct _Buffer_type {
                unsigned char _Data[_Part_size_bytes];
            };

            _STL_INTERNAL_STATIC_ASSERT(sizeof(_Buffer_type) == _Part_size_bytes); // assume no padding

            const _Buffer_type _Buffer                   = *reinterpret_cast<const _Buffer_type*>(_Left_ptr);
            *reinterpret_cast<_Buffer_type*>(_Left_ptr)  = *reinterpret_cast<const _Buffer_type*>(_Right_ptr);
            *reinterpret_cast<_Buffer_type*>(_Right_ptr) = _Buffer;
            _Left_ptr += _Part_size_bytes;
            _Right_ptr += _Part_size_bytes;
        } while (_Left_ptr != _Stop);
    }

    if constexpr (_Size_tail != 0) {
        struct _Last_buffer_type {
            unsigned char _Data[_Size_tail];
        };

        _STL_INTERNAL_STATIC_ASSERT(sizeof(_Last_buffer_type) == _Size_tail); // assume no padding

        const _Last_buffer_type _Last_buffer              = *reinterpret_cast<const _Last_buffer_type*>(_Left_ptr);
        *reinterpret_cast<_Last_buffer_type*>(_Left_ptr)  = *reinterpret_cast<const _Last_buffer_type*>(_Right_ptr);
        *reinterpret_cast<_Last_buffer_type*>(_Right_ptr) = _Last_buffer;
    }
}
_STD_END

// TRANSITION, non-_Ugly attribute tokens
#pragma pop_macro("no_specializations")
#pragma pop_macro("known_semantics")
#pragma pop_macro("intrinsic")
#pragma pop_macro("msvc")

#pragma pop_macro("new")
_STL_RESTORE_CLANG_WARNINGS
#pragma warning(pop)
#pragma pack(pop)

#endif // _STL_COMPILER_PREPROCESSOR
#endif // _TYPE_TRAITS_

}
