#include <windows.h>

import Atl;
import std;
import Deduplicator;

Atl::Int Main(Atl::Int argc, Atl::Char *argv[])
{
  try
  {
    //Deduplicator deduplicator{"C:/s/s.txt"};
    //deduplicator("C:/s");
    std::cout << Atl::CRC32{Atl::File{"empty.txt"}} << '\n';
    std::cout << Atl::CRC32{""} << '\n';
    std::cout << Atl::CRC32{Atl::File{"loop.txt"}} << '\n';
    std::cout << Atl::CRC32{"1234567890123456789012345678901234"} << '\n';
    std::cout << Atl::CRC32{"123"} << '\n';
    std::cout << Atl::CRC32{Atl::File{"123.txt"}} << '\n';
    std::cout << Atl::CRC32{Atl::File{"128.txt"}} << '\n';
    //std::cout << Atl::CRC32{Atl::File{"D:/Shared/JUR-286ch.mp4"}} << '\n';

    std::cout << Atl::MD5{Atl::File{"empty.txt"}} << '\n';
    std::cout << Atl::MD5{""} << '\n';
    std::cout << Atl::MD5{Atl::File{"loop.txt"}} << '\n';
    std::cout << Atl::MD5{"1234567890123456789012345678901234"} << '\n';
    std::cout << Atl::MD5{"123"} << '\n';
    std::cout << Atl::MD5{Atl::File{"123.txt"}} << '\n';
    std::cout << Atl::MD5{Atl::File{"128.txt"}} << '\n';
    //std::cout << Atl::MD5{Atl::File{"D:/Shared/JUR-286ch.mp4"}} << '\n';

    std::cout << Atl::SHA256{Atl::File{"empty.txt"}} << '\n';
    std::cout << Atl::SHA256{""} << '\n';//e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
    std::cout << Atl::SHA256{Atl::File{"loop.txt"}} << '\n';
    std::cout << Atl::SHA256{"1234567890123456789012345678901234"} << '\n';//6f427b79c3326fd2c717e093fd4bcabc1b2fa34b0f345821961fc2bd9204a9d0
    std::cout << Atl::SHA256{"123"} << '\n';//a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3
    std::cout << Atl::SHA256{Atl::File{"123.txt"}} << '\n';
    std::cout << Atl::SHA256{Atl::File{"128.txt"}} << '\n';//639c89bb7f5f4cb4cf6714ee5ec108f63aba682b4b45d9a7bf895bf7f13f7703
    //std::cout << Atl::SHA256{Atl::File{"D:/Shared/JUR-286ch.mp4"}} << '\n';
    char* cs[10000];
    cs[0] = new char[25];
    int m = 1000;
    for (int i{1}; i < 10000; ++i) {
      cs[i] = new char[25];
      m = std::abs((long long)(cs[i] - cs[i - 1])) < m ? std::abs((long long)(cs[i] - cs[i - 1])) : m;
    }
    std::cout << m;
  }
  catch (const Atl::Exception &error)
  {
    std::cerr << Atl::getDateTime() << " [ERROR] " << error.what() << '\n';
    return -1;
  }
  catch (...)
  {
    std::cerr << Atl::getDateTime() << " [ERROR] " << "Unknow!\n";
    return -2;
  }
  return 0;
}
