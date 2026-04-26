export module Atl:Memory;

import :Common;
import :Def;
import :Math;

import <new.h>;

namespace Atl
{
  Int _newHandle(Size size)
  {
    return 0;
  }
  //_set_new_handler(_newHandle);

  /*constexpr Size _Asan_granularity = 8;
  constexpr Size _Asan_granularity_mask = _Asan_granularity - 1;

  template<typename T>
  constexpr Size newAlignof = max(alignof(T), 16);

  template<Size _Align>
  constexpr Void _Deallocate(Void* ptr, Size _Bytes) noexcept {
    if (isConstEval()) {
      ::operator delete(ptr);
      return;
    }

    if constexpr (_Align > 16) {
      Size _Passed_align = _Align;
      if (_Bytes >= _Big_allocation_threshold) {
        // boost the alignment of big allocations to help autovectorization
        _Passed_align = (_STD max) (_Align, _Big_allocation_alignment);
      }
      ::operator delete(ptr, _Bytes, align_val_t{_Passed_align});
    } else {
      if (_Bytes >= _Big_allocation_threshold) {
        // boost the alignment of big allocations to help autovectorization
        _Adjust_manually_vector_aligned(ptr, _Bytes);
      }
      ::operator delete(ptr, _Bytes);
    }
  }

  export
  {
    template<typename T>
    class Allocator {
    public:
      constexpr Allocator() noexcept {}

      constexpr Allocator(const Allocator&) noexcept = default;
      template<typename Other>
      constexpr Allocator(const Allocator<Other>&) noexcept {}
      constexpr ~Allocator()                           = default;
      constexpr Allocator& operator=(const Allocator&) = default;

      [[nodiscard]] constexpr T* allocate(const Size size) {
        //static_assert(sizeof(T) > 0, "T must be complete before calling allocate.");
        return static_cast<T*>(_Allocate<newAlignof<T>>(size * sizeof(T)));
      }

      constexpr Void deallocate(T* const ptr, const Size size) noexcept {
        //_STL_ASSERT(ptr != nullptr || size == 0, "null pointer cannot point to a block of non-zero size");
        // no overflow check on the following multiply; we assume _Allocate did that check
        _Deallocate<newAlignof<T>>(ptr, size * sizeof(T));
      }

      [[nodiscard]] constexpr Data<T> allocate_at_least(const Size size) {
        return {allocate(size), size};
      }

      static constexpr Size _Minimum_asan_allocation_alignment{_Asan_granularity};
    };
  }*/
}
