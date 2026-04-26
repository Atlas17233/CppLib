export module Deduplicator;

import std;
import Atl;
import FileHashes;

export class Deduplicator final
{
public:
  Deduplicator(const std::filesystem::path& path): pathHashesFile_{path} { hashes_.deserialize(pathHashesFile_); }
  ~Deduplicator() { hashes_.serialize(pathHashesFile_); }

  constexpr void operator()(const std::filesystem::path& path)
  {
    std::filesystem::is_directory(path) ? directory(path) : file(path);
  }

private:
  void file(const std::filesystem::path& path)
  {
    if (hashes_.has(Atl::MD5{Atl::File{path}}.toString(), std::filesystem::file_size(path))) {
      std::filesystem::remove(path);
    }
  }
  void directory(const std::filesystem::path& path)
  {
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
      std::filesystem::is_directory(entry) ? directory(entry) : file(entry);
    }
    if (std::filesystem::is_empty(path)) {
      std::filesystem::remove(path);
    }
  }

private:
  FileHashes hashes_;
  std::filesystem::path pathHashesFile_;
};
