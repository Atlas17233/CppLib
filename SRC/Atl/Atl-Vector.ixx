export module Atl:Vector;

import :Allocator;
import :Compare;
import :Def;
import :InitializerList;

namespace Atl
{
  export
  {
    template <typename T, typename AllocType = Allocator<T>>
    class Vector: private AllocType
    {
    private:
      constexpr Void constructDefault() noexcept
      {
        begin_ = nullptr;
        end_ = nullptr;
        capacity_ = nullptr;
      }

      constexpr Void construct(Size size) noexcept
      {
        begin_ = Alloc::allocate(size);
        end_ = begin_ + size;
        capacity_ = end_;
      }

      constexpr Bool enoughCapacity(Size size) noexcept
      {
        destroyRange(begin_, end_);
        if (capacity() < size) {
          capacity_ = Alloc::expand(begin_, capacity_, begin_ + size);
          if (capacity() < size) {
            Alloc::deallocate(begin_, capacity_);
            return false;
          }
        }
        end_ = begin_ + size;
        return true;
      }

      constexpr Void destruct() noexcept
      {
        if (capacity_) {
          destroyRange(begin_, end_);
          Alloc::deallocate(begin_, capacity_);
        }
      }

      template <typename... Type>
      constexpr Void constructN(Size size, Type&&... value) noexcept
      {
        if (size) {
          construct(size);

          if constexpr (sizeof...(value) == 0) {
            defaultConstruct(begin_, size);
          } else if constexpr (sizeof...(value) == 1) {
            fillConstruct(begin_, size, value...);
          } else if constexpr (sizeof...(value) == 2) {
            copyConstruct(forward<Type>(value)..., begin_);
          }
        } else {
          constructDefault();
        }
      }

    public:
      using Alloc = AllocType;
      using Type = T;

      class ConstIterator
      {
      public:
        using IteratorConcept = ContiguousIteratorTag;
        using IteratorCategory = RandomAccessIteratorTag;

        using Type = T;
        using Container = Vector<Type, Alloc>;

        [[nodiscard]] constexpr const Type& operator*() const noexcept
        {
          return *pointer_;
        }

        [[nodiscard]] constexpr const Type* operator->() const noexcept
        {
          return pointer_;
        }

        constexpr ConstIterator& operator++() noexcept
        {
          ++pointer_;
          return *this;
        }

        constexpr ConstIterator operator++(int) noexcept
        {
          ConstIterator tmp{*this};
          ++pointer_;
          return tmp;
        }

        constexpr ConstIterator& operator--() noexcept
        {
          --pointer_;
          return *this;
        }

        constexpr ConstIterator operator--(int) noexcept
        {
          ConstIterator tmp{*this};
          --pointer_;
          return tmp;
        }

        constexpr ConstIterator& operator+=(PtrDiff offset) noexcept
        {
          pointer_ += offset;
          return *this;
        }

        [[nodiscard]] constexpr ConstIterator operator+(PtrDiff offset) const noexcept
        {
          return pointer_ + offset;
        }

        constexpr ConstIterator& operator-=(PtrDiff offset) noexcept
        {
          pointer_ -= offset;
          return *this;
        }

        [[nodiscard]] constexpr ConstIterator operator-(PtrDiff offset) const noexcept
        {
          return pointer_ - offset;
        }

        [[nodiscard]] constexpr PtrDiff operator-(const ConstIterator& right) const noexcept
        {
          return pointer_ - right.pointer_;
        }

        [[nodiscard]] constexpr const Type& operator[](PtrDiff offset) const noexcept
        {
          return pointer_[offset];
        }

        [[nodiscard]] constexpr Bool operator==(const ConstIterator& right) const noexcept
        {
          return pointer_ == right.pointer_;
        }

        [[nodiscard]] constexpr StrongOrdering operator<=>(const ConstIterator& right) const noexcept
        {
          return pointer_ <=> right.pointer_;
        }

      protected:
        Type* pointer_{};
      };

      class Iterator: public ConstIterator
      {
      public:
        using Base = ConstIterator;

        using IteratorConcept = ContiguousIteratorTag;
        using IteratorCategory = RandomAccessIteratorTag;

        using Type = T;
        using Container = Vector<Type, Alloc>;

        [[nodiscard]] constexpr Type& operator*() const noexcept
        {
          return *Base::pointer_;
        }

        [[nodiscard]] constexpr Type* operator->() const noexcept
        {
          return Base::pointer_;
        }

        constexpr Iterator& operator++() noexcept
        {
          ++Base::pointer_;
          return *this;
        }

        constexpr Iterator operator++(int) noexcept
        {
          Iterator tmp{*this};
          ++Base::pointer_;
          return tmp;
        }

        constexpr Iterator& operator--() noexcept
        {
          --Base::pointer_;
          return *this;
        }

        constexpr Iterator operator--(int) noexcept
        {
          Iterator tmp{*this};
          --Base::pointer_;
          return tmp;
        }

        constexpr Iterator& operator+=(PtrDiff offset) noexcept
        {
          Base::pointer_ += offset;
          return *this;
        }

        [[nodiscard]] constexpr Iterator operator+(PtrDiff offset) const noexcept
        {
          return Base::pointer_ + offset;
        }

        constexpr Iterator& operator-=(PtrDiff offset) noexcept
        {
          Base::pointer_ -= offset;
          return *this;
        }

        using Base::operator-;

        [[nodiscard]] constexpr Iterator operator-(PtrDiff offset) const noexcept
        {
          return Base::pointer_ - offset;
        }

        [[nodiscard]] constexpr Type& operator[](PtrDiff offset) const noexcept
        {
          return Base::pointer_[offset];
        }
      };

      constexpr Vector() noexcept
      {
        constructDefault();
      }

      constexpr explicit Vector(Alloc&& alloc) noexcept: Alloc{forward<Alloc>(alloc)}
      {
        constructDefault();
      }

      constexpr explicit Vector(Size size, Alloc&& alloc = Alloc()) noexcept: Alloc{forward<Alloc>(alloc)}
      {
        constructN(size);
      }

      constexpr Vector(Size size, const Type& value, Alloc&& alloc = Alloc()) noexcept: Alloc{forward<Alloc>(alloc)}
      {
        constructN(size, value);
      }

      template <IteratorType Iter>
      constexpr Vector(Iter begin, Iter end, Alloc&& alloc = Alloc()) noexcept: Alloc{forward<Alloc>(alloc)}
      {
        constructN(size, begin, end);
      }

      constexpr Vector(InitializerList<Type> list, Alloc&& alloc = Alloc()) noexcept: Alloc{forward<Alloc>(alloc)}
      {
        constructN(list.size(), list.begin(), list.end());
      }

      constexpr Vector(const Vector& right) noexcept: Alloc{right}
      {
        constructN(right.size(), right.begin_, right.end_);
      }

      constexpr Vector(const Vector& right, Identity<Alloc>&& alloc) noexcept: Alloc{forward<Alloc>(alloc)}
      {
        constructN(right.size(), right.begin_, right.end_);
      }

      constexpr Vector(Vector&& right) noexcept:
        Alloc{move(right)}, begin_{right.begin_}, end_{right.end_}, capacity_{right.capacity_}
      {
        right.constructDefault();
      }

      constexpr Vector(Vector&& right, Identity<Alloc>&& alloc) noexcept:
        Alloc{forward<Alloc>(alloc)}, begin_{right.begin_}, end_{right.end_}, capacity_{right.capacity_}
      {
        right.constructDefault();
      }

      constexpr Vector& operator=(Vector&& right) noexcept
      {
        if (this != addressOf(right)) [[likely]] {
          destruct();
          begin_ = right.begin_;
          end_ = right.end_;
          capacity_ = right.capacity_;
          right.constructDefault();
        }
        return *this;
      }

      constexpr Vector& operator=(const Vector& right) noexcept
      {
        assign(right.begin(), right.end());
        return *this;
      }

      constexpr Vector& operator=(InitializerList<Type> list)
      {
        assign(list.begin(), list.end());
        return *this;
      }

      constexpr ~Vector() noexcept
      {
        destruct();
      }

      constexpr Void assign(Size size, const Type& value) noexcept
      {
        if (capacity_) {
          if (enoughCapacity(size)) {
            fillConstruct(begin_, size, value);
          } else {
            constructN(size, value);
          }
        } else if (size) {
          construct(size);
          fillConstructUnsafe(begin_, size, value);
        }
      }

      template <IteratorType Iter>
      constexpr Void assign(Iter begin, Iter end) noexcept
      {
        Size size{end - begin};
        if (capacity_) {
          if (enoughCapacity(size)) {
            copyConstruct(begin, end, begin_);
          } else {
            constructN(size, begin, end);
          }
        } else if (size) {
          construct(size);
          copyConstruct(begin, end, begin_);
        }
      }

      constexpr Void assign(const InitializerList<Type> list) noexcept
      {
        assign(list.begin(), list.end());
      }

      constexpr Void reserve(Size newCapacity) noexcept
      {
        if (capacity_) {
          capacity_ = Alloc::expand(begin_, capacity_, begin_ + newCapacity);
          if (capacity() < newCapacity) {
            Type* memory{Alloc::allocate(newCapacity)};
            if !consteval {
              memcpy(memory, begin_, end_ - begin_);
            } else {
              Type* src{begin_};
              Type* dst{memory};
              while (src < end_) {
                if constexpr (isNothrowMoveConstructible<Type> || !isCopyConstructible<Type>) {
                  new (dst++) Type(move(*src++));
                } else {
                  new (dst++) Type(*src++);
                }
              }
            }
            Alloc::deallocate(begin_, capacity_);
            end_ = memory + size();
            begin_ = memory;
            capacity_ = memory + newCapacity;
          }
        } else if (newCapacity) {
          begin_ = Alloc::allocate(newCapacity);
          end_ = begin_;
          capacity_ = begin_ + newCapacity;
        }
      }

      constexpr Void shrink() noexcept
      {
        if (end_ < capacity_) {
          capacity_ = Alloc::shrink(begin_, capacity_, end_);
          if (begin_ == capacity_) {
            constructDefault();
          }
        }
      }

      constexpr Void clear() noexcept
      {
        destroyRange(begin_, end_);
        end_ = begin_;
      }
/*
      constexpr iterator insert(const_iterator _Where, const _Ty& _Val) { // insert _Val at _Where
        return emplace(_Where, _Val);
      }

      constexpr iterator insert(const_iterator _Where, _Ty&& _Val) { // insert by moving _Val at _Where
        return emplace(_Where, _STD move(_Val));
      }

      constexpr iterator insert(const_iterator _Where, _CRT_GUARDOVERFLOW const size_type _Count, const _Ty& _Val) {
        // insert _Count * _Val at _Where
        const pointer _Whereptr = _Where.pointer;

        auto& _Al        = _Getal();
        auto& _My_data   = _Mypair._Myval2;
        pointer& _Mylast = _My_data._Mylast;

        const pointer _Oldfirst = _My_data._Myfirst;
        const pointer _Oldlast  = _Mylast;
        const auto _Whereoff        = static_cast<size_type>(_Whereptr - _Oldfirst);
        const auto _Unused_capacity = static_cast<size_type>(_My_data._Myend - _Oldlast);
        const bool _One_at_back     = _Count == 1 && _Whereptr == _Oldlast;
        if (_Count == 0) { // nothing to do, avoid invalidating iterators
        } else if (_Count > _Unused_capacity) { // reallocate
          const auto _Oldsize = static_cast<size_type>(_Oldlast - _Oldfirst);

          if (_Count > max_size() - _Oldsize) {
              _Xlength();
          }

          const size_type _Newsize = _Oldsize + _Count;
          size_type _Newcapacity   = _Calculate_growth(_Newsize);

          const pointer _Newvec           = _Allocate_at_least_helper(_Al, _Newcapacity);
          const pointer _Constructed_last = _Newvec + _Whereoff + _Count;

          _Reallocation_guard2 _Guard{_Al, _Newvec, _Newcapacity, _Constructed_last, _Constructed_last};
          auto& _Constructed_first = _Guard._Constructed_first;

          _Uninitialized_fill_n(_Newvec + _Whereoff, _Count, _Val, _Al);
          _Constructed_first = _Newvec + _Whereoff;

          if (_One_at_back) { // provide strong guarantee
              if constexpr (is_nothrow_move_constructible_v<_Ty> || !is_copy_constructible_v<_Ty>) {
                  _Uninitialized_move(_Oldfirst, _Oldlast, _Newvec, _Al);
              } else {
                  _Uninitialized_copy(_Oldfirst, _Oldlast, _Newvec, _Al);
              }
          } else { // provide basic guarantee
              _Uninitialized_move(_Oldfirst, _Whereptr, _Newvec, _Al);
              _Constructed_first = _Newvec;
              _Uninitialized_move(_Whereptr, _Oldlast, _Newvec + _Whereoff + _Count, _Al);
          }

          _Guard._New_begin = nullptr;
          _Change_array(_Newvec, _Newsize, _Newcapacity);
        } else if (_One_at_back) { // provide strong guarantee
          _Emplace_back_with_unused_capacity(_Val);
        } else { // provide basic guarantee
          const _Alloc_temporary2<_Alty> _Tmp_storage(_Al, _Val); // handle aliasing
          const auto& _Tmp              = _Tmp_storage._Get_value();
          const auto _Affected_elements = static_cast<size_type>(_Oldlast - _Whereptr);
          _Orphan_range(_Whereptr, _Oldlast);

          _ASAN_VECTOR_EXTEND_GUARD(static_cast<size_type>(_Oldlast - _My_data._Myfirst) + _Count);
          if (_Count > _Affected_elements) { // new stuff spills off end
            _Mylast = _Uninitialized_fill_n(_Oldlast, _Count - _Affected_elements, _Tmp, _Al);
            _Mylast = _Uninitialized_move(_Whereptr, _Oldlast, _Mylast, _Al);
            _STD fill(_Whereptr, _Oldlast, _Tmp);
          } else { // new stuff can all be assigned
            _Mylast = _Uninitialized_move(_Oldlast - _Count, _Oldlast, _Oldlast, _Al);
            _Move_backward_unchecked(_Whereptr, _Oldlast - _Count, _Oldlast);
            _STD fill_n(_Whereptr, _Count, _Tmp);
          }
          _ASAN_VECTOR_RELEASE_GUARD;
        }

        return _Make_iterator_offset(_Whereoff);
      }

      template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
      constexpr iterator insert(const_iterator _Where, _Iter _First, _Iter _Last) {
        const pointer _Whereptr = _Where.pointer;
        auto& _My_data          = _Mypair._Myval2;
        const pointer _Oldfirst = _My_data._Myfirst;
        _STD _Adl_verify_range(_First, _Last);
        auto _UFirst         = _STD _Get_unwrapped(_First);
        auto _ULast          = _STD _Get_unwrapped(_Last);
        const auto _Whereoff = static_cast<size_type>(_Whereptr - _Oldfirst);
        if constexpr (_Is_cpp17_fwd_iter_v<_Iter>) {
          const auto _Length = static_cast<Size>(_STD distance(_UFirst, _ULast));
          const auto _Count  = _STD _Convert_size<size_type>(_Length);
          _Insert_counted_range(_Where, _UFirst, _Count);
        } else if constexpr (forward_iterator<_Iter>) {
          const auto _Length = _STD _To_unsigned_like(_RANGES distance(_UFirst, _ULast));
          const auto _Count  = _Convert_size<size_type>(_Length);
          _Insert_counted_range(_Where, _UFirst, _Count);
        } else {
          _Insert_uncounted_range(_Where, _UFirst, _ULast);
        }
        return _Make_iterator_offset(_Whereoff);
      }

      constexpr iterator insert(const_iterator _Where, initializer_list<_Ty> _Ilist) {
        const pointer _Whereptr = _Where.pointer;
        auto& _My_data          = _Mypair._Myval2;
        const pointer _Oldfirst = _My_data._Myfirst;
        const auto _Whereoff = static_cast<size_type>(_Whereptr - _Oldfirst);

        const auto _Count = _Convert_size<size_type>(_Ilist.size());
        _Insert_counted_range(_Where, _Ilist.begin(), _Count);
        return _Make_iterator_offset(_Whereoff);
      }

      template <typename... Type>
      constexpr iterator emplace(const_iterator _Where, Type&&... _Val) {
        // insert by perfectly forwarding _Val at _Where
        const pointer _Whereptr = _Where.pointer;
        auto& _My_data          = _Mypair._Myval2;
        const pointer _Oldlast  = _My_data._Mylast;
        if (_Oldlast != _My_data._Myend) {
          if (_Whereptr == _Oldlast) { // at back, provide strong guarantee
              _Emplace_back_with_unused_capacity(_STD forward<Type>(_Val)...);
          } else {
              auto& _Al = _Getal();
              _Alloc_temporary2<_Alty> _Obj(_Al, _STD forward<Type>(_Val)...); // handle aliasing
              // after constructing _Obj, provide basic guarantee
              _Orphan_range(_Whereptr, _Oldlast);
              _ASAN_VECTOR_EXTEND_GUARD(static_cast<size_type>(_Oldlast - _My_data._Myfirst) + 1);
              _Alty_traits::construct(_Al, _Unfancy(_Oldlast), _STD move(_Oldlast[-1]));
              _ASAN_VECTOR_RELEASE_GUARD;
              ++_My_data._Mylast;
              _Move_backward_unchecked(_Whereptr, _Oldlast - 1, _Oldlast);
              *_Whereptr = _STD move(_Obj._Get_value());
          }

          return _Make_iterator(_Whereptr);
        }

        return _Make_iterator(_Emplace_reallocate(_Whereptr, _STD forward<Type>(_Val)...));
      }
*/
      [[nodiscard]] constexpr Bool operator==(const Vector<Type, Alloc>& right)
      {
        if (size() != right.size()) {
          return false;
        }

        return equal(begin_, end_, right._Unchecked_begin());
      }

      [[nodiscard]] constexpr auto operator<=>(const Vector<T, AllocType>& right)
          -> decltype(*begin1 <=> *begin2) noexcept
      {
        Type* begin1{begin_};
        Type* end1{end_};
        Type* begin2{right.begin_};
        Type* end2{right.end_};

        using _Memcmp_pred = _Lex_compare_three_way_memcmp_classify<decltype(_UFirst1), decltype(_UFirst2), _Cmp>;
        if constexpr (!is_void_v<_Memcmp_pred>) {
          if !consteval {
            Size size1{end_ - begin_};
            Size size2{right.end_ - right.begin_};
            Size minSize{min(size1, size2)};
            Size index{_Mismatch_vectorized<sizeof(Type)>(begin1, begin2, minSize)};
            if (index == size1) {
              return index == size2 ? StrongOrdering::Equal : StrongOrdering::Less;
            } else if (index == size2) {
              return StrongOrdering::Greater;
            } else {
              return _Comp(begin1[index], begin2[index]);
            }
          }
        }
        for (; true; ++begin1, ++begin2) {
          if (begin1 == end1) {
            return begin2 == end2 ? StrongOrdering::Equal : StrongOrdering::Less;
          } else if (begin2 == end2) {
            return StrongOrdering::Greater;
          } else if (*begin1 != *begin2) {
            return *begin1 <=> *begin2;
          }
        }
      }

      constexpr Void swap(Vector& right) noexcept
      {
        if (this != addressOf(right)) [[likely]] {
          swap(begin_, right.begin_);
          swap(end_, right.end_);
          swap(capacity_, right.capacity_);
        }
      }

      friend constexpr Void swap(Vector<T, AllocType>& left, Vector<T, AllocType>& right) noexcept
      {
        left.swap(right);
      }

      [[msvc::forceinline]] [[nodiscard]] constexpr Bool empty() const noexcept { return begin_ == end_; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Size size() const noexcept { return end_ - begin_; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Size capacity() const noexcept { return capacity_ - begin_; }

      [[msvc::forceinline]] [[nodiscard]] constexpr const Type& operator[](Size i) const noexcept { return begin_[i]; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Type& operator[](Size i) noexcept { return begin_[i]; }
      [[msvc::forceinline]] [[nodiscard]] constexpr const Type& front() const noexcept { return *begin_; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Type& front() noexcept { return *begin_; }
      [[msvc::forceinline]] [[nodiscard]] constexpr const Type& back() const noexcept { return end_[-1]; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Type& back() noexcept { return end_[-1]; }
      [[msvc::forceinline]] [[nodiscard]] constexpr const Type* data() const noexcept { return begin_; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Type* data() noexcept { return begin_; }
      [[msvc::forceinline]] [[nodiscard]] constexpr Alloc get_allocator() const noexcept { return *this; }

    private:
      Type* begin_;
      Type* end_;
      Type* capacity_;
    };
  }
}
