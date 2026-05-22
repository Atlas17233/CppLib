export module Atl:Array;

import :Def;

namespace Atl
{
  /*template<typename Type, Size size_>
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

  template<typename Type, Size size_>
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
    template <typename Type, Size size_> requires(size_ > 0)
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

    protected:
      Type data_[size_];
    };
  }
}
