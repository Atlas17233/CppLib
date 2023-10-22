#pragma once
#ifndef deduplicator_hpp
#define deduplicator_hpp

#include <filesystem>
#include <string>
#include "files.hpp"
#include "md5.hpp"

namespace Atlas
{
  class Deduplicator
  {
  public:
    Deduplicator(const std::filesystem::path& path);
    ~Deduplicator();

    bool operator()(const std::filesystem::path& path);

  private:
    Files files_;
    MD5 md5_;
    std::filesystem::path hashes_;
  };
}

#endif