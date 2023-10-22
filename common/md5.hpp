#pragma once
#ifndef md5_hpp
#define md5_hpp

#include <filesystem>
#include <string>
#include "def.hpp"

namespace Atlas
{
  class MD5
  {
  public:
    std::string operator()(const std::filesystem::path& path);

  private:
    void process(const Byte block[64]);

  private:
    Uint32 state_[4];
    Uint32 count_[2];
    Byte buffer_[64];
  };
}

#endif
