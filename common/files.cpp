#include "files.hpp"
#include <exception>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "def.hpp"

void Atlas::Files::deserialize(const std::filesystem::path& path)
{
  std::ifstream files(path);
  if (!files.is_open()) {
    throw std::exception(("Hashes file " + path.string() + " open failed!").c_str());
  }
  std::string s;
  std::istringstream line;
  while (std::getline(files, s)) {
    line.str(s);
    std::string hash;
    line >> hash;
    std::vector<Size> sizes;
    Size size;
    while (line >> size) {
      sizes.push_back(size);
    }
    files_[hash] = sizes;
    line.clear();
  }
}

bool Atlas::Files::has(const std::string& hash, Size size)
{
  if (files_.contains(hash)) {
    for (const Size& s : files_[hash]) {
      if (size == s) {
        return true;
      }
    }
    files_[hash].push_back(size);
    return false;
  }
  files_[hash] = std::vector<Size>{ size };
  return false;
}

void Atlas::Files::serialize(const std::filesystem::path& path) const
{
  std::ofstream files(path);
  if (!files.is_open()) {
    throw std::exception(("Hashes file " + path.string() + " open failed!").c_str());
  }
  for (const std::pair<const std::string, std::vector<Size>>& file : files_) {
    std::string line = file.first;
    for (const int size : file.second) {
      line += ' ' +  std::to_string(size);
    }
    files << line << std::endl;
  }
}
