export module Atl:File;

import :Def;
import :String;
import :Windows;

import "Macros";

namespace Atl
{
  export
  {
    class File final
    {
    public:
      explicit File(const std::filesystem::path& path) noexcept: data_{}, size_{std::filesystem::file_size(path)}
      {
        if (size_) {
          file_ = createFile(path.c_str(), GenericRead, FileShareRead, nullptr, OpenExisting, FileAttributeNormal, nullptr);
          if (file_ != invalidHandle) {
            mapping_ = createFileMapping(file_, nullptr, PageRead, 0, 0, nullptr);
            if (mapping_) {
              data_ = mapViewOfFile(mapping_, FileMapRead, 0, 0, 0);
              if (data_) {
                return;
              }
              closeHandle(mapping_);
            }
            closeHandle(file_);
          }
        }
      }

      constexpr ~File() noexcept
      {
        if (data_) {
          unmapViewOfFile(data_);
          closeHandle(mapping_);
          closeHandle(file_);
        }
      }

      constexpr operator ConstString() const noexcept { return ConstString{(const Char*)data_, size_}; }

    private:
      const Void* data_;
      Size size_;
      Void* file_;
      Void* mapping_;
    };
  }
}
