export module Atl:Digest;

import :Array;
import :Bit;
import :Def;
import :String;

namespace Atl
{
  template <StringLiteral name, Size size_>
  class Digest final: public Array<UInt32, size_>
  {
  public:
    explicit constexpr Digest(const ConstString& str) noexcept
    {
      operator()(str);
    }

    [[msvc::forceinline]] constexpr Void init() noexcept { copy(initValue, size_, Array<UInt32, size_>::data_); }

    constexpr const Digest& operator()(const UInt8* data, Size size) noexcept;

    [[msvc::forceinline]] constexpr const Digest& operator()(const ConstString& str) noexcept
    {
      return operator()((UInt8*)str.data(), str.size());
    }

    [[nodiscard]] constexpr std::string toString(const UInt8 table[16] = hex) const noexcept
    {
      std::string result;
      result.reserve(size_ << 3);
      for (Int i{0}; i < (size_ << 2); ++i) {
        result += table[((UInt8*)this)[i] >> 4];
        result += table[((UInt8*)this)[i] & 15];
      }
      return result;
    }

  private:
    static constexpr UInt32 initValue[size_]{};
  };

  export
  {
    template <StringLiteral name, Size size_>
    constexpr std::ostream& operator<<(std::ostream& out, const Digest<name, size_>& digest) noexcept
    {
      return out << digest.toString();
    }

    using CRC32 = Digest<"CRC32", 1>;
    using MD5 = Digest<"MD5", 4>;
    using SHA256 = Digest<"SHA-256", 8>;
  }
}
