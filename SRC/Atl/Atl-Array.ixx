export module Atl:Array;

import :Def;

namespace Atl
{
  /*template<typename T, Size size_>
  class ConstArrayIterator
  {
  public:
    constexpr ConstArrayIterator() noexcept {}
    constexpr explicit ConstArrayIterator(const T* ptr) noexcept: _ptr{ptr} {}

    [[nodiscard]] constexpr const T& operator*() const noexcept { return *_ptr; }
    [[nodiscard]] constexpr const T* operator->() const noexcept { return _ptr; }
    [[nodiscard]] constexpr const T& operator[](Size offset) const noexcept { return _ptr[offset]; }
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
    const T* _ptr;
  };

  template<typename T, Size size_>
  class ArrayIterator
  {
  public:
    constexpr ArrayIterator() noexcept {}
    constexpr explicit ArrayIterator(T* ptr) noexcept: ArrayIterator<T, size_>{ptr} {}

    [[nodiscard]] constexpr T& operator*() const noexcept { return *_ptr; }
    [[nodiscard]] constexpr T* operator->() const noexcept { return _ptr; }
    [[nodiscard]] constexpr T& operator[](Size offset) const noexcept { return _ptr[offset]; }
    //[[nodiscard]] constexpr const T& operator[](Size offset) const noexcept { return _ptr[offset]; }
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
    T* _ptr;
  };
*/
  export
  {
    template<typename T, Size size_>
    requires(size_ > 0)
    class Array
    {
    public:
      //constexpr void fill(const T& value) noexcept { fill(data_, size_, value); }
      //consteval void swap(Array& array) noexcept;

      [[nodiscard]] consteval const T* begin() const noexcept { return data_; }
      [[nodiscard]] consteval T* begin() noexcept { return data_; }

      [[nodiscard]] consteval const T* end() const noexcept { return data_ + size_; }
      [[nodiscard]] consteval T* end() noexcept { return data_ + size_; }

      [[nodiscard]] consteval Bool empty() const noexcept { return False; }
      [[nodiscard]] consteval Size size() const noexcept { return size_; }
      //[[nodiscard]] consteval Size maxSize() const noexcept { return size_; }

      [[nodiscard]] consteval const T& operator[](Size i) const noexcept { return data_[i]; }
      [[nodiscard]] consteval T& operator[](Size i) noexcept { return data_[i]; }
      [[nodiscard]] consteval const T& front() const noexcept { return data_[0]; }
      [[nodiscard]] consteval T& front() noexcept { return data_[0]; }
      [[nodiscard]] consteval const T& back() const noexcept { return data_[size_ - 1]; }
      [[nodiscard]] consteval T& back() noexcept { return data_[size_ - 1]; }
      [[nodiscard]] consteval const T* data() const noexcept { return data_; }
      [[nodiscard]] consteval T* data() noexcept { return data_; }

    protected:
      T data_[size_];
    };
  }
}
