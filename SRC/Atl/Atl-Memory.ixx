export module Atl:Memory;

import :Def;
import :Windows;

namespace Atl
{
  constexpr UInt32 Capacity{0x200000};

  Void releaseMemory() noexcept
  {
    ;
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr Type alignUpPage(Type value) noexcept
  {
    return (Type)((UInt64)value + 0xfff & 0xfffffffffffff000);
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr Type alignDownPage(Type value) noexcept
  {
    return (Type)((UInt64)value & 0xfffffffffffff000);
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr Type alignUpGranularity(Type value) noexcept
  {
    return (Type)((UInt64)value + 0xffff & 0xffffffffffff0000);
  }

  template <typename Type>
  [[msvc::forceinline]] [[nodiscard]] constexpr Type alignDownGranularity(Type value) noexcept
  {
    return (Type)((UInt64)value & 0xffffffffffff0000);
  }

  [[msvc::forceinline]] [[nodiscard]] Void* reserve(Size size) noexcept
  {
    return virtualAlloc(nullptr, size, Reserve, PageReadWrite);
  }

  [[msvc::forceinline]] Void commit(Void* memory, Size size) noexcept
  {
    if (!virtualAlloc(memory, size, Commit, PageReadWrite)) [[unlikely]] {
      releaseMemory();
      virtualAlloc(memory, size, Commit, PageReadWrite);
    }
  }

  [[msvc::forceinline]] Void decommit(Void* memory, Size size) noexcept
  {
    virtualFree(memory, size, Decommit);
  }

  [[msvc::forceinline]] Void release(Void* memory) noexcept
  {
    virtualFree(memory, 0, Release);
  }

  [[msvc::forceinline]] [[nodiscard]] Void* alloc(Size size) noexcept
  {
    Void* memory{virtualAlloc(nullptr, size, Alloc, PageReadWrite)};
    if (!memory) [[unlikely]] {
      releaseMemory();
      return virtualAlloc(nullptr, size, Alloc, PageReadWrite);
    }
    return memory;
  }

  [[msvc::forceinline]] Void recommit(Void* memory, Size size) noexcept
  {
    decommit(memory, size);
    commit(memory, size);
  }

  [[msvc::forceinline]] [[nodiscard]] Void* realloc(Void* memory, Void* capacity, Void* newCapacity) noexcept
  {
    capacity = alignUpPage(capacity);
    newCapacity = alignUpPage(newCapacity);
    if (capacity > newCapacity) {
      recommit(newCapacity, (UInt64)capacity - (UInt64)newCapacity);
    }
    return capacity;
  }
}
