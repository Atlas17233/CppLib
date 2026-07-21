export module Atl:Array;

import :Compare;
import :Def;
import :Type;

import "Macros";

namespace Atl
{
  /*template <typename Type, Size size_>
  class ConstArrayIterator
  {
  public:
    constexpr ConstArrayIterator() noexcept {}
    constexpr explicit ConstArrayIterator(const Type* ptr) noexcept: _ptr{ptr} {}

    [[nodiscard]] constexpr const Type& operator*() const noexcept { return *_ptr; }
    [[nodiscard]] constexpr const Type* operator->() const noexcept { return _ptr; }
    [[nodiscard]] constexpr const Type& operator[](Size offset) const noexcept { return _ptr[offset]; }
    constexpr ConstArrayIterator& operator++() noexcept { ++_ptr; return *this; }
    constexpr ConstArrayIterator& operator--() noexcept { --_ptr; return *this; }
    constexpr ConstArrayIterator operator++(int) noexcept { ConstArrayIterator tmp{_ptr}; ++_ptr; return tmp; }
    constexpr ConstArrayIterator operator--(int) noexcept { ConstArrayIterator tmp{_ptr}; --_ptr; return tmp; }
    constexpr ConstArrayIterator& operator+=(Int64 offset) noexcept { _ptr += offset; return *this; }
    constexpr ConstArrayIterator& operator-=(Int64 offset) noexcept { _ptr -= offset; return *this; }
    [[nodiscard]] constexpr Int64 operator-(const ConstArrayIterator& iter) const noexcept { return _ptr - iter._ptr; }
    [[nodiscard]] constexpr ConstArrayIterator operator+(Int64 offset) const noexcept { return ConstArrayIterator{_ptr + offset}; }
    [[nodiscard]] constexpr ConstArrayIterator operator-(Int64 offset) const noexcept { return ConstArrayIterator{_ptr - offset}; }
    [[nodiscard]] constexpr friend ConstArrayIterator operator+(Int64 offset, const ConstArrayIterator& iter) noexcept { return ConstArrayIterator{iter._ptr + offset}; }
    [[nodiscard]] constexpr bool operator==(const ConstArrayIterator& iter) const noexcept { return _ptr == iter._ptr; }
    //[[nodiscard]] constexpr strong_ordering operator<=>(const ConstArrayIterator& iter) const noexcept { return _ptr <=> iter._ptr; }
  private:
    const Type* _ptr;
  };

  template <typename Type, Size size_>
  class ArrayIterator
  {
  public:
    constexpr ArrayIterator() noexcept {}
    constexpr explicit ArrayIterator(Type* ptr) noexcept: ArrayIterator<Type, size_>{ptr} {}

    [[nodiscard]] constexpr Type& operator*() const noexcept { return *_ptr; }
    [[nodiscard]] constexpr Type* operator->() const noexcept { return _ptr; }
    [[nodiscard]] constexpr Type& operator[](Size offset) const noexcept { return _ptr[offset]; }
    //[[nodiscard]] constexpr const Type& operator[](Size offset) const noexcept { return _ptr[offset]; }
    constexpr ArrayIterator& operator++() noexcept { ++_ptr; return *this; }
    constexpr ArrayIterator& operator--() noexcept { --_ptr; return *this; }
    constexpr ArrayIterator operator++(int) noexcept { ArrayIterator tmp{_ptr}; ++_ptr; return tmp; }
    constexpr ArrayIterator operator--(int) noexcept { ArrayIterator tmp{_ptr}; --_ptr; return tmp; }
    constexpr ArrayIterator& operator+=(Int64 offset) noexcept { _ptr += offset; return *this; }
    constexpr ArrayIterator& operator-=(Int64 offset) noexcept { _ptr -= offset; return *this; }
    [[nodiscard]] constexpr Int64 operator-(const ArrayIterator& iter) const noexcept { return _ptr - iter._ptr; }
    [[nodiscard]] constexpr ArrayIterator operator+(Int64 offset) const noexcept { return ArrayIterator{_ptr + offset}; }
    [[nodiscard]] constexpr ArrayIterator operator-(Int64 offset) const noexcept { return ArrayIterator{_ptr - offset}; }
    [[nodiscard]] constexpr friend ArrayIterator operator+(Int64 offset, const ArrayIterator& iter) noexcept { return ArrayIterator{iter._ptr + offset}; }
    [[nodiscard]] constexpr bool operator==(const ArrayIterator& iter) const noexcept { return _ptr == iter._ptr; }
    //[[nodiscard]] constexpr strong_ordering operator<=>(const ArrayIterator& iter) const noexcept { return _ptr <=> iter._ptr; }
  private:
    Type* _ptr;
  };
*/
  export
  {
    template <typename Type, Size size_>
    requires (size_ > 0)
    class Array
    {
    public:
      //constexpr void fill(const Type& value) noexcept { fill(data_, size_, value); }
      //consteval void swap(Array& array) noexcept;

      [[nodiscard]] constexpr const Type* begin() const noexcept { return data_; }
      [[nodiscard]] constexpr Type* begin() noexcept { return data_; }

      [[nodiscard]] constexpr const Type* end() const noexcept { return data_ + size_; }
      [[nodiscard]] constexpr Type* end() noexcept { return data_ + size_; }

      [[nodiscard]] consteval Bool empty() const noexcept { return false; }
      [[nodiscard]] consteval Size size() const noexcept { return size_; }

      [[nodiscard]] constexpr const Type& operator[](Size i) const noexcept { return data_[i]; }
      [[nodiscard]] constexpr Type& operator[](Size i) noexcept { return data_[i]; }
      [[nodiscard]] constexpr const Type& front() const noexcept { return data_[0]; }
      [[nodiscard]] constexpr Type& front() noexcept { return data_[0]; }
      [[nodiscard]] constexpr const Type& back() const noexcept { return data_[size_ - 1]; }
      [[nodiscard]] constexpr Type& back() noexcept { return data_[size_ - 1]; }
      [[nodiscard]] constexpr const Type* data() const noexcept { return data_; }
      [[nodiscard]] constexpr Type* data() noexcept { return data_; }

      template <Size sizeRight>
      requires (sizeRight > 0)
      [[nodiscard]] constexpr Bool operator==(const Array<Type, sizeRight>& right) const noexcept
      {
        if (size() != right.size()) {
          return false;
        }
        if constexpr (hasUniqueObjectRepresentations<Type>) {
          if !consteval {
            return !memcmp(begin(), right.begin(), (UInt8*)end() - (UInt8*)begin());
          }
        }
        Type* begin1{begin()};
        Type* end1{end()};
        Type* begin2{right.begin()};
        for (; begin1 < end1; ++begin1, ++begin2) {
          if (*begin1 != *begin2) {
            return false;
          }
        }
        return true;
      }

      template <Size sizeRight>
      requires (sizeRight > 0)
      [[nodiscard]] constexpr auto operator<=>(const Array<Type, sizeRight>& right) const noexcept -> decltype(declvalue<Type&>() <=> declvalue<Type&>())
      {
        if constexpr (isIntegral<Type>) {
          if !consteval {
            Size index{mismatchVectorized<sizeof(Type)>(begin(), right.begin(), min(size_, right.size_))};
            if (index == size_) {
              return index == right.size_ ? StrongOrdering::equal : StrongOrdering::less;
            } else if (index == right.size_) {
              return StrongOrdering::greater;
            } else {
              return data_[index] <=> right.data_[index];
            }
          }
        }
        Type* begin1{data_};
        Type* end1{data_ + size_};
        Type* begin2{right.data_};
        Type* end2{right.data_ + size_};
        for (;; ++begin1, ++begin2) {
          if (begin1 == end1) {
            return begin2 == end2 ? StrongOrdering::equal : StrongOrdering::less;
          } else if (begin2 == end2) {
            return StrongOrdering::greater;
          } else if (*begin1 != *begin2) {
            return *begin1 <=> *begin2;
          }
        }
      }

    protected:
      Type data_[size_];
    };
  }
}
