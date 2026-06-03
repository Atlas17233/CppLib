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
  Void* const InvalidHandleValue{(Void*)Max<UInt64>};
  constexpr UInt32 FileMapRead{0x4};

  constexpr Int PageSize{0x1000};
}