export module Atl:Vector;

import :Allocator;
import :Def;

namespace Atl
{
  export
  {
    template <typename Type, typename Allocator = LargeAllocator>
    class Vector: private Allocator
    {
    public:
      /*constexpr void reserve(Size newCapacity) {
        if (newCapacity > capacity()) {
          _Reallocate<_Reallocation_policy::_At_least>(_Newcapacity);
        }
      }*/


      [[nodiscard]] constexpr Bool empty() const noexcept { return begin_ == end_; }
      [[nodiscard]] constexpr Size size() const noexcept { return (Size)(end_ - begin_); }
      [[nodiscard]] constexpr Size capacity() const noexcept { return (Size)(capacity_ - begin_); }

      [[nodiscard]] constexpr const Type& operator[](Size i) const noexcept { return begin_[i]; }
      [[nodiscard]] constexpr Type& operator[](Size i) noexcept { return begin_[i]; }

    private:
      Type* begin_;
      Type* end_;
      Type* capacity_;
    };
  }
}
