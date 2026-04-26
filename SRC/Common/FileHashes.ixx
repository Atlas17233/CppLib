export module FileHashes;

import std;
import Atl;

export class FileHashes final
{
public:
  void deserialize(const std::filesystem::path &path)
  {
    std::ifstream files{path};
    if (!files.is_open()) {
      throw Atl::Exception{"Input file " + path.string() + " open failed!"};
    }
    std::string s;
    std::istringstream line;
    while (std::getline(files, s)) {
      line.str(s);
      std::string hash;
      line >> hash;
      std::vector<Atl::Size> sizes;
      Atl::Size size;
      while (line >> size) {
        sizes.push_back(size);
      }
      hashes_[hash] = sizes;
      line.clear();
    }
  }
  bool has(const std::string &hash, Atl::Size size)
  {
    if (hashes_.contains(hash)) {
      for (const Atl::Size &s : hashes_[hash]) {
        if (size == s) {
          return true;
        }
      }
      hashes_[hash].push_back(size);
    } else {
      hashes_[hash] = std::vector<Atl::Size>{ size };
    }
    return false;
  }
  void serialize(const std::filesystem::path &path) const
  {
    std::ofstream files{path};
    if (!files.is_open()) {
      throw Atl::Exception{"Output file " + path.string() + " open failed!"};
    }
    for (const std::pair<const std::string, std::vector<Atl::Size>> &file : hashes_) {
      std::string line{file.first};
      for (const Atl::Size size : file.second) {
        line += ' ' +  std::to_string(size);
      }
      files << line << '\n';
    }
  }

private:
  std::map<const std::string, std::vector<Atl::Size>> hashes_;
};
