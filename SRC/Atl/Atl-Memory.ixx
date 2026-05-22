export module Atl:Memory;

import :Def;

import <windows.h>;

namespace Atl
{
  constexpr ULong Reserve{0x2000};
  constexpr ULong Commit{0x1000};
  constexpr ULong Decommit{0x4000};
  constexpr ULong Alloc{Reserve | Commit};
  constexpr ULong Release{0x8000};

  constexpr ULong ReadWrite{0x4};

  constexpr Int PageSize{0x1000};

  [[msvc::forceinline]] Void releaseMemory() noexcept
  {
    ;
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr UInt64 alignUpPage(Type value) noexcept { return (UInt64)value + 4095 & 0xfffffffffffff000; }

  export {
    /*template <typename Type, typename... Types>
    requires (!isUnboundedArray<Type>) && requires(Type* _Location, Types&&... arguments) {
        ::new ((void*)_Location) Type(forward<Types>(arguments)...); // per LWG-3888
    }
    constexpr Type* construct_at(Type* _Location, Types&&... arguments) noexcept
    {
      if constexpr (isArray<Type>) {
        ::new((void*)_Location) Type();
        return __builtin_launder(_Location);
      } else {
        _MSVC_CONSTEXPR return ::new((void*)_Location) Type(forward<Types>(arguments)...);
      }
    }

    template <typename Type, typename... Types>
    constexpr Void _Construct_in_place(Type& object, Types&&... values) noexcept {
      if consteval {
        _STD construct_at(_STD addressof(object), forward<Types>(values)...);
      } else {
        ::new ((void*)_STD addressof(object)) Type(forward<Types>(values)...);
      }
    }*/

    template <typename Type>
    [[msvc::forceinline]] [[nodiscard]] Type* alloc(Size size) noexcept
    {
      Type* memory{(Type*)VirtualAlloc(nullptr, size * sizeof(Type), Alloc, ReadWrite)};
      if (!memory) [[unlikely]] {
        releaseMemory();
        return (Type*)VirtualAlloc(nullptr, size * sizeof(Type), Alloc, ReadWrite);
      }
      return memory;
    }

    template <typename Type>
    [[msvc::forceinline]] [[nodiscard]] Type* realloc(Type* memory, Size size, Size newSize) noexcept
    {
      Size sizeAllocated{alignUpPage(size * sizeof(Type))};
      Size sizeRequired{newSize * sizeof(Type)};
      if (sizeAllocated < sizeRequired) {
        Type* newMemory{(Type*)VirtualAlloc(nullptr, sizeRequired, Alloc, ReadWrite)};
        if (!newMemory) [[unlikely]] {
          releaseMemory();
          newMemory = (Type*)VirtualAlloc(nullptr, sizeRequired, Alloc, ReadWrite);
        }
        //!!!!!!!!!!!!!!!!check move or copy
        memcpy(newMemory, memory, size);
        VirtualFree(memory, 0, Release);
        return newMemory;
      }
      Size sizeUsed{alignUpPage(sizeRequired)};
      Size sizeUnused{sizeAllocated - sizeUsed};
      if (sizeUnused) {
        Void* baseAddr{(UInt64)memory + sizeUsed};
        VirtualFree(baseAddr, sizeUnused, Decommit);
        VirtualAlloc(baseAddr, sizeUnused, Commit, ReadWrite);
      }
      return memory;
    }

    [[msvc::forceinline]] Void releasePage(Void* page, Size n) noexcept
    {
      VirtualFree(page, n, Decommit);
      VirtualAlloc(page, n, Commit, ReadWrite);
    }

    [[msvc::forceinline]] Void dealloc(Void* memory) noexcept
    {
      VirtualFree(memory, 0, Release);
    }
  }
  /*constexpr Size _Asan_granularity = 8;
  constexpr Size _Asan_granularity_mask = _Asan_granularity - 1;

  template<typename Type>
  constexpr Size newAlignof = max(alignof(Type), 16);

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
    template<typename Type>
    class Allocator {
    public:
      constexpr Allocator() noexcept {}

      constexpr Allocator(const Allocator&) noexcept = default;
      template<typename Other>
      constexpr Allocator(const Allocator<Other>&) noexcept {}
      constexpr ~Allocator()                           = default;
      constexpr Allocator& operator=(const Allocator&) = default;

      [[nodiscard]] constexpr Type* allocate(const Size size) {
        //static_assert(sizeof(Type) > 0, "Type must be complete before calling allocate.");
        return static_cast<Type*>(_Allocate<newAlignof<Type>>(size * sizeof(Type)));
      }

      constexpr Void deallocate(Type* const ptr, const Size size) noexcept {
        //_STL_ASSERT(ptr != nullptr || size == 0, "null pointer cannot point to a block of non-zero size");
        // no overflow check on the following multiply; we assume _Allocate did that check
        _Deallocate<newAlignof<Type>>(ptr, size * sizeof(Type));
      }

      [[nodiscard]] constexpr DataStruct<Type> allocate_at_least(const Size size) {
        return {allocate(size), size};
      }

      static constexpr Size _Minimum_asan_allocation_alignment{_Asan_granularity};
    };
  }*/
}
