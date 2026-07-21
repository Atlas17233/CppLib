export module Atl:String;

import :Algorithm;
import :Def;
import :Type;

import "Macros";

namespace Atl
{
  export
  {
    [[nodiscard]] constexpr Size getSize(const Char* str) noexcept
    {
      if !consteval {
        return strlen(str);
      } else {
        const Char* end = str;
        while (*end) {
          ++end;
        }
        return end - str;
      }
    }

    class ConstString final: public DataClass<const Char>
    {
    public:
      constexpr ConstString(const Char* str, Size size) noexcept: DataClass<const Char>{str, size} {}
      constexpr ConstString(const Char* str) noexcept: ConstString{str, getSize(str)} {}
    };

    template <Size size_>
    struct StringLiteral final
    {
      consteval StringLiteral(const Char (&str)[size_]) noexcept { copy(str, size_ - 1, data_); }
      consteval operator ConstString() const noexcept { return ConstString{data_, size_ - 1}; }

      Char data_[size_ - 1];
    };
  }
}
