#include "md5.hpp"
#include <exception>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string> 
#include "def.hpp"

constexpr Atlas::Size BUFFER_SIZE = 1024;
constexpr Atlas::Byte HEX[16] = {
  '0', '1', '2', '3',
  '4', '5', '6', '7',
  '8', '9', 'A', 'B',
  'C', 'D', 'E', 'F'
};

inline Atlas::Uint32 rotate(Atlas::Uint32 num, Atlas::Uint32 n)
{
  return (num << n) | (num >> (32 - n));
}

std::string Atlas::MD5::operator()(const std::filesystem::path& path)
{
  count_[0] = count_[1] = 0;
  state_[0] = 0x67452301;
  state_[1] = 0xefcdab89;
  state_[2] = 0x98badcfe;
  state_[3] = 0x10325476;

  std::ifstream fs(path, std::ios::binary);
  if (!fs.is_open()) {
    throw std::exception(("Open " + path.string() + " failed!").c_str());
  }
  std::streamsize size;
  Byte buffer[BUFFER_SIZE];
  while (!fs.eof()) {
    fs.read((char*)buffer, BUFFER_SIZE);
    size = fs.gcount();
    if (size > 0) {
      Uint32 i = 0, index = (count_[0] >> 3) & 0x3f;
      if ((count_[0] += size << 3) < (size << 3)) {
        ++count_[1];
      }
      count_[1] += size >> 29;
      Uint32 partLen = 64 - index;
      if (size >= partLen) {
        memcpy(buffer_ + index, buffer, partLen);
        process(buffer_);
        for (i = partLen; i + 63 < size; i += 64) {
          process(buffer + i);
        }
        index = 0;
      }
      memcpy(buffer_ + index, buffer + i, size - i);
    }
  }

  Uint32 index = (count_[0] >> 3) & 0x3f;
  if (index < 56) {
    buffer_[index] = 0x80;
    memset(buffer_ + index + 1, 0, 55 - index);
  } else {
    buffer_[index] = 0x80;
    memset(buffer_ + index + 1, 0, 63 - index);
    process(buffer_);
    memset(buffer_, 0, 56);
  }
  memcpy(buffer_ + 56, count_, 8);
  process(buffer_);

  std::string result;
  result.reserve(32);
  for (Size i = 0; i < 16; ++i) {
    result += HEX[((Byte*)state_)[i] >> 4];
    result += HEX[((Byte*)state_)[i] & 0xf];
  }
  return result;
}

inline void Atlas::MD5::process(const Atlas::Byte block[64])
{
  Uint32 a = state_[0], b = state_[1], c = state_[2], d = state_[3];
  Uint32* x = (Uint32*)block;

  a = rotate(a + (d ^ (b & (c ^ d))) + x[0] + 0xd76aa478, 7) + b;
  d = rotate(d + (c ^ (a & (b ^ c))) + x[1] + 0xe8c7b756, 12) + a;
  c = rotate(c + (b ^ (d & (a ^ b))) + x[2] + 0x242070db, 17) + d;
  b = rotate(b + (a ^ (c & (d ^ a))) + x[3] + 0xc1bdceee, 22) + c;
  a = rotate(a + (d ^ (b & (c ^ d))) + x[4] + 0xf57c0faf, 7) + b;
  d = rotate(d + (c ^ (a & (b ^ c))) + x[5] + 0x4787c62a, 12) + a;
  c = rotate(c + (b ^ (d & (a ^ b))) + x[6] + 0xa8304613, 17) + d;
  b = rotate(b + (a ^ (c & (d ^ a))) + x[7] + 0xfd469501, 22) + c;
  a = rotate(a + (d ^ (b & (c ^ d))) + x[8] + 0x698098d8, 7) + b;
  d = rotate(d + (c ^ (a & (b ^ c))) + x[9] + 0x8b44f7af, 12) + a;
  c = rotate(c + (b ^ (d & (a ^ b))) + x[10] + 0xffff5bb1, 17) + d;
  b = rotate(b + (a ^ (c & (d ^ a))) + x[11] + 0x895cd7be, 22) + c;
  a = rotate(a + (d ^ (b & (c ^ d))) + x[12] + 0x6b901122, 7) + b;
  d = rotate(d + (c ^ (a & (b ^ c))) + x[13] + 0xfd987193, 12) + a;
  c = rotate(c + (b ^ (d & (a ^ b))) + x[14] + 0xa679438e, 17) + d;
  b = rotate(b + (a ^ (c & (d ^ a))) + x[15] + 0x49b40821, 22) + c;

  a = rotate(a + (c ^ (d & (b ^ c))) + x[1] + 0xf61e2562, 5) + b;
  d = rotate(d + (b ^ (c & (a ^ b))) + x[6] + 0xc040b340, 9) + a;
  c = rotate(c + (a ^ (b & (d ^ a))) + x[11] + 0x265e5a51, 14) + d;
  b = rotate(b + (d ^ (a & (c ^ d))) + x[0] + 0xe9b6c7aa, 20) + c;
  a = rotate(a + (c ^ (d & (b ^ c))) + x[5] + 0xd62f105d, 5) + b;
  d = rotate(d + (b ^ (c & (a ^ b))) + x[10] + 0x02441453, 9) + a;
  c = rotate(c + (a ^ (b & (d ^ a))) + x[15] + 0xd8a1e681, 14) + d;
  b = rotate(b + (d ^ (a & (c ^ d))) + x[4] + 0xe7d3fbc8, 20) + c;
  a = rotate(a + (c ^ (d & (b ^ c))) + x[9] + 0x21e1cde6, 5) + b;
  d = rotate(d + (b ^ (c & (a ^ b))) + x[14] + 0xc33707d6, 9) + a;
  c = rotate(c + (a ^ (b & (d ^ a))) + x[3] + 0xf4d50d87, 14) + d;
  b = rotate(b + (d ^ (a & (c ^ d))) + x[8] + 0x455a14ed, 20) + c;
  a = rotate(a + (c ^ (d & (b ^ c))) + x[13] + 0xa9e3e905, 5) + b;
  d = rotate(d + (b ^ (c & (a ^ b))) + x[2] + 0xfcefa3f8, 9) + a;
  c = rotate(c + (a ^ (b & (d ^ a))) + x[7] + 0x676f02d9, 14) + d;
  b = rotate(b + (d ^ (a & (c ^ d))) + x[12] + 0x8d2a4c8a, 20) + c;

  a = rotate(a + (b ^ c ^ d) + x[5] + 0xfffa3942, 4) + b;
  d = rotate(d + (a ^ b ^ c) + x[8] + 0x8771f681, 11) + a;
  c = rotate(c + (d ^ a ^ b) + x[11] + 0x6d9d6122, 16) + d;
  b = rotate(b + (c ^ d ^ a) + x[14] + 0xfde5380c, 23) + c;
  a = rotate(a + (b ^ c ^ d) + x[1] + 0xa4beea44, 4) + b;
  d = rotate(d + (a ^ b ^ c) + x[4] + 0x4bdecfa9, 11) + a;
  c = rotate(c + (d ^ a ^ b) + x[7] + 0xf6bb4b60, 16) + d;
  b = rotate(b + (c ^ d ^ a) + x[10] + 0xbebfbc70, 23) + c;
  a = rotate(a + (b ^ c ^ d) + x[13] + 0x289b7ec6, 4) + b;
  d = rotate(d + (a ^ b ^ c) + x[0] + 0xeaa127fa, 11) + a;
  c = rotate(c + (d ^ a ^ b) + x[3] + 0xd4ef3085, 16) + d;
  b = rotate(b + (c ^ d ^ a) + x[6] + 0x04881d05, 23) + c;
  a = rotate(a + (b ^ c ^ d) + x[9] + 0xd9d4d039, 4) + b;
  d = rotate(d + (a ^ b ^ c) + x[12] + 0xe6db99e5, 11) + a;
  c = rotate(c + (d ^ a ^ b) + x[15] + 0x1fa27cf8, 16) + d;
  b = rotate(b + (c ^ d ^ a) + x[2] + 0xc4ac5665, 23) + c;

  a = rotate(a + (c ^ (b | ~d)) + x[0] + 0xf4292244, 6) + b;
  d = rotate(d + (b ^ (a | ~c)) + x[7] + 0x432aff97, 10) + a;
  c = rotate(c + (a ^ (d | ~b)) + x[14] + 0xab9423a7, 15) + d;
  b = rotate(b + (d ^ (c | ~a)) + x[5] + 0xfc93a039, 21) + c;
  a = rotate(a + (c ^ (b | ~d)) + x[12] + 0x655b59c3, 6) + b;
  d = rotate(d + (b ^ (a | ~c)) + x[3] + 0x8f0ccc92, 10) + a;
  c = rotate(c + (a ^ (d | ~b)) + x[10] + 0xffeff47d, 15) + d;
  b = rotate(b + (d ^ (c | ~a)) + x[1] + 0x85845dd1, 21) + c;
  a = rotate(a + (c ^ (b | ~d)) + x[8] + 0x6fa87e4f, 6) + b;
  d = rotate(d + (b ^ (a | ~c)) + x[15] + 0xfe2ce6e0, 10) + a;
  c = rotate(c + (a ^ (d | ~b)) + x[6] + 0xa3014314, 15) + d;
  b = rotate(b + (d ^ (c | ~a)) + x[13] + 0x4e0811a1, 21) + c;
  a = rotate(a + (c ^ (b | ~d)) + x[4] + 0xf7537e82, 6) + b;
  d = rotate(d + (b ^ (a | ~c)) + x[11] + 0xbd3af235, 10) + a;
  c = rotate(c + (a ^ (d | ~b)) + x[2] + 0x2ad7d2bb, 15) + d;
  b = rotate(b + (d ^ (c | ~a)) + x[9] + 0xeb86d391, 21) + c;

  state_[0] += a;
  state_[1] += b;
  state_[2] += c;
  state_[3] += d;
}
