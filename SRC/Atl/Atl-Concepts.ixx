export module Atl:Concepts;

import :Def;
import :Type;

namespace Atl
{
  template <typename Type>
  concept StandardUnsignedIntegral = isStandardUnsigned<Type>;
}
