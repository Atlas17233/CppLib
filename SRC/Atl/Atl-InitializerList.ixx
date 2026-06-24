export module Atl:InitializerList;

import :Def;

namespace Atl
{
  export template <typename Type>
  class InitializerList
  {
  public:
    [[msvc::forceinline]] constexpr InitializerList() noexcept: begin_{}, end_{} {}
    [[msvc::forceinline]] constexpr InitializerList(const Type* begin, const Type* end) noexcept: begin_{begin}, end_{end} {}

    [[msvc::forceinline]] [[nodiscard]] constexpr const Type* begin() const noexcept { return begin_; }
    [[msvc::forceinline]] [[nodiscard]] constexpr const Type* end() const noexcept { return end_; }
    [[msvc::forceinline]] [[nodiscard]] constexpr Size size() const noexcept { return end_ - begin_; }
    [[msvc::forceinline]] [[nodiscard]] constexpr Bool empty() const noexcept { return begin_ == end_; }
    [[msvc::forceinline]] [[nodiscard]] constexpr const Type* data() const noexcept { return begin_; }

  private:
    const Type* begin_;
    const Type* end_;
  };
}