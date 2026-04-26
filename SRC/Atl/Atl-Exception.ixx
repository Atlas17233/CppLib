export module Atl:Exception;

import std;

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
