#include <exception>
#include <filesystem>
#include <iostream>
#include "common/deduplicator.hpp"
#include "common/time.hpp"

int main(int argc, char* argv[])
{
  try
  {
    Atlas::Deduplicator deduplicator("hashes.txt");
    deduplicator("D:\\test");
  }
  catch(const std::exception& error)
  {
    std::cerr << Atlas::getDateTime() << " [ERROR] " << error.what() << '\n';
    return 1;
  }
  catch(...)
  {
    return 2;
  }
  return 0;
}
