export module Atl:Vector;

import :Allocator;
import :Def;

namespace Atl
{
  export
  {
    template <typename Type, typename Allocator = LargeAllocator>
    class Vector
    {
    public:
      /*constexpr Void _Tidy() noexcept {
          auto& _Al         = _Mypair._Get_first();
          auto& _My_data    = _Mypair._Myval2;
          pointer& _Myfirst = _My_data._Myfirst;
          pointer& _Mylast  = _My_data._Mylast;
          pointer& _Myend   = _My_data._Myend;

          if (_Myfirst) { // destroy and deallocate old array
              _STD _Destroy_range(_Myfirst, _Mylast, _Al);
              _ASAN_VECTOR_REMOVE;
              _Al.deallocate(_Myfirst, static_cast<size_type>(_Myend - _Myfirst));

              _Myfirst = nullptr;
              _Mylast  = nullptr;
              _Myend   = nullptr;
          }
      }

      constexpr Vector& operator=(Vector&& right) noexcept(_Choose_pocma_v<_Alty> != _Pocma_values::_No_propagate_allocators) {
        if (this == addressOf(right)) [[unlikely]] {
          return *this;
        }

        _Alty& _Al                = _Mypair._Get_first();
        _Alty& right_al          = right._Mypair._Get_first();
        constexpr auto _Pocma_val = _Choose_pocma_v<_Alty>;
        if constexpr (_Pocma_val == _Pocma_values::_No_propagate_allocators) {
          if (_Al != right_al) {
            _Move_assign_unequal_alloc(right);
            return *this;
          }
        }

        _Tidy();
        _Pocma(_Al, right_al);
        _Mypair._Myval2._Take_contents(right._Mypair._Myval2);
        return *this;
      }*/

      [[nodiscard]] constexpr Bool empty() const noexcept { return begin_ == end_; }
      [[nodiscard]] constexpr Size size() const noexcept { return (Size)(end_ - begin_); }

      [[nodiscard]] constexpr const Type& operator[](Size i) const noexcept { return begin_[i]; }
      [[nodiscard]] constexpr Type& operator[](Size i) noexcept { return begin_[i]; }

      Type* begin_;
      Type* end_;
      Type* capacity_;
    };
  }
}
