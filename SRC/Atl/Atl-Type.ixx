export module Atl:Type;

import :Def;

namespace Atl
{
  template<typename T>
  struct ConstRemoved {
    using Type = T;
  };

  template<typename T>
  struct ConstRemoved<const T> {
    using Type = T;
  };

  template<typename T>
  struct VolatileRemoved {
    using Type = T;
  };

  template<typename T>
  struct VolatileRemoved<volatile T> {
    using Type = T;
  };

  template<typename T>
  struct ConstVolatileRemoved {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = F<T>;
  };

  template<typename T>
  struct ConstVolatileRemoved<const T> {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = const F<T>;
  };

  template<typename T>
  struct ConstVolatileRemoved<volatile T> {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = volatile F<T>;
  };

  template<typename T>
  struct ConstVolatileRemoved<const volatile T> {
    using Type = T;

    template<template<typename> typename F>
    using _Apply = const volatile F<T>;
  };

  export
  {
    template<typename T>
    using removeConst = typename ConstRemoved<T>::Type;

    template<typename T>
    using removeVolatile = typename VolatileRemoved<T>::Type;

    template<typename T>
    using removeConstVolatile = typename ConstVolatileRemoved<T>::Type;
    
    template<typename, typename>
    constexpr Bool isSame = False;
  }

  template<typename T>
  constexpr Bool isSame<T, T> = True;
}
