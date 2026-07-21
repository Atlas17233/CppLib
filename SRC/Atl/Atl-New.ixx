export module Atl:New;

import "Macros";

namespace Atl
{
  export template <typename Type>
  [[nodiscard]] forceinline constexpr Type* launder(Type* pointer) noexcept
  {
    return __builtin_launder(pointer);
  }
}
