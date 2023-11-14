#include "deduplicator.hpp"
#include <filesystem>

Atlas::Deduplicator::Deduplicator(const std::filesystem::path& path) :
  hashes_(path)
{
  files_.deserialize(hashes_);
}

Atlas::Deduplicator::~Deduplicator()
{
  files_.serialize(hashes_);
}

void Atlas::Deduplicator::operator()(const std::filesystem::path& path)
{
  std::filesystem::is_directory(path) ? directory(path) : file(path);
}

inline void Atlas::Deduplicator::file(const std::filesystem::path& path)
{
  if (files_.has(md5_(path), std::filesystem::file_size(path))) {
    std::filesystem::remove(path);
  }
}

void Atlas::Deduplicator::directory(const std::filesystem::path& path)
{
  for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
    std::filesystem::is_directory(entry) ? directory(entry) : file(entry);
  }
  if (std::filesystem::is_empty(path)) {
    std::filesystem::remove(path);
  }
}
