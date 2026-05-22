import Atl;
import std;
import Deduplicator;
import <windows.h>;

Atl::Int Main(Atl::Int argc, Atl::Char *argv[])
{
  try
  {
    /*Deduplicator deduplicator{"C:/s/s.txt"};
    deduplicator("C:/s");*/
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
      m = std::abs((Atl::Int64)(cs[i] - cs[i - 1])) < m ? std::abs((Atl::Int64)(cs[i] - cs[i - 1])) : m;
    }
    std::cout << m << "\n---------------\n";
    Atl::Test test;
    test.insert(5 ,5 );
    test.insert(8 ,8 );
    test.insert(2 ,2 );
    test.insert(3 ,3 );
    test.insert(6 ,6 );
    test.insert(22,22);
    test.insert(7 ,7 );
    test.insert(30,30);
    test.insert(31,31);
    test.insert(15,15);
    test.insert(23,23);
    test.insert(20,20);
    test.insert(21,21);
    test.insert(11,11);
    test.insert(4 ,4 );
    test.insert(26,26);
    test.insert(14,14);
    test.insert(9 ,9 );
    test.insert(29,29);
    test.insert(24,24);
    test.insert(25,25);
    test.insert(18,18);
    test.insert(16,16);
    test.insert(1 ,1 );
    test.insert(17,17);
    test.insert(12,12);
    test.insert(19,19);
    test.insert(27,27);
    test.insert(10,10);
    test.insert(13,13);
    test.insert(28,28);
    /*test.insert(32,32);
    test.insert(33,33);
    test.insert(34,34);
    test.insert(35,35);
    test.insert(36,36);
    test.insert(37,37);
    test.insert(38,38);
    test.insert(39,39);
    test.remove(16);
    test.remove(17);
    test.remove(18);
    test.remove(19);
    test.remove(20);
    test.remove(21);
    test.remove(22);
    test.remove(23);
    test.remove(24);
    test.remove(25);
    test.remove(26);
    test.remove(27);
    test.remove(28);
    test.remove(29);
    test.remove(30);
    test.remove(31);*/
    test.printTreeHorizontal(test.nodes_[test.nodes_->size]);
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
