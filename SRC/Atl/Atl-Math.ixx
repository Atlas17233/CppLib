export module Atl:Math;

import :Def;

import "Macros";

namespace Atl
{
  export
  {
    template <typename Type>
    [[nodiscard]] constexpr const Type& min(const Type& left, const Type& right) noexcept
    {
      return left < right ? left : right;
    }

    template <typename Type>
    [[nodiscard]] constexpr const Type& max(const Type& left, const Type& right) noexcept
    {
      return left > right ? left : right;
    }
  }
}
