export module Atl:Compare;

import :Def;
import :Type;

import "Macros";

namespace Atl
{
  export
  {
    using PartialOrdering = std::partial_ordering;
    using WeakOrdering = std::weak_ordering;
    using StrongOrdering = std::strong_ordering;

    [[nodiscard]] constexpr Bool isEq(PartialOrdering comp) noexcept {
      return comp == 0;
    }

    [[nodiscard]] constexpr Bool isNEq(PartialOrdering comp) noexcept {
      return comp != 0;
    }

    [[nodiscard]] constexpr Bool isLt(PartialOrdering comp) noexcept {
      return comp < 0;
    }

    [[nodiscard]] constexpr Bool isLtEq(PartialOrdering comp) noexcept {
      return comp <= 0;
    }

    [[nodiscard]] constexpr Bool isGt(PartialOrdering comp) noexcept {
      return comp > 0;
    }

    [[nodiscard]] constexpr Bool isGtEq(PartialOrdering comp) noexcept {
      return comp >= 0;
    }
  }
}
