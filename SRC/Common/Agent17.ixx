export module Agent17;

import Atl;
import std;

export Atl::Void run(const std::filesystem::path& path)
{
  Atl::Char digestPath[]{"E:\\Game\\Agent17\\Agent17\\0.bin"};
  Atl::File data{path};

  for (Atl::Int i{}; i < 10; ++i) {
    digestPath[28] = i + 48;
    Atl::File digests{digestPath};

  }
}