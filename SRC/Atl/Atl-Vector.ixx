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
    [[nodiscard]] constexpr Bool empty() const noexcept { return begin_ == end_; }
    [[nodiscard]] constexpr Size size() const noexcept { return (Size)(end_ - begin_); }

    [[nodiscard]] constexpr const Type& operator[](Size i) const noexcept { return begin_[i]; }
    [[nodiscard]] constexpr Type& operator[](Size i) noexcept { return begin_[i]; }
/*
    constexpr Void push_back(const Type& value) {
        _Emplace_one_at_back(value);
    }

    constexpr Void push_back(Type&& value) {
        _Emplace_one_at_back(move(value));
    }

    private:
      template <class... ValueType>
      constexpr Type& _Emplace_back_with_unused_capacity(ValueType&&... value) {
          if constexpr (isNothrowConstructible<Type, ValueType...> && _Uses_default_construct<_Alloc, Type*, ValueType...>) {
              _STD _Construct_in_place(*end_, forward<ValueType>(value)...);
          } else {
              _Alty_traits::construct(_Getal(), _Unfancy(end_), forward<ValueType>(value)...);
          }
          return *(end_++);
      }

    template <class... _Valty>
    _CONSTEXPR20 pointer _Emplace_reallocate(const pointer _Whereptr, _Valty&&... _Val) {
        // reallocate and insert by perfectly forwarding _Val at _Whereptr
        _Alty& _Al        = _Getal();
        auto& _My_data    = _Mypair._Myval2;
        pointer& _Myfirst = _My_data._Myfirst;
        pointer& _Mylast  = _My_data._Mylast;

        const auto _Whereoff = _Whereptr - _Myfirst;
        const auto _Oldsize  = static_cast<size_type>(_Mylast - _Myfirst);

        const size_type _Newsize = _Oldsize + 1;
        size_type _Newcapacity   = _Calculate_growth(_Newsize);

        const pointer _Newvec           = _STD _Allocate_at_least_helper(_Al, _Newcapacity);
        const pointer _Constructed_last = _Newvec + static_cast<_Iter_diff_t<pointer>>(_Whereoff + 1);

        _Reallocation_guard2 _Guard{_Al, _Newvec, _Newcapacity, _Constructed_last, _Constructed_last};
        auto& _Constructed_first = _Guard._Constructed_first;

        _Alty_traits::construct(_Al, _STD _Unfancy(_Newvec + _Whereoff), _STD forward<_Valty>(_Val)...);
        _Constructed_first = _Newvec + _Whereoff;

        if (_Whereptr == _Mylast) { // at back, provide strong guarantee
            if constexpr (is_nothrow_move_constructible_v<_Ty> || !is_copy_constructible_v<_Ty>) {
                _STD _Uninitialized_move(_Myfirst, _Mylast, _Newvec, _Al);
            } else {
                _STD _Uninitialized_copy(_Myfirst, _Mylast, _Newvec, _Al);
            }
        } else { // provide basic guarantee
            _STD _Uninitialized_move(_Myfirst, _Whereptr, _Newvec, _Al);
            _Constructed_first = _Newvec;
            _STD _Uninitialized_move(
                _Whereptr, _Mylast, _Newvec + static_cast<_Iter_diff_t<pointer>>(_Whereoff + 1), _Al);
        }

        _Guard._New_begin = nullptr;
        _Change_array(_Newvec, _Newsize, _Newcapacity);
        return _Newvec + _Whereoff;
    }

      template <typename... ValueType>
      constexpr Type& _Emplace_one_at_back(ValueType&&... value) {
          if (end_ != capacity_) {
              return _Emplace_back_with_unused_capacity(_STD forward<ValueType>(value)...);
          }

          return *_Emplace_reallocate(end_, _STD forward<ValueType>(value)...);
      }
*/
      Type* begin_;
      Type* end_;
      Type* capacity_;
    };
  }
}
