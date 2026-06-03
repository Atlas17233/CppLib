export module Atl:Limits;

import :Def;
import :Type;

namespace Atl
{
  export
  {
    template <typename Type>
    constexpr Type Min{isSigned<Type> ? 1 << sizeof(Type) * 8 - 1 : 0};

    template <typename Type>
    constexpr Type Max{~Min<Type>};
  }
}
