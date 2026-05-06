module;
#include <windows.h>
export module Atl:File;

import :Def;
import :String;

namespace Atl
{
  export
  {
    class File final: private Data<const Void>
    {
    public:
      explicit File(const std::filesystem::path& path) noexcept:
        Data<const Void>{nullptr, std::filesystem::file_size(path)}
      {
        if (size_) {
          file_ = CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
          if (file_ != INVALID_HANDLE_VALUE) {
            mapping_ = CreateFileMappingW(file_, NULL, PAGE_READONLY, 0, 0, NULL);
            if (mapping_) {
              data_ = MapViewOfFile(mapping_, FILE_MAP_READ, 0, 0, 0);
              if (data_) {
                return;
              }
              CloseHandle(mapping_);
            }
            CloseHandle(file_);
          }
        }
      }

      constexpr ~File() noexcept
      {
        if (data_) {
          UnmapViewOfFile(data_);
          CloseHandle(mapping_);
          CloseHandle(file_);
        }
      }

      constexpr operator ConstString() const noexcept { return ConstString{(const Char*)data(), size()}; }

    private:
      HANDLE file_;
      HANDLE mapping_;
    };
  }
}
