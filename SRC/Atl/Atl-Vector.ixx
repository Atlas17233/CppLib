export module Atl:Vector;

import :Def;

namespace Atl
{
  export
  {
    /*template<typename T, Size size_>
    requires (size_ > 0)
    struct Array
    {
      //constexpr void fill(const T& value) noexcept { fill(data_, size_, value); }
      //consteval void swap(Array& array) noexcept;

      [[nodiscard]] consteval T* begin() noexcept { return data_; }
      [[nodiscard]] consteval const T* begin() const noexcept { return data_; }

      [[nodiscard]] consteval T* end() noexcept { return data_ + size_; }
      [[nodiscard]] consteval const T* end() const noexcept { return data_ + size_; }

      [[nodiscard]] consteval Bool empty() const noexcept { return false; }
      [[nodiscard]] consteval Size size() const noexcept { return size_; }
      [[nodiscard]] consteval Size maxSize() const noexcept { return size_; }

      [[nodiscard]] consteval T& operator[](Size i) noexcept { return data_[i]; }
      [[nodiscard]] consteval T& front() noexcept { return data_[0]; }
      [[nodiscard]] consteval T& back() noexcept { return data_[size_ - 1]; }
      [[nodiscard]] consteval T* data() noexcept { return data_; }
      [[nodiscard]] consteval const T& operator[](Size i) const noexcept { return data_[i]; }
      [[nodiscard]] consteval const T& front() const noexcept { return data_[0]; }
      [[nodiscard]] consteval const T& back() const noexcept { return data_[size_ - 1]; }
      [[nodiscard]] consteval const T* data() const noexcept { return data_; }

      T data_[size_];
    }*/;
  }
}
