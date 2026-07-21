export module Atl:InitializerList;

import :Def;

import "Macros";

namespace Atl
{
  export template <typename Type>
  class InitializerList
  {
  public:
    constexpr InitializerList() noexcept: begin_{}, end_{} {}
    constexpr InitializerList(const Type* begin, const Type* end) noexcept: begin_{begin}, end_{end} {}

    [[nodiscard]] constexpr const Type* begin() const noexcept { return begin_; }
    [[nodiscard]] constexpr const Type* end() const noexcept { return end_; }
    [[nodiscard]] constexpr Size size() const noexcept { return end_ - begin_; }
    [[nodiscard]] constexpr Bool empty() const noexcept { return begin_ == end_; }
    [[nodiscard]] constexpr const Type* data() const noexcept { return begin_; }

  private:
    const Type* begin_;
    const Type* end_;
  };
}
