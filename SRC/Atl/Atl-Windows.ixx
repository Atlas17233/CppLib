export module Atl:Windows;

import :Def;
import :Limits;

import <windows.h>;

import "Macros";

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

    constexpr Size PageSize{0x1000};

  }
}
