export module Atl:Windows;

import :Def;
import :Limits;

import <windows.h>;

namespace Atl
{
  export
  {
    using SecurityAttributes = SECURITY_ATTRIBUTES;

    enum AllocFreeType: UInt32 {
      Commit = 0x1000,
      Reserve = 0x2000,
      Alloc = 0x3000,
      Decommit = 0x4000,
      Release = 0x8000
    };

    enum Protect: UInt32 {
      PageNoAccess = 0x1,
      PageRead = 0x2,
      PageReadWrite = 0x4,
      PageWriteCopy = 0x8,
      PageExecuteNoAccess = 0x10,
      PageExecuteRead = 0x20,
      PageExecuteReadWrite = 0x40,
      PageExecuteWriteCopy = 0x80
    };

    enum ShareMode: UInt32 {
      FileShareRead = 0x1,
      FileShareWrite = 0x2,
      FileShareDelete = 0x4
    };

    enum DesiredAccess: UInt32 {
      FileMapWrite = 0x2,
      FileMapRead = 0x4,
      FileMapAllAccess = 0xf001f,
      GenericAll = 0x10000000,
      GenericExecute = 0x20000000,
      GenericWrite = 0x40000000,
      GenericRead = 0x80000000
    };

    enum CreationDisposition: UInt32 {
      CreateNew = 0x1,
      CreateAlways = 0x2,
      OpenExisting = 0x3,
      OpenAlways = 0x4,
      TruncateExisting = 0x5
    };

    enum FlagsAndAttributes: UInt32 {
      FileAttributeNormal = 0x80
    };

    constexpr Int PageSize{0x1000};

    [[msvc::forceinline]] Void* virtualAlloc(Void* address, Size size, UInt32 allocationType, UInt32 protect) noexcept
    {
      return VirtualAlloc(address, size, allocationType, protect);
    }

    [[msvc::forceinline]] Int virtualFree(Void* address, Size size, UInt32 freeType) noexcept
    {
      return VirtualFree(address, size, freeType);
    }

    [[msvc::forceinline]] [[nodiscard]] Void* createFile(
        const std::filesystem::path& fileName,
        UInt32 desiredAccess,
        UInt32 shareMode,
        SecurityAttributes* securityAttributes,
        UInt32 creationDisposition,
        UInt32 flagsAndAttributes = FileAttributeNormal,
        Void* templateFile = nullptr) noexcept
    {
      templateFile = CreateFileW(
          fileName.c_str(),
          desiredAccess,
          shareMode,
          securityAttributes,
          creationDisposition,
          flagsAndAttributes,
          templateFile);
      return templateFile != (Void*)Max<UInt64> ? templateFile : nullptr;
    }

    [[msvc::forceinline]] [[nodiscard]] Void* createFileMapping(
        Void* file,
        SecurityAttributes* fileMappingAttributes,
        UInt32 protect,
        UInt32 maximumSizeHigh,
        UInt32 maximumSizeLow,
        const CharW* name) noexcept
    {
      return CreateFileMappingW(file, fileMappingAttributes, protect, maximumSizeHigh, maximumSizeLow, name);
    }

    [[msvc::forceinline]] [[nodiscard]] Void* mapViewOfFile(
        Void* fileMappingObject,
        UInt32 desiredAccess,
        UInt32 fileOffsetHigh,
        UInt32 fileOffsetLow,
        Size numberOfBytesToMap) noexcept
    {
      return MapViewOfFile(fileMappingObject, desiredAccess, fileOffsetHigh, fileOffsetLow, numberOfBytesToMap);
    }

    [[msvc::forceinline]] Int unmapViewOfFile(const Void* baseAddress) noexcept
    {
      return UnmapViewOfFile(baseAddress);
    }

    [[msvc::forceinline]] Int closeHandle(Void* object) noexcept
    {
      return CloseHandle(object);
    }
  }
}