export module Atl:Memory;

import :Def;

import <windows.h>;

namespace Atl
{
  constexpr UInt32 Reserve{0x2000};
  constexpr UInt32 Commit{0x1000};
  constexpr UInt32 Decommit{0x4000};
  constexpr UInt32 Alloc{Reserve | Commit};
  constexpr UInt32 Release{0x8000};

  constexpr UInt32 ReadWrite{0x4};

  constexpr Int PageSize{0x1000};

  Void releaseMemory() noexcept
  {
    ;
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr UInt64 alignUpPage(Type value) noexcept {
    return (UInt64)(value) + 0xfff & 0xfffffffffffff000;
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr UInt64 alignDownPage(Type value) noexcept {
    return (UInt64)(value) & 0xfffffffffffff000;
  }

  export {
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
}
