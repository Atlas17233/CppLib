export module Atl:Windows;

import :Def;
import :Limits;

import <windows.h>;

namespace Atl
{
  constexpr UInt32 Reserve{0x2000};
  constexpr UInt32 Commit{0x1000};
  constexpr UInt32 Decommit{0x4000};
  constexpr UInt32 Alloc{Reserve | Commit};
  constexpr UInt32 Release{0x8000};

  constexpr UInt32 Read{0x2};
  constexpr UInt32 ReadWrite{0x4};

  constexpr UInt32 GenericRead{0x80000000};
  constexpr UInt32 FileShareRead{0x1};
  constexpr UInt32 OpenExisting{0x3};
  constexpr UInt32 FileAttributeNormal{0x80};
  constexpr UInt32 FileMapRead{0x4};

  constexpr Int PageSize{0x1000};

  [[msvc::forceinline]] [[nodiscard]] Void* createFile(const CharW* lpFileName,
      UInt32 dwDesiredAccess,
      UInt32 dwShareMode,
      SECURITY_ATTRIBUTES* lpSecurityAttributes,
      UInt32 dwCreationDisposition,
      UInt32 dwFlagsAndAttributes = FileAttributeNormal,
      Void* hTemplateFile = nullptr) noexcept
  {
    Void* file{CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile)};
    return file != (Void*)Max<UInt64> ? file : nullptr;
  }

  [[msvc::forceinline]] Int closeHandle(Void* hObject) noexcept
  {
    return CloseHandle(hObject);
  }

}