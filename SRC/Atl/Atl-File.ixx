export module Atl:File;

import :Def;
import :String;
import :Windows;

namespace Atl
{
  export
  {
    class File final: private Data<const Void>
    {
    public:
      explicit File(const std::filesystem::path& path) noexcept: Data<const Void>{nullptr, std::filesystem::file_size(path)}
      {
        if (size_ && (file_ = createFile(path.c_str(), GenericRead, FileShareRead, nullptr, OpenExisting))) {
          if (mapping_ = CreateFileMappingW(file_, nullptr, Read, 0, 0, nullptr)) {
            if (data_ = MapViewOfFile(mapping_, FileMapRead, 0, 0, 0)) {
              return;
            }
            closeHandle(mapping_);
          }
          closeHandle(file_);
        }
      }

      constexpr ~File() noexcept
      {
        if (data_) {
          UnmapViewOfFile(data_);
          closeHandle(mapping_);
          closeHandle(file_);
        }
      }

      constexpr operator ConstString() const noexcept { return ConstString{(const Char*)data_, size_}; }

    private:
      Void* file_;
      Void* mapping_;
    };
  }
}
