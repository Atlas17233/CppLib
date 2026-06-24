export module Atl:Compare;

import :Def;
import :Type;

namespace Atl
{
  Void expectedLiteralZero();

  struct LiteralZero {
    template <IntType Type>
    consteval LiteralZero(Type zero) noexcept
    {
      if (zero) {
        expectedLiteralZero();
      }
    }
  };

  using Compare = Int8;

  enum class CompareEq : Compare { Equal = 0, Equivalent = Equal };
  enum class CompareOrd : Compare { Less = -1, Greater = 1 };
  enum class CompareNcmp : Compare { Unordered = -128 };

  export
  {
    struct PartialOrdering {
      static const PartialOrdering Less;
      static const PartialOrdering Equivalent;
      static const PartialOrdering Greater;
      static const PartialOrdering Unordered;

      [[nodiscard]] friend constexpr Bool operator==(PartialOrdering value, LiteralZero) noexcept
      {
        return value.value == 0;
      }

      [[nodiscard]] friend constexpr Bool operator==(PartialOrdering, PartialOrdering) noexcept = default;

      [[nodiscard]] friend constexpr Bool operator<(PartialOrdering value, LiteralZero) noexcept
      {
        return value.value == (Compare)CompareOrd::Less;
      }

      [[nodiscard]] friend constexpr Bool operator>(PartialOrdering value, LiteralZero) noexcept
      {
        return value.value > 0;
      }

      [[nodiscard]] friend constexpr Bool operator<=(PartialOrdering value, LiteralZero) noexcept
      {
        return (Compare)-(UInt32)value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator>=(PartialOrdering value, LiteralZero) noexcept
      {
        return value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator<(LiteralZero, PartialOrdering value) noexcept
      {
        return value.value > 0;
      }

      [[nodiscard]] friend constexpr Bool operator>(LiteralZero, PartialOrdering value) noexcept
      {
        return value.value < 0;
      }

      [[nodiscard]] friend constexpr Bool operator<=(LiteralZero, PartialOrdering value) noexcept
      {
        return value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator>=(LiteralZero, PartialOrdering value) noexcept
      {
        return value.value <= 0;
      }

      [[nodiscard]] friend constexpr PartialOrdering operator<=>(PartialOrdering value, LiteralZero) noexcept
      {
        return value;
      }

      [[nodiscard]] friend constexpr PartialOrdering operator<=>(LiteralZero, PartialOrdering value) noexcept
      {
        return {(Compare)-value.value};
      }

      Compare value;
    };

    constexpr PartialOrdering PartialOrdering::Less{(Compare)CompareOrd::Less};
    constexpr PartialOrdering PartialOrdering::Equivalent{(Compare)CompareEq::Equivalent};
    constexpr PartialOrdering PartialOrdering::Greater{(Compare)CompareOrd::Greater};
    constexpr PartialOrdering PartialOrdering::Unordered{(Compare)CompareNcmp::Unordered};

    struct WeakOrdering {
      static const WeakOrdering Less;
      static const WeakOrdering Equivalent;
      static const WeakOrdering Greater;

      constexpr operator PartialOrdering() const noexcept
      {
        return {value};
      }

      [[nodiscard]] friend constexpr Bool operator==(WeakOrdering value, LiteralZero) noexcept
      {
        return value.value == 0;
      }

      [[nodiscard]] friend constexpr Bool operator==(WeakOrdering, WeakOrdering) noexcept = default;

      [[nodiscard]] friend constexpr Bool operator<(WeakOrdering value, LiteralZero) noexcept
      {
        return value.value < 0;
      }

      [[nodiscard]] friend constexpr Bool operator>(WeakOrdering value, LiteralZero) noexcept
      {
        return value.value > 0;
      }

      [[nodiscard]] friend constexpr Bool operator<=(WeakOrdering value, LiteralZero) noexcept
      {
        return value.value <= 0;
      }

      [[nodiscard]] friend constexpr Bool operator>=(const WeakOrdering value, LiteralZero) noexcept
      {
        return value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator<(LiteralZero, const WeakOrdering value) noexcept
      {
        return value.value > 0;
      }

      [[nodiscard]] friend constexpr Bool operator>(LiteralZero, const WeakOrdering value) noexcept
      {
        return value.value < 0;
      }

      [[nodiscard]] friend constexpr Bool operator<=(LiteralZero, const WeakOrdering value) noexcept
      {
        return value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator>=(LiteralZero, const WeakOrdering value) noexcept
      {
        return value.value <= 0;
      }

      [[nodiscard]] friend constexpr WeakOrdering operator<=>(const WeakOrdering value, LiteralZero) noexcept
      {
        return value;
      }

      [[nodiscard]] friend constexpr WeakOrdering operator<=>(LiteralZero, const WeakOrdering value) noexcept
      {
        return {(Compare)-value.value};
      }

      Compare value;
    };

    constexpr WeakOrdering WeakOrdering::Less{(Compare)CompareOrd::Less};
    constexpr WeakOrdering WeakOrdering::Equivalent{(Compare)CompareEq::Equivalent};
    constexpr WeakOrdering WeakOrdering::Greater{(Compare)CompareOrd::Greater};

    struct StrongOrdering
    {
      static const StrongOrdering Less;
      static const StrongOrdering Equal;
      static const StrongOrdering Equivalent;
      static const StrongOrdering Greater;

      constexpr operator PartialOrdering() const noexcept
      {
        return {value};
      }

      constexpr operator WeakOrdering() const noexcept
      {
        return {value};
      }

      [[nodiscard]] friend constexpr Bool operator==(const StrongOrdering value, LiteralZero) noexcept
      {
        return value.value == 0;
      }

      [[nodiscard]] friend constexpr Bool operator==(StrongOrdering, StrongOrdering) noexcept = default;

      [[nodiscard]] friend constexpr Bool operator<(const StrongOrdering value, LiteralZero) noexcept
      {
        return value.value < 0;
      }

      [[nodiscard]] friend constexpr Bool operator>(const StrongOrdering value, LiteralZero) noexcept
      {
        return value.value > 0;
      }

      [[nodiscard]] friend constexpr Bool operator<=(const StrongOrdering value, LiteralZero) noexcept
      {
        return value.value <= 0;
      }

      [[nodiscard]] friend constexpr Bool operator>=(const StrongOrdering value, LiteralZero) noexcept
      {
        return value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator<(LiteralZero, const StrongOrdering value) noexcept
      {
        return value.value > 0;
      }

      [[nodiscard]] friend constexpr Bool operator>(LiteralZero, const StrongOrdering value) noexcept
      {
        return value.value < 0;
      }

      [[nodiscard]] friend constexpr Bool operator<=(LiteralZero, const StrongOrdering value) noexcept
      {
        return value.value >= 0;
      }

      [[nodiscard]] friend constexpr Bool operator>=(LiteralZero, const StrongOrdering value) noexcept
      {
        return value.value <= 0;
      }

      [[nodiscard]] friend constexpr StrongOrdering operator<=>(const StrongOrdering value, LiteralZero) noexcept
      {
        return value;
      }

      [[nodiscard]] friend constexpr StrongOrdering operator<=>(LiteralZero, const StrongOrdering value) noexcept
      {
        return {(Compare)-value.value};
      }

      Compare value;
    };

    constexpr StrongOrdering StrongOrdering::Less{(Compare)CompareOrd::Less};
    constexpr StrongOrdering StrongOrdering::Equal{(Compare)CompareEq::Equal};
    constexpr StrongOrdering StrongOrdering::Equivalent{(Compare)CompareEq::Equivalent};
    constexpr StrongOrdering StrongOrdering::Greater{(Compare)CompareOrd::Greater};

    [[nodiscard]] constexpr Bool isEq(PartialOrdering comp) noexcept {
      return comp == 0;
    }

    [[nodiscard]] constexpr Bool isNotEq(PartialOrdering comp) noexcept {
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
