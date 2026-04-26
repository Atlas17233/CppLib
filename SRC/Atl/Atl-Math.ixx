export module Atl:Math;

import :Def;

namespace Atl
{
  export
  {
    template<typename T>
    [[nodiscard]] constexpr const T& min(const T& l, const T& r) noexcept
    {
      return std::min(l, r);
    }

    template<typename T>
    [[nodiscard]] constexpr const T& max(const T& l, const T& r) noexcept
    {
      return std::max(l, r);
    }
  }
}
