#include "time.hpp"
#include <chrono>
#include <format>
#include <string>

std::string Atlas::getDateTime()
{
  return std::format("{:%F %T}", std::chrono::zoned_time{std::chrono::current_zone(), std::chrono::system_clock::now()}).substr(0, 19);
}
