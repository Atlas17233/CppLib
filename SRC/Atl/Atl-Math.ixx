export module Atl:Math;

import :Def;

namespace Atl
{
  export
  {
    template<typename Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr const Type& min(const Type& left, const Type& right) noexcept
    {
      return left < right ? left : right;
    }

    template<typename Type>
    [[msvc::forceinline]] [[nodiscard]] constexpr const Type& max(const Type& left, const Type& right) noexcept
    {
      return left > right ? left : right;
    }
  }
}
