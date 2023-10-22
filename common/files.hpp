#pragma once
#ifndef files_hpp
#define files_hpp

#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "def.hpp"

namespace Atlas
{
  class Files
  {
  public:
    void deserialize(const std::filesystem::path& path);
    bool has(const std::string& hash, Size size);
    void serialize(const std::filesystem::path& path) const;

  private:
    std::map<const std::string, std::vector<Size>> files_;
  };
}

#endif
