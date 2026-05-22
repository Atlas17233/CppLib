export module Atl:Exception;

import :Def;

namespace Atl
{
  export
  {
    struct Exception: public std::exception
    {
      Exception(const std::string_view& str): std::exception{str.data()} {}
    };
  }
}
