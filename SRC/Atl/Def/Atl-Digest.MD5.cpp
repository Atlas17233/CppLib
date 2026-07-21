module Atl:Digest.MD5;

import :Digest;

import "../Macros";

#define F1(i0, i1, i2) (h[i2] ^ h[i0] & (h[i1] ^ h[i2]))
#define F2(i0, i1, i2) (h[i1] ^ h[i2] & (h[i0] ^ h[i1]))
#define F3(i0, i1, i2) (h[i0] ^ h[i1] ^ h[i2])
#define F4(i0, i1, i2) (h[i1] ^ (h[i0] | ~h[i2]))

#define round(F, i0, i1, i2, i3, k, i, s) h[i0] = h[i1] + Atl::rotL(h[i0] + F(i1, i2, i3) + k + m[i], s)

forceinline static constexpr Atl::Void process(Atl::UInt32 digest[4], const Atl::UInt32 m[16]) noexcept
{
  Atl::UInt32 h[4];
  Atl::copy(digest, 4, h);
  round(F1, 0, 1, 2, 3, 0xd76aa478,  0,  7);
  round(F1, 3, 0, 1, 2, 0xe8c7b756,  1, 12);
  round(F1, 2, 3, 0, 1, 0x242070db,  2, 17);
  round(F1, 1, 2, 3, 0, 0xc1bdceee,  3, 22);
  round(F1, 0, 1, 2, 3, 0xf57c0faf,  4,  7);
  round(F1, 3, 0, 1, 2, 0x4787c62a,  5, 12);
  round(F1, 2, 3, 0, 1, 0xa8304613,  6, 17);
  round(F1, 1, 2, 3, 0, 0xfd469501,  7, 22);
  round(F1, 0, 1, 2, 3, 0x698098d8,  8,  7);
  round(F1, 3, 0, 1, 2, 0x8b44f7af,  9, 12);
  round(F1, 2, 3, 0, 1, 0xffff5bb1, 10, 17);
  round(F1, 1, 2, 3, 0, 0x895cd7be, 11, 22);
  round(F1, 0, 1, 2, 3, 0x6b901122, 12,  7);
  round(F1, 3, 0, 1, 2, 0xfd987193, 13, 12);
  round(F1, 2, 3, 0, 1, 0xa679438e, 14, 17);
  round(F1, 1, 2, 3, 0, 0x49b40821, 15, 22);
  round(F2, 0, 1, 2, 3, 0xf61e2562,  1,  5);
  round(F2, 3, 0, 1, 2, 0xc040b340,  6,  9);
  round(F2, 2, 3, 0, 1, 0x265e5a51, 11, 14);
  round(F2, 1, 2, 3, 0, 0xe9b6c7aa,  0, 20);
  round(F2, 0, 1, 2, 3, 0xd62f105d,  5,  5);
  round(F2, 3, 0, 1, 2, 0x02441453, 10,  9);
  round(F2, 2, 3, 0, 1, 0xd8a1e681, 15, 14);
  round(F2, 1, 2, 3, 0, 0xe7d3fbc8,  4, 20);
  round(F2, 0, 1, 2, 3, 0x21e1cde6,  9,  5);
  round(F2, 3, 0, 1, 2, 0xc33707d6, 14,  9);
  round(F2, 2, 3, 0, 1, 0xf4d50d87,  3, 14);
  round(F2, 1, 2, 3, 0, 0x455a14ed,  8, 20);
  round(F2, 0, 1, 2, 3, 0xa9e3e905, 13,  5);
  round(F2, 3, 0, 1, 2, 0xfcefa3f8,  2,  9);
  round(F2, 2, 3, 0, 1, 0x676f02d9,  7, 14);
  round(F2, 1, 2, 3, 0, 0x8d2a4c8a, 12, 20);
  round(F3, 0, 1, 2, 3, 0xfffa3942,  5,  4);
  round(F3, 3, 0, 1, 2, 0x8771f681,  8, 11);
  round(F3, 2, 3, 0, 1, 0x6d9d6122, 11, 16);
  round(F3, 1, 2, 3, 0, 0xfde5380c, 14, 23);
  round(F3, 0, 1, 2, 3, 0xa4beea44,  1,  4);
  round(F3, 3, 0, 1, 2, 0x4bdecfa9,  4, 11);
  round(F3, 2, 3, 0, 1, 0xf6bb4b60,  7, 16);
  round(F3, 1, 2, 3, 0, 0xbebfbc70, 10, 23);
  round(F3, 0, 1, 2, 3, 0x289b7ec6, 13,  4);
  round(F3, 3, 0, 1, 2, 0xeaa127fa,  0, 11);
  round(F3, 2, 3, 0, 1, 0xd4ef3085,  3, 16);
  round(F3, 1, 2, 3, 0, 0x04881d05,  6, 23);
  round(F3, 0, 1, 2, 3, 0xd9d4d039,  9,  4);
  round(F3, 3, 0, 1, 2, 0xe6db99e5, 12, 11);
  round(F3, 2, 3, 0, 1, 0x1fa27cf8, 15, 16);
  round(F3, 1, 2, 3, 0, 0xc4ac5665,  2, 23);
  round(F4, 0, 1, 2, 3, 0xf4292244,  0,  6);
  round(F4, 3, 0, 1, 2, 0x432aff97,  7, 10);
  round(F4, 2, 3, 0, 1, 0xab9423a7, 14, 15);
  round(F4, 1, 2, 3, 0, 0xfc93a039,  5, 21);
  round(F4, 0, 1, 2, 3, 0x655b59c3, 12,  6);
  round(F4, 3, 0, 1, 2, 0x8f0ccc92,  3, 10);
  round(F4, 2, 3, 0, 1, 0xffeff47d, 10, 15);
  round(F4, 1, 2, 3, 0, 0x85845dd1,  1, 21);
  round(F4, 0, 1, 2, 3, 0x6fa87e4f,  8,  6);
  round(F4, 3, 0, 1, 2, 0xfe2ce6e0, 15, 10);
  round(F4, 2, 3, 0, 1, 0xa3014314,  6, 15);
  round(F4, 1, 2, 3, 0, 0x4e0811a1, 13, 21);
  round(F4, 0, 1, 2, 3, 0xf7537e82,  4,  6);
  round(F4, 3, 0, 1, 2, 0xbd3af235, 11, 10);
  round(F4, 2, 3, 0, 1, 0x2ad7d2bb,  2, 15);
  round(F4, 1, 2, 3, 0, 0xeb86d391,  9, 21);
  for (Atl::Int i{}; i < 4; ++i) {
    digest[i] += h[i];
  }
}

template <>
constexpr Atl::UInt32 Atl::MD5::initValue[4]{0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

template <>
const Atl::MD5& Atl::MD5::operator()(const UInt8* data, Size size) noexcept
{
  init();
  UInt8 buffer[64];
  Size counter{size & 0x3f};
  if (data && size) [[likely]] {
    const UInt8* i{data};
    for (const UInt8* end{data + size - 64}; i <= end; i += 64) {
      process(data_, (UInt32*)i);
    }
    copy(i, counter, buffer);
  }
  buffer[counter] = 0x80;
  if (++counter <= 56) {
    fill(buffer + counter, 56 - counter, 0);
  } else {
    fill(buffer + counter, 64 - counter, 0);
    process(data_, (UInt32*)buffer);
    fill(buffer, 56, 0);
  }
  ((Size*)buffer)[7] = (size << 3);
  process(data_, (UInt32*)buffer);
  return *this;
}
