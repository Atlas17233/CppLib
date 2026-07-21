export module Atl:DateTime;

import :Def;

import "Macros";

namespace Atl
{
  export
  {
    constexpr std::string getDateTime()
    {
      return std::format("{:%F %T}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()}).substr(0, 19);
    }
  }
}
