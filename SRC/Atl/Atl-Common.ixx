export module Atl:Common;

import :Def;

namespace Atl
{
  export
  {
    [[nodiscard]] constexpr Bool isConstEval() noexcept { return __builtin_is_constant_evaluated(); }
  }
}
