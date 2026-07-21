export module Atl:Utility;

import :Concepts;
import :Def;
import :Type;

import "Macros";

namespace Atl
{
  struct [[nodiscard]] ZeroupperOnExit { // TRANSITION, DevCom-10331414
    ZeroupperOnExit() = default;

    ZeroupperOnExit(const ZeroupperOnExit&)            = delete;
    ZeroupperOnExit& operator=(const ZeroupperOnExit&) = delete;

    ~ZeroupperOnExit() {
      _mm256_zeroupper();
    }
  };

  template <typename Type>
  concept SwappableType = isMoveConstructible<Type> && isMoveAssignable<Type>;

  export
  {
    template <SwappableType Type>
    constexpr Void swap(Type& left, Type& right) noexcept {
      Type tmp{move(left)};
      left = move(right);
      right = move(tmp);
    }
  }

}
