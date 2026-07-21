export module Atl:Exception;

import :Def;

import "Macros";

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
