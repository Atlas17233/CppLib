#include "deduplicator.hpp"
#include <exception>
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

bool Atlas::Deduplicator::operator()(const std::filesystem::path& path)
{
  std::filesystem::directory_iterator directory(path);
  for (const std::filesystem::directory_entry& entry : directory) {
    if (std::filesystem::is_directory(entry)) {
      operator()(entry);
    } else {
      files_.search(md5_(entry), std::filesystem::file_size(entry));
    }
  }
}
