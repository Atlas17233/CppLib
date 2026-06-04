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
  constexpr Bool isFloating{isAnyOf<removeCV<Type>, Float, Double>};

  export template <typename Type>
  constexpr Bool isArithmetic{isIntegral<Type> || isFloating<Type>};

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

  export template <typename Type>
  constexpr Bool isVoid = isSame<removeCV<Type>, Void>;

  export template <typename... Types>
  using Vaild = Void;

  export template <typename Type>
  using Const = const Type;

  export template <typename Type>
  using Volatile = volatile Type;

  export template <typename Type>
  using ConstVolatile = const volatile Type;

  template <typename Type, typename = Void>
  struct AddReference
  {
    using Lvalue = Type;
    using Rvalue = Type;
  };

  template <typename Type>
  struct AddReference<Type, Vaild<Type&>>
  {
    using Lvalue = Type&;
    using Rvalue = Type&&;
  };

  export template <typename Type>
  using lvalueReference = AddReference<Type>::Lvalue;

  export template <typename Type>
  using rvalueReference = AddReference<Type>::Rvalue;

  export template <typename Type>
  rvalueReference<Type> declvalue() noexcept;

  export template <typename T>
  struct RemoveExtent
  {
    using Type = T;
  };

  template <typename T, Size i>
  struct RemoveExtent<T[i]>
  {
    using Type = T;
  };

  template <typename T>
  struct RemoveExtent<T[]>
  {
    using Type = T;
  };

  export template <typename Type>
  using removeExtent = RemoveExtent<Type>::Type;

  export template <typename T>
  struct RemoveAllExtents
  {
    using Type = T;
  };

  template <typename T, Size i>
  struct RemoveAllExtents<T[i]>
  {
    using Type = RemoveAllExtents<T>::Type;
  };

  template <typename T>
  struct RemoveAllExtents<T[]>
  {
    using Type = RemoveAllExtents<T>::Type;
  };

  export template <typename Type>
  using removeAllExtents = RemoveAllExtents<Type>::Type;

  export template <typename T>
  struct RemovePointer
  {
    using Type = T;
  };

  template <typename T>
  struct RemovePointer<T*>
  {
    using Type = T;
  };

  template <typename T>
  struct RemovePointer<T* const>
  {
    using Type = T;
  };

  template <typename T>
  struct RemovePointer<T* volatile>
  {
    using Type = T;
  };

  template <typename T>
  struct RemovePointer<T* const volatile>
  {
    using Type = T;
  };

  export template <typename Type>
  using removePointer = RemovePointer<Type>::Type;

  template <typename T, typename = Void>
  struct AddPointer
  {
    using Type = T;
  };

  template <typename T>
  struct AddPointer<T, Vaild<removeR<T>*>>
  {
    using Type = removeR<T>*;
  };

  export template <typename Type>
  using Pointer = AddPointer<Type>::Type;

  export template <typename>
  constexpr Bool isArray{false};

  template <typename Type, Size n>
  constexpr Bool isArray<Type[n]>{true};

  template <typename Type>
  constexpr Bool isArray<Type[]>{true};

  export template <typename>
  constexpr Bool isBoundedArray{false};

  template <typename Type, Size n>
  constexpr Bool isBoundedArray<Type[n]>{true};

  export template <typename>
  constexpr Bool isUnboundedArray{false};

  template <typename Type>
  constexpr Bool isUnboundedArray<Type[]>{true};

  export template <typename>
  constexpr Bool isLvalueReference{false};

  template <typename Type>
  constexpr Bool isLvalueReference<Type&>{true};

  export template <typename>
  constexpr Bool isRvalueReference{false};

  template <typename Type>
  constexpr Bool isRvalueReference<Type&&>{true};

  export template <typename>
  constexpr Bool isReference{false};

  template <typename Type>
  constexpr Bool isReference<Type&>{true};

  template <typename Type>
  constexpr Bool isReference<Type&&>{true};

  export template <typename>
  constexpr Bool isPointer{false};

  template <typename Type>
  constexpr Bool isPointer<Type*>{true};

  template <typename Type>
  constexpr Bool isPointer<Type* const>{true};

  template <typename Type>
  constexpr Bool isPointer<Type* volatile>{true};

  template <typename Type>
  constexpr Bool isPointer<Type* const volatile>{true};

  export template <typename Type>
  constexpr Bool isNullptr{isSame<removeCV<Type>, Nullptr>};

  export template <typename Type>
  constexpr Bool isUnion{__is_union(Type)};

  export template <typename Type>
  constexpr Bool isClass{__is_class(Type)};

  export template <typename Type>
  constexpr Bool isFundamental{isArithmetic<Type> || isVoid<Type> || isNullptr<Type>};

  export template <typename From, typename To>
  constexpr Bool isConvertible{__is_convertible_to(From, To)};

  export template <typename Type>
  constexpr Bool isEnum{__is_enum(Type)};

  export template <typename Type>
  constexpr Bool isScopedEnum{isEnum<Type> && !isConvertible<Type, Int>};

  export template <typename Type>
  constexpr Bool isCompound{!isFundamental<Type>};

  export template <typename>
  constexpr Bool isConst{false};

  template <typename Type>
  constexpr Bool isConst<const Type>{true};

  export template <typename>
  constexpr Bool isVolatile{false};

  template <typename Type>
  constexpr Bool isVolatile<volatile Type>{true};

  export template <typename Type>
  constexpr Bool isFunction{!isConst<const Type> && !isReference<Type>};

  export template <typename Type>
  constexpr Bool isObject{isConst<const Type> && !isVoid<Type>};

  template <typename Type>
  struct MemberPointer: False {};

  template <typename Type, typename C>
  struct MemberPointer<Type C::*>: True {
    using Class = C;
  };

  export template <typename Type>
  constexpr Bool isMemberPointer{MemberPointer<removeCV<Type>>::value};

  export template <typename Type>
  constexpr Bool isMemberFunctionPointer{isMemberPointer<Type> && isFunction<removeCV<Type>>};

  export template <typename Type>
  constexpr Bool isMemberObjectPointer{isMemberPointer<Type> && !isFunction<removeCV<Type>>};

  export template <typename Type>
  constexpr Bool isScalar{isArithmetic<Type> || isEnum<Type> || isPointer<Type> || isMemberPointer<Type> || isNullptr<Type>};

  export template <typename Type>
  constexpr Bool isEmpty{__is_empty(Type)};

  export template <typename Type>
  constexpr Bool isPolymorphic{__is_polymorphic(Type)};

  export template <typename Type>
  constexpr Bool isAbstract{__is_abstract(Type)};

  export template <typename Type>
  constexpr Bool isFinal{__is_final(Type)};

  export template <typename Type>
  constexpr Bool isStandardLayout{__is_standard_layout(Type)};

  export template <typename Type>
  constexpr Bool isTrivial{__is_trivial(Type)};

  export template <typename Type>
  constexpr Bool isTriviallyCopyable{__is_trivially_copyable(Type)};

  export template <typename Type>
  constexpr Bool hasVirtualDestructor{__has_virtual_destructor(Type)};

  export template <typename Type>
  constexpr Bool hasUniqueObjectRepresentations{__has_unique_object_representations(Type)};

  export template <typename Type>
  constexpr Bool isAggregate{isArray<Type> || __is_aggregate(Type)};

  export template <typename Type, typename... Args>
  constexpr Bool isConstructible{__is_constructible(Type, Args...)};

  export template <typename Type>
  constexpr Bool isCopyConstructible{__is_constructible(Type, lvalueReference<const Type>)};

  export template <typename Type>
  constexpr Bool isDefaultConstructible{__is_constructible(Type)};

  template <typename Type>
  Void ImplicitlyDefaultConstruct(const Type&);

  template<typename Type>
  concept isImplicitlyDefaultConstructible = requires { ImplicitlyDefaultConstruct<Type>({}); };

  export template <typename Type>
  constexpr Bool isMoveConstructible{__is_constructible(Type, Type)};

  export template <typename To, typename From>
  constexpr Bool isAssignable{__is_assignable(To, From)};

  template <typename To, typename From>
  constexpr Bool isAssignableNoPreconditionCheck{isAssignable<To, From>};

  export template <typename Type>
  constexpr Bool isCopyAssignable{__is_assignable(lvalueReference<Type>, lvalueReference<const Type>)};

  template <typename Type>
  constexpr Bool isCopyAssignableNoPreconditionCheck{isCopyAssignable<Type>};

  template <typename Type>
  constexpr Bool isCopyAssignableUnchecked{isCopyAssignable<Type>};

  export template <typename Type>
  constexpr Bool isMoveAssignable{__is_assignable(lvalueReference<Type>, Type)};

  template <typename Type>
  constexpr Bool isMoveAssignableNoPreconditionCheck{isMoveAssignable<Type>};

  template <typename Type>
  constexpr Bool isMoveAssignableUnchecked{isMoveAssignable<Type>};

  export template <typename Type>
  constexpr Bool isDestructible{__is_destructible(Type)};

  export template <typename Type, typename... Args>
  constexpr Bool isTriviallyConstructible{__is_trivially_constructible(Type, Args...)};

  export template <typename Type>
  constexpr Bool isTriviallyCopyConstructible{__is_trivially_constructible(Type, lvalueReference<const Type>)};

  export template <typename Type>
  constexpr Bool isTriviallyDefaultConstructible{__is_trivially_constructible(Type)};

  export template <typename Type>
  constexpr Bool isTriviallyMoveConstructible{__is_trivially_constructible(Type, Type)};

  export template <typename To, typename From>
  constexpr Bool isTriviallyAssignable{__is_trivially_assignable(To, From)};

  export template <typename Type>
  constexpr Bool isTriviallyCopyAssignable{__is_trivially_assignable(lvalueReference<Type>, lvalueReference<const Type>)};

  export template <typename Type>
  constexpr Bool isTriviallyMoveAssignable{__is_trivially_assignable(lvalueReference<Type>, Type)};

  export template <typename Type>
  constexpr Bool isTriviallyDestructible{__is_trivially_destructible(Type)};

  export template <typename Type, typename... Args>
  constexpr Bool isNothrowConstructible{__is_nothrow_constructible(Type, Args...)};

  export template <typename Type>
  constexpr Bool isNothrowCopyConstructible{__is_nothrow_constructible(Type, lvalueReference<const Type>)};

  export template <typename Type>
  constexpr Bool isNothrowDefaultConstructible{__is_nothrow_constructible(Type)};

  export template <typename Type>
  constexpr Bool isNothrowMoveConstructible{__is_nothrow_constructible(Type, Type)};

  export template <typename To, typename From>
  constexpr Bool isNothrowAssignable{__is_nothrow_assignable(To, From)};

  export template <typename Type>
  constexpr Bool isNothrowCopyAssignable{__is_nothrow_assignable(lvalueReference<Type>, lvalueReference<const Type>)};

  export template <typename Type>
  constexpr Bool isNothrowMoveAssignable{__is_nothrow_assignable(lvalueReference<Type>, Type)};

  export template <typename Type>
  constexpr Bool isNothrowDestructible{__is_nothrow_destructible(Type)};

  export template <typename Type>
  constexpr Bool isSigned{isSignedIntegral<Type> || isFloating<Type>};

  export template <typename Type>
  constexpr Bool isUnsigned{isUnsignedIntegral<Type>};

  template <typename Type>
  constexpr Bool isStandardUnsigned{isAnyOf<Type, UInt8, UInt16, UInt32, UInt64>};

  template <typename Type>
  concept StandardUnsignedIntegral = isStandardUnsigned<Type>;

  template <Size>
  struct MakeSigned;

  template <>
  struct MakeSigned<1> {
    using Type = Int8;
  };

  template <>
  struct MakeSigned<2> {
    using Type = Int16;
  };

  template <>
  struct MakeSigned<4> {
    using Type = Int32;
  };

  template <>
  struct MakeSigned<8> {
    using Type = Int64;
  };

  export template <typename Type>
  using SignedType = MakeSigned<sizeof(removeCV<Type>)>::Type; // isNonboolIntegral<T> || isEnum<T>

  template <Size>
  struct MakeUnsigned;

  template <>
  struct MakeUnsigned<1> {
    using Type = UInt8;
  };

  template <>
  struct MakeUnsigned<2> {
    using Type = UInt16;
  };

  template <>
  struct MakeUnsigned<4> {
    using Type = UInt32;
  };

  template <>
  struct MakeUnsigned<8> {
    using Type = UInt64;
  };

  export template <typename Type>
  using UnsignedType = MakeUnsigned<sizeof(removeCV<Type>)>::Type; // isNonboolIntegral<T> || isEnum<T>

  template <typename Type>
  constexpr UnsignedType<Type> unsignedValue(Type value) {
    return (UnsignedType<Type>)value;
  }

  export template <typename Type>
  constexpr Size alignOf{alignof(Type)};

  template <typename T, Bool = isEnum<T>>
  struct UnderlyingType {
    using Type = __underlying_type(T);
  };

  template <typename Type>
  struct UnderlyingType<Type, false> {};

  export template <typename Type>
  using underlyingType = UnderlyingType<Type>::Type;

  export template <typename Type>
  constexpr Size rank = 0;

  template <typename Type, Size n>
  constexpr Size rank<Type[n]> = rank<Type> + 1;

  template <typename Type>
  constexpr Size rank<Type[]> = rank<Type> + 1;

  export template <typename Type, UInt32 i = 0>
  constexpr Size extent = 0;

  template <typename Type, Size n>
  constexpr Size extent<Type[n], 0> = n;

  template <typename Type, UInt32 i, Size n>
  constexpr Size extent<Type[n], i> = extent<Type, i - 1>;

  template <typename Type, UInt32 i>
  constexpr Size extent<Type[], i> = extent<Type, i - 1>;

  export template <typename Base, typename Derived>
  constexpr Bool isBaseOf = __is_base_of(Base, Derived);

  export template <typename T, typename U = removeR<T>>
  using decay = conditional<isArray<U>, Pointer<removeExtent<U>>, conditional<isFunction<U>, Pointer<U>, removeCV<U>>>;

  template <typename Type1, typename Type2>
  using conditionalType = decltype(false ? declvalue<Type1>() : declvalue<Type2>());

  template <typename Type1, typename Type2>
  struct ConstLvalueConditionalOperator {};

  template <typename Type1, typename Type2>
  requires requires { typename conditionalType<const Type1&, const Type2&>; }
  struct ConstLvalueConditionalOperator<Type1, Type2>
  {
    using Type = removeCVR<conditionalType<const Type1&, const Type2&>>;
  };

  template <typename Type1, typename Type2, typename = Void>
  struct DecayedConditionalOperator: ConstLvalueConditionalOperator<Type1, Type2> {};

  template <typename Type1, typename Type2>
  struct DecayedConditionalOperator<Type1, Type2, Vaild<conditionalType<Type1, Type2>>>
  {
    using Type = decay<conditionalType<Type1, Type2>>;
  };

  export template <typename... Types>
  struct CommonType;

  export template <typename... Types>
  using commonType = CommonType<Types...>::Type;

  template <>
  struct CommonType<> {};

  template <typename Type1>
  struct CommonType<Type1> : CommonType<Type1, Type1> {};

template <typename Type1, typename Type2, typename Decayed1 = decay<Type1>, typename Decayed2 = decay<Type2>>
struct CommonType2 : CommonType<Decayed1, Decayed2> {};

template <typename Type1, typename Type2>
struct CommonType2<Type1, Type2, Type1, Type2> : DecayedConditionalOperator<Type1, Type2> {};

template <typename Type1, typename Type2>
struct CommonType<Type1, Type2> : CommonType2<Type1, Type2> {};

template <typename _Void, typename Type1, typename Type2, typename... Rest>
struct CommonType3 {};

template <typename Type1, typename Type2, typename... Rest>
struct CommonType3<Vaild<commonType<Type1, Type2>>, Type1, Type2, Rest...>
  : CommonType<commonType<Type1, Type2>, Rest...> {};

template <typename Type1, typename Type2, typename... Rest>
struct CommonType<Type1, Type2, Rest...> : CommonType3<void, Type1, Type2, Rest...> {};

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

template <typename Type1>
struct _Add_qualifiers { // _Add_qualifiers<A>::template _Apply is XREF(A) from N4950 [meta.trans.other]/2.2
  template <typename Type2>
  using _Apply = _Copy_cv<Type1, Type2>;
};
template <typename Type1>
struct _Add_qualifiers<Type1&> {
  template <typename Type2>
  using _Apply = lvalueReference<_Copy_cv<Type1, Type2>>;
};
template <typename Type1>
struct _Add_qualifiers<Type1&&> {
  template <typename Type2>
  using _Apply = rvalueReference<_Copy_cv<Type1, Type2>>;
};

template <typename Type1, typename Type2>
using _Cond_res = // N4950 [meta.trans.other]/2.4
  decltype(false ? returnsExactly<Type1>() : returnsExactly<Type2>());

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

// N4950 [meta.trans.other]/5.3.4: "if commonType<Type1, Type2> is well-formed..."
// N4950 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
template <typename Type1, typename Type2>
struct _Common_reference2C : CommonType<Type1, Type2> {};

// N4950 [meta.trans.other]/5.3.3: "if COND_RES(Type1, Type2) is well-formed..."
template <typename Type1, typename Type2>
  requires requires { typename _Cond_res<Type1, Type2>; }
struct _Common_reference2C<Type1, Type2> {
  using type = _Cond_res<Type1, Type2>;
};

// N4950 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is well-formed..."
template <typename Type1, typename Type2>
using _Basic_specialization = basic_common_reference<removeCVR<Type1>, removeCVR<Type2>,
  _Add_qualifiers<Type1>::template _Apply, _Add_qualifiers<Type2>::template _Apply>::type;

template <typename Type1, typename Type2>
struct _Common_reference2B : _Common_reference2C<Type1, Type2> {};

template <typename Type1, typename Type2>
  requires requires { typename _Basic_specialization<Type1, Type2>; }
struct _Common_reference2B<Type1, Type2> {
  using type = _Basic_specialization<Type1, Type2>;
};

// N4950 [meta.trans.other]/5.3.1: "Let R be COMMON-REF(Type1, Type2). If Type1 and Type2 are reference types, R is well-formed, and
// is_convertible_v<Pointer<Type1>, Pointer<R>> && is_convertible_v<Pointer<Type2>, Pointer<R>> is
// true, then the member typedef type denotes R."
template <typename Type1, typename Type2>
struct _Common_reference2A : _Common_reference2B<Type1, Type2> {};

template <typename Type1, typename Type2>
requires isLvalueReference<_Cond_res<_Copy_cv<Type1, Type2>&, _Copy_cv<Type2, Type1>&>>
using _LL_common_ref = _Cond_res<_Copy_cv<Type1, Type2>&, _Copy_cv<Type2, Type1>&>;

template <typename Type1, typename Type2>
struct _Common_reference2AX {};

template <typename Type1, typename Type2>
requires requires { typename _LL_common_ref<Type1, Type2>; }
struct _Common_reference2AX<Type1&, Type2&> {
  using type = _LL_common_ref<Type1, Type2>; // "both lvalues" case from N4950 [meta.trans.other]/2.5
};

template <typename Type1, typename Type2>
  requires isConvertible<Type1&&, _LL_common_ref<const Type1, Type2>>
struct _Common_reference2AX<Type1&&, Type2&> {
  using type = _LL_common_ref<const Type1, Type2>; // "rvalue and lvalue" case from N4950 [meta.trans.other]/2.7
};

template <typename Type1, typename Type2>
  requires isConvertible<Type2&&, _LL_common_ref<const Type2, Type1>>
struct _Common_reference2AX<Type1&, Type2&&> {
  using type = _LL_common_ref<const Type2, Type1>; // "lvalue and rvalue" case from N4950 [meta.trans.other]/2.8
};

template <typename Type1, typename Type2>
using _RR_common_ref = removeR<_LL_common_ref<Type1, Type2>>&&;

template <typename Type1, typename Type2>
  requires isConvertible<Type1&&, _RR_common_ref<Type1, Type2>>
        && isConvertible<Type2&&, _RR_common_ref<Type1, Type2>>
struct _Common_reference2AX<Type1&&, Type2&&> {
  using type = _RR_common_ref<Type1, Type2>; // "both rvalues" case from N4950 [meta.trans.other]/2.6
};

template <typename Type1, typename Type2>
using _Common_ref_2AX_t = _Common_reference2AX<Type1, Type2>::type;

template <typename Type1, typename Type2>
  requires isConvertible<Pointer<Type1>, Pointer<_Common_ref_2AX_t<Type1, Type2>>>
        && isConvertible<Pointer<Type2>, Pointer<_Common_ref_2AX_t<Type1, Type2>>>
struct _Common_reference2A<Type1, Type2> {
  using type = _Common_ref_2AX_t<Type1, Type2>;
};

template <typename Type1, typename Type2>
struct common_reference<Type1, Type2> : _Common_reference2A<Type1, Type2> {};

// N4950 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
template <typename Type1, typename Type2, typename T3, typename... Rest>
struct common_reference<Type1, Type2, T3, Rest...> {};
template <typename Type1, typename Type2, typename T3, typename... Rest>
  requires requires { typename common_reference_t<Type1, Type2>; }
struct common_reference<Type1, Type2, T3, Rest...> : common_reference<common_reference_t<Type1, Type2>, T3, Rest...> {
};

  export template <typename T>
  struct TypeIidentity {
    using Type = T;
  };

  export template <typename Type>
  using typeIdentity = TypeIidentity<Type>::Type;

  template <typename Type, template <typename...> typename Template>
  constexpr Bool isSpecialization{false}; // true if and only if Type is a specialization of _Template
  template <template <typename...> typename Template, typename... Types>
  constexpr Bool isSpecialization<Template<Types...>, Template>{true};

  export template <typename Type>
  [[nodiscard]] [[msvc::intrinsic]] constexpr Type&& forward(removeR<Type>& value) noexcept { return (Type&&)value; }

  export template <typename Type>
  [[nodiscard]] [[msvc::intrinsic]] constexpr removeR<Type>&& move(Type&& value) noexcept { return (removeR<Type>&&)value; }

  export template <typename Type>
  [[nodiscard]] [[msvc::intrinsic]] constexpr
    conditional<!isNothrowMoveConstructible<Type> && isCopyConstructible<Type>, const Type&, Type&&>
    moveIfNoexcept(Type& value) noexcept
  {
    return move(value);
  }

  export template <typename Type>
  [[nodiscard]] constexpr Type* addressOf(Type& value) noexcept { return __builtin_addressof(value); }

  template <typename Type>
  [[nodiscard]] Type fakeCopyInit(Type) noexcept;

  export template <typename Type>
  class ReferenceWrapper;

  enum class InvokerStrategy {
    Functor,
    PmfObject,
    PmfRefwrap,
    PmfPointer,
    PmdObject,
    PmdRefwrap,
    PmdPointer
  };

  struct InvokerFunctor {
    static constexpr InvokerStrategy strategy{InvokerStrategy::Functor};

    template <typename Callable, typename... Types>
    static constexpr auto call(Callable&& object, Types&&... values)
        noexcept(noexcept(((Callable&&)object)(static_cast<Types&&>(values)...))) //
        -> decltype(((Callable&&)object)(static_cast<Types&&>(values)...))
    {
      return ((Callable&&)object)(static_cast<Types&&>(values)...);
    }
  };

  struct InvokerPmfObject {
    static constexpr InvokerStrategy strategy{InvokerStrategy::PmfObject};

    template <typename Decayed, typename Type, typename... Types>
    static constexpr auto call(Decayed pmf, Type&& argument, Types&&... arguments)
        noexcept(noexcept((static_cast<Type&&>(argument).*pmf)(static_cast<Types&&>(arguments)...))) //
        -> decltype((static_cast<Type&&>(argument).*pmf)(static_cast<Types&&>(arguments)...))
    {
      return (static_cast<Type&&>(argument).*pmf)(static_cast<Types&&>(arguments)...);
    }
  };

  struct InvokerPmfRefwrap {
    static constexpr InvokerStrategy strategy{InvokerStrategy::PmfRefwrap};

    template <typename Decayed, typename Refwrap, typename... Types>
    static constexpr auto call(Decayed pmf, Refwrap refwrap, Types&&... arguments)
        noexcept(noexcept((refwrap.get().*pmf)(static_cast<Types&&>(arguments)...))) //
        -> decltype((refwrap.get().*pmf)(static_cast<Types&&>(arguments)...))
    {
      return (refwrap.get().*pmf)(static_cast<Types&&>(arguments)...);
    }
  };

  struct InvokerPmfPointer {
    static constexpr InvokerStrategy strategy{InvokerStrategy::PmfPointer};

    template <typename Decayed, typename Type, typename... Types>
    static constexpr auto call(Decayed pmf, Type&& argument, Types&&... arguments)
        noexcept(noexcept(((*static_cast<Type&&>(argument)).*pmf)(static_cast<Types&&>(arguments)...))) //
        -> decltype(((*static_cast<Type&&>(argument)).*pmf)(static_cast<Types&&>(arguments)...))
    {
      return ((*static_cast<Type&&>(argument)).*pmf)(static_cast<Types&&>(arguments)...);
    }
  };

  struct InvokerPmdObject {
    static constexpr InvokerStrategy strategy{InvokerStrategy::PmdObject};

    template <typename Decayed, typename Type>
    static constexpr auto call(Decayed pmd, Type&& argument) noexcept -> decltype(static_cast<Type&&>(argument).*pmd)
    {
      return static_cast<Type&&>(argument).*pmd;
    }
  };

  struct InvokerPmdRefwrap {
    static constexpr InvokerStrategy strategy{InvokerStrategy::PmdRefwrap};

    template <typename Decayed, typename Refwrap>
    static constexpr auto call(Decayed pmd, Refwrap refwrap) noexcept -> decltype(refwrap.get().*pmd)
    {
      return refwrap.get().*pmd;
    }
  };

  struct InvokerPmdPointer {
    static constexpr InvokerStrategy strategy{InvokerStrategy::PmdPointer};

    template <typename Decayed, typename Type>
    static constexpr auto call(Decayed pmd, Type&& argument) noexcept(noexcept((*static_cast<Type&&>(argument)).*pmd))
        -> decltype((*static_cast<Type&&>(argument)).*pmd)
    {
      return (*static_cast<Type&&>(argument)).*pmd;
    }
  };

  template <typename Callable, typename Type, typename RemovedCVR = removeCVR<Callable>,
      Bool isPmf = isMemberFunctionPointer<RemovedCVR>,
      Bool isPmd = isMemberObjectPointer<RemovedCVR>>
  struct Invoker;

  template <typename Callable, typename Type, typename RemovedCVR>
  struct Invoker<Callable, Type, RemovedCVR, true, false>
      : conditional<
          isSame<typename MemberPointer<RemovedCVR>::Class, removeCVR<Type>>
              || isBaseOf<typename MemberPointer<RemovedCVR>::Class, removeCVR<Type>>,
          InvokerPmfObject,
          conditional<isSpecialization<removeCVR<Type>, ReferenceWrapper>, InvokerPmfRefwrap, InvokerPmfPointer>> {};

  template <typename Callable, typename Type, typename RemovedCVR>
  struct Invoker<Callable, Type, RemovedCVR, false, true>
      : conditional<
          isSame<typename MemberPointer<RemovedCVR>::Class, removeCVR<Type>>
              || isBaseOf<typename MemberPointer<RemovedCVR>::Class, removeCVR<Type>>,
          InvokerPmdObject,
          conditional<isSpecialization<removeCVR<Type>, ReferenceWrapper>, InvokerPmdRefwrap, InvokerPmdPointer>> {};

  template <typename Callable, typename Type, typename RemovedCVR>
  struct Invoker<Callable, Type, RemovedCVR, false, false> : InvokerFunctor {};

  export template <typename Callable>
  constexpr auto invoke(Callable&& object) noexcept(noexcept(((Callable&&)object)())) -> decltype(((Callable&&)object)())
  {
    return ((Callable&&)object)();
  }

  export template <typename Callable, typename Type, typename... Types>
  constexpr auto invoke(Callable&& object, Type&& argument, Types&&... arguments)
      noexcept(noexcept(Invoker<Callable, Type>::_Call(
          (Callable&&)object, static_cast<Type&&>(argument), static_cast<Types&&>(arguments)...))) //
      -> decltype(Invoker<Callable, Type>::_Call(
          (Callable&&)object, static_cast<Type&&>(argument), static_cast<Types&&>(arguments)...))
  {
    if constexpr (Invoker<Callable, Type>::strategy == InvokerStrategy::Functor) {
      return ((Callable&&)object)(static_cast<Type&&>(argument), static_cast<Types&&>(arguments)...);
    } else if constexpr (Invoker<Callable, Type>::strategy == InvokerStrategy::PmfObject) {
      return (static_cast<Type&&>(argument).*object)(static_cast<Types&&>(arguments)...);
    } else if constexpr (Invoker<Callable, Type>::strategy == InvokerStrategy::PmfRefwrap) {
      return (argument.get().*object)(static_cast<Types&&>(arguments)...);
    } else if constexpr (Invoker<Callable, Type>::strategy == InvokerStrategy::PmfPointer) {
      return ((*static_cast<Type&&>(argument)).*object)(static_cast<Types&&>(arguments)...);
    } else if constexpr (Invoker<Callable, Type>::strategy == InvokerStrategy::PmdObject) {
      return static_cast<Type&&>(argument).*object;
    } else if constexpr (Invoker<Callable, Type>::strategy == InvokerStrategy::PmdRefwrap) {
      return argument.get().*object;
    } else {
      _STL_INTERNAL_STATIC_ASSERT(Invoker<Callable, Type>::strategy == InvokerStrategy::PmdPointer);
      return (*static_cast<Type&&>(argument)).*object;
    }
  }

  template <typename From, typename To, Bool = isConvertible<From, To>, Bool = isVoid<To>>
  constexpr Bool isNothrowConvertible = noexcept(fakeCopyInit<To>(declvalue<From>()));

  template <typename From, typename To, Bool isVoid>
  constexpr Bool isNothrowConvertible<From, To, false, isVoid> = false;

  template <typename From, typename To>
  constexpr Bool isNothrowConvertible<From, To, true, true> = true;

  template <typename From, typename To, typename = Void>
  constexpr Bool isInvokeConvertible{false};

  template <typename To, typename From>
  constexpr Bool notReferenceConvertsFromTemporary{true};

  template <typename From, typename To>
  constexpr Bool isInvokeConvertible<From, To, Vaild<decltype(fakeCopyInit<To>(returnsExactly<From>()))>>
      {notReferenceConvertsFromTemporary<To, From>};

  template <typename From, typename To>
  constexpr Bool isInvokeNothrowConvertible{noexcept(fakeCopyInit<To>(returnsExactly<From>()))};

  template <typename Result, Bool isNothrow>
  struct InvokeTraitsCommon {
    using Type = Result;
    static constexpr Bool isInvocable{true};
    static constexpr Bool isNothrowInvocable{isNothrow};
    template <typename Rx>
    static constexpr Bool isInvocableR{isVoid<Rx> || isInvokeConvertible<Type, Rx>};
    template <typename Rx>
    static constexpr Bool isNothrowInvocableR
        {isNothrowInvocable && (isVoid<Rx> || isInvokeConvertible<Type, Rx> && isInvokeNothrowConvertible<Type, Rx>)};
  };

  template <typename Void, typename Callable>
  struct InvokeTraitsZero {
    static constexpr Bool isInvocable{false};
    static constexpr Bool isNothrowInvocable{false};
    template <typename Rx>
    static constexpr Bool isInvocableR{false};
    template <typename Rx>
    static constexpr Bool isNothrowInvocableR{false};
  };

  template <typename Callable>
  using DecltypeInvokeZero = decltype(declvalue<Callable>()());

  template <typename Callable>
  struct InvokeTraitsZero<Vaild<DecltypeInvokeZero<Callable>>, Callable>
      : InvokeTraitsCommon<DecltypeInvokeZero<Callable>, noexcept(declvalue<Callable>()())> {};

  template <typename Void, typename... Types>
  struct InvokeTraitsNonzero {
    static constexpr Bool isInvocable{false};
    static constexpr Bool isNothrowInvocable{false};
    template <typename Rx>
    static constexpr Bool isInvocableR{false};
    template <typename Rx>
    static constexpr Bool isNothrowInvocableR{false};
  };

  template <class Callable, class Type, class... Types>
  using DecltypeInvokeNonzero = decltype(Invoker<Callable, Type>::_Call(
    declvalue<Callable>(), declvalue<Type>(), declvalue<Types>()...));

  template <class Callable, class Type, class... Types>
  struct InvokeTraitsNonzero<Vaild<DecltypeInvokeNonzero<Callable, Type, Types...>>, Callable, Type, Types...>
      : InvokeTraitsCommon<DecltypeInvokeNonzero<Callable, Type, Types...>,
          noexcept(Invoker<Callable, Type>::call(declvalue<Callable>(), declvalue<Type>(), declvalue<Types>()...))> {};
/*
template <class Callable, class... _Args>
using _Select_invoke_traits = conditional_t<sizeof...(_Args) == 0, _Invoke_traits_zero<void, Callable>,
  _Invoke_traits_nonzero<void, Callable, _Args...>>;

template <class Callable, class... _Args>
using _Invoke_result_t = typename _Select_invoke_traits<Callable, _Args...>::type;

template <class _Rx, class Callable, class... _Args>
using _Is_invocable_r_ = typename _Select_invoke_traits<Callable, _Args...>::template _Is_invocable_r<_Rx>;

template <class _Rx, class Callable, class... _Args>
struct _Is_invocable_r : _Is_invocable_r_<_Rx, Callable, _Args...> {
  // determines whether Callable is callable with _Args and return type _Rx
};

export template <class Callable, class... _Args>
struct invoke_result : _Select_invoke_traits<Callable, _Args...> {
  // determine the result type of invoking Callable with _Args
};

export template <class Callable, class... _Args>
using invoke_result_t = typename _Select_invoke_traits<Callable, _Args...>::type;

export template <class Callable, class... _Args>
struct is_invocable : _Select_invoke_traits<Callable, _Args...>::_Is_invocable {
  // determines whether Callable is callable with _Args
};

export template <class Callable, class... _Args>
constexpr bool is_invocable_v =
  _Select_invoke_traits<Callable, _Args...>::_Is_invocable::value;

export template <class Callable, class... _Args>
struct is_nothrow_invocable
  : _Select_invoke_traits<Callable, _Args...>::_Is_nothrow_invocable {
  // determines whether Callable is nothrow-callable with _Args
};

export template <class Callable, class... _Args>
constexpr bool is_nothrow_invocable_v =
  _Select_invoke_traits<Callable, _Args...>::_Is_nothrow_invocable::value;

export template <class _Rx, class Callable, class... _Args>
struct is_invocable_r : _Is_invocable_r_<_Rx, Callable, _Args...> {
  // determines whether Callable is callable with _Args and return type _Rx
};

export template <class _Rx, class Callable, class... _Args>
constexpr bool is_invocable_r_v = _Is_invocable_r_<_Rx, Callable, _Args...>::value;

export template <class _Rx, class Callable, class... _Args>
struct is_nothrow_invocable_r
  : _Select_invoke_traits<Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx> {
  // determines whether Callable is nothrow-callable with _Args and return type _Rx
};

export template <class _Rx, class Callable, class... _Args>
constexpr bool is_nothrow_invocable_r_v =
  _Select_invoke_traits<Callable, _Args...>::template _Is_nothrow_invocable_r<_Rx>::value;

export template <class Type, class _Ty2>
struct is_layout_compatible : bool_constant<__is_layout_compatible(Type, _Ty2)> {};

export template <class Type, class _Ty2>
constexpr bool is_layout_compatible_v = __is_layout_compatible(Type, _Ty2);

export template <class _Base, class _Derived>
struct is_pointer_interconvertible_base_of
  : bool_constant<__is_pointer_interconvertible_base_of(_Base, _Derived)> {};

export template <class _Base, class _Derived>
constexpr bool is_pointer_interconvertible_base_of_v =
  __is_pointer_interconvertible_base_of(_Base, _Derived);

export template <class _ClassTy, class _MemberTy>
[[nodiscard]] constexpr bool is_pointer_interconvertible_with_class(
  _MemberTy _ClassTy::* _Pm) noexcept {
  return __is_pointer_interconvertible_with_class(_ClassTy, _Pm);
}

export template <class _ClassTy1, class _ClassTy2, class _MemberTy1, class _MemberTy2>
[[nodiscard]] constexpr bool is_corresponding_member(
  _MemberTy1 _ClassTy1::* _Pm1, _MemberTy2 _ClassTy2::* _Pm2) noexcept {
  return __is_corresponding_member(_ClassTy1, _ClassTy2, _Pm1, _Pm2);
}

template <class _Ty>
struct _Function_args {}; // determine whether _Ty is a function
*/
//2068

}
