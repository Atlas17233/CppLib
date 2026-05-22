export module Atl:Limits;

import :Def;
import :Type;

namespace Atl
{
  export
  {
    template <typename Type>
    constexpr Type Min = isUnsigned<Type> ? 0 : 1 << sizeof(Type) * 8 - 1;

    template <typename Type>
    constexpr Type Max = ~Min<Type>;
  }
}
