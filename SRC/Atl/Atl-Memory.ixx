export module Atl:Memory;

import :Def;
import :Type;
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

  export template <typename Container>
  [[nodiscard]] constexpr auto begin(Container& container) noexcept -> decltype(container.begin())
  {
    return container.begin();
  }

  export template <typename Container>
  [[nodiscard]] constexpr auto begin(const Container& container) noexcept -> decltype(container.begin())
  {
    return container.begin();
  }

  export template <typename Container>
  [[nodiscard]] constexpr auto end(Container& container) noexcept -> decltype(container.end())
  {
    return container.end();
  }

  export template <typename Container>
  [[nodiscard]] constexpr auto end(const Container& container) noexcept -> decltype(container.end())
  {
    return container.end();
  }

  export template <typename Type, Size size>
  [[nodiscard]] constexpr Type* begin(Type (&array)[size]) noexcept
  {
    return array;
  }

  export template <typename Type, Size size>
  [[nodiscard]] constexpr Type* end(Type (&array)[size]) noexcept
  {
    return array + size;
  }

  template <typename Type>
  constexpr Void destroyRange(Type* begin, const Type* end) noexcept
  {
    if constexpr (!isTriviallyDestructible<Type>) {
      for (; begin < end; ++begin) {
        if constexpr (isArray<Type>) {
          destroyRange(*begin, *begin + extent<Type>);
        } else {
          begin->~Type();
        }
      }
    }
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void defaultConstruct(Type* begin, Type* end)
  {
    if !consteval {
      memset(begin, 0, (UInt8*)end - (UInt8*)begin);
    } else {
      while (begin < end) {
        new (begin++) Type();
      }
    }
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void defaultConstruct(Type* begin, Size size)
  {
    defaultConstruct(begin, begin + size);
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void fillConstructRuntime(Type* begin, Type* end, Size size, const Type& value)
  {
    if !consteval {
      if constexpr (sizeof(*begin) == 1) {
        memset(begin, value, size);
        return;
      } else {
        if (isZero(value)) {
          memset(begin, 0, (UInt8*)end - (UInt8*)begin);
          return;
        }
      }
    }
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void fillConstruct(Type* begin, Type* end, const Type& value)
  {
    fillConstructRuntime(begin, end, end - begin, value);
    while (begin < end) {
      new (begin++) Type(value);
    }
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void fillConstruct(Type* begin, Size size, const Type& value)
  {
    fillConstructRuntime(begin, begin + size, size, value);
    while (begin < end) {
      new (begin++) Type(value);
    }
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void fillConstructUnsafe(Type* begin, Type* end, const Type& value)
  {
    fillConstructRuntime(begin, end, end - begin, value);
    do {
      new (begin) Type(value);
    } while (++begin < end);
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void fillConstructUnsafe(Type* begin, Size size, const Type& value)
  {
    fillConstructRuntime(begin, begin + size, size, value);
    do {
      new (begin) Type(value);
    } while (++begin < end);
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void copyConstruct(const Type* begin, const Type* end, Type* dest)
  {
    if !consteval {
      memcpy(dest, begin, (UInt8*)end - (UInt8*)begin);
    } else {
      while (begin < end) {
        new (dest++) Type(*begin++);
      }
    }
  }

  template <typename Type>
  [[msvc::forceinline]] constexpr Void copyConstruct(const Type* begin, Size size, Type* dest)
  {
    copyConstruct(begin, begin + size, dest);
  }

  template <NotPointerType Pointer>
  constexpr Pointer refancyMaybeNull(typename Pointer::Type* pointer) noexcept {
    return pointer == nullptr ? Pointer{} : addressOf(*pointer);
  }

  template <PointerType Ptr>
  constexpr Ptr refancyMaybeNull(Ptr pointer) noexcept {
    return pointer;
  }

}
