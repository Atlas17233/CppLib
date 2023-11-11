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
  std::filesystem::directory_iterator directory(path);
  for (const std::filesystem::directory_entry& entry : directory) {
    if (std::filesystem::is_directory(entry)) {
      operator()(entry);
    } else {
      if (files_.has(md5_(entry), std::filesystem::file_size(entry))) {
        std::filesystem::remove(entry);
      }
    }
  }
}
