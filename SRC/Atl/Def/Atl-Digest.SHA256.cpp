module Atl:Digest.SHA256;

import :Digest;
import <stdlib.h>;

#define s0(i) (Atl::rotR(w[i],  7) ^ Atl::rotR(w[i], 18) ^ w[i] >>  3)
#define s1(i) (Atl::rotR(w[i], 17) ^ Atl::rotR(w[i], 19) ^ w[i] >> 10)

#define S1(i) (Atl::rotR(h[i], 6) ^ Atl::rotR(h[i], 11) ^ Atl::rotR(h[i], 25))
#define ch(i0, i1, i2) (h[i2] ^ h[i0] & (h[i1] ^ h[i2]))
#define temp1(i0, i1, i2, i3, i4, k, w) h[i0] += h[i4] += S1(i1) + ch(i1, i2, i3) + k + w

#define S0(i) (Atl::rotR(h[i], 2) ^ Atl::rotR(h[i], 13) ^ Atl::rotR(h[i], 22))
#define maj(i0, i1, i2) (h[i0] & h[i1] | h[i2] & (h[i0] | h[i1]))
#define temp2(i0, i1, i2, i3) h[i3] += S0(i0) + maj(i0, i1, i2)

#define round(i0, i1, i2, i3, i4, i5, i6, i7, k, w) temp1(i3, i4, i5, i6, i7, k, w); temp2(i0, i1, i2, i7)

#define convertLittleToBig(i) (w[i] = Atl::convertEndian(chunk[i]))
#define extend(i0, i1, i2, i) (w[i] += s0(i0) + w[i1] + s1(i2))

static constexpr Atl::Void process(Atl::Uint digest[8], const Atl::Uint chunk[16]) noexcept
{
  Atl::Uint h[8], w[16];
  Atl::copy(digest, 8, h);
  round(0, 1, 2, 3, 4, 5, 6, 7, 0x428a2f98, convertLittleToBig( 0));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0x71374491, convertLittleToBig( 1));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0xb5c0fbcf, convertLittleToBig( 2));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0xe9b5dba5, convertLittleToBig( 3));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0x3956c25b, convertLittleToBig( 4));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0x59f111f1, convertLittleToBig( 5));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0x923f82a4, convertLittleToBig( 6));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0xab1c5ed5, convertLittleToBig( 7));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0xd807aa98, convertLittleToBig( 8));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0x12835b01, convertLittleToBig( 9));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0x243185be, convertLittleToBig(10));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0x550c7dc3, convertLittleToBig(11));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0x72be5d74, convertLittleToBig(12));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0x80deb1fe, convertLittleToBig(13));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0x9bdc06a7, convertLittleToBig(14));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0xc19bf174, convertLittleToBig(15));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0xe49b69c1, extend( 1,  9, 14,  0));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0xefbe4786, extend( 2, 10, 15,  1));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0x0fc19dc6, extend( 3, 11,  0,  2));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0x240ca1cc, extend( 4, 12,  1,  3));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0x2de92c6f, extend( 5, 13,  2,  4));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0x4a7484aa, extend( 6, 14,  3,  5));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0x5cb0a9dc, extend( 7, 15,  4,  6));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0x76f988da, extend( 8,  0,  5,  7));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0x983e5152, extend( 9,  1,  6,  8));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0xa831c66d, extend(10,  2,  7,  9));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0xb00327c8, extend(11,  3,  8, 10));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0xbf597fc7, extend(12,  4,  9, 11));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0xc6e00bf3, extend(13,  5, 10, 12));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0xd5a79147, extend(14,  6, 11, 13));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0x06ca6351, extend(15,  7, 12, 14));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0x14292967, extend( 0,  8, 13, 15));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0x27b70a85, extend( 1,  9, 14,  0));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0x2e1b2138, extend( 2, 10, 15,  1));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0x4d2c6dfc, extend( 3, 11,  0,  2));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0x53380d13, extend( 4, 12,  1,  3));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0x650a7354, extend( 5, 13,  2,  4));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0x766a0abb, extend( 6, 14,  3,  5));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0x81c2c92e, extend( 7, 15,  4,  6));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0x92722c85, extend( 8,  0,  5,  7));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0xa2bfe8a1, extend( 9,  1,  6,  8));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0xa81a664b, extend(10,  2,  7,  9));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0xc24b8b70, extend(11,  3,  8, 10));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0xc76c51a3, extend(12,  4,  9, 11));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0xd192e819, extend(13,  5, 10, 12));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0xd6990624, extend(14,  6, 11, 13));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0xf40e3585, extend(15,  7, 12, 14));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0x106aa070, extend( 0,  8, 13, 15));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0x19a4c116, extend( 1,  9, 14,  0));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0x1e376c08, extend( 2, 10, 15,  1));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0x2748774c, extend( 3, 11,  0,  2));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0x34b0bcb5, extend( 4, 12,  1,  3));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0x391c0cb3, extend( 5, 13,  2,  4));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0x4ed8aa4a, extend( 6, 14,  3,  5));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0x5b9cca4f, extend( 7, 15,  4,  6));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0x682e6ff3, extend( 8,  0,  5,  7));
  round(0, 1, 2, 3, 4, 5, 6, 7, 0x748f82ee, extend( 9,  1,  6,  8));
  round(7, 0, 1, 2, 3, 4, 5, 6, 0x78a5636f, extend(10,  2,  7,  9));
  round(6, 7, 0, 1, 2, 3, 4, 5, 0x84c87814, extend(11,  3,  8, 10));
  round(5, 6, 7, 0, 1, 2, 3, 4, 0x8cc70208, extend(12,  4,  9, 11));
  round(4, 5, 6, 7, 0, 1, 2, 3, 0x90befffa, extend(13,  5, 10, 12));
  round(3, 4, 5, 6, 7, 0, 1, 2, 0xa4506ceb, extend(14,  6, 11, 13));
  round(2, 3, 4, 5, 6, 7, 0, 1, 0xbef9a3f7, extend(15,  7, 12, 14));
  round(1, 2, 3, 4, 5, 6, 7, 0, 0xc67178f2, extend( 0,  8, 13, 15));
  for (Atl::Int i{0}; i < 8; ++i)
  {
    digest[i] += h[i];
  }
}

template<>
const Atl::SHA256& Atl::SHA256::operator()(const Byte* data, Size size) noexcept
{
  data_[0] = 0x6a09e667;
  data_[1] = 0xbb67ae85;
  data_[2] = 0x3c6ef372;
  data_[3] = 0xa54ff53a;
  data_[4] = 0x510e527f;
  data_[5] = 0x9b05688c;
  data_[6] = 0x1f83d9ab;
  data_[7] = 0x5be0cd19;
  Byte buffer[64];
  Size counter{size & 0x3f};
  if (data && size) {
    const Byte* i{data};
    for (const Byte* end{data + size - 64}; i <= end; i += 64) {
      [[msvc::forceinline]] process(data_, (Uint*)i);
    }
    copy(i, counter, buffer);
  }
  buffer[counter] = 0x80;
  if (++counter <= 56) {
    fill(buffer + counter, 56 - counter, 0);
  } else {
    fill(buffer + counter, 64 - counter, 0);
    process(data_, (Uint*)buffer);
    fill(buffer, 56, 0);
  }
  ((Size*)buffer)[7] = convertEndian64(size << 3);
  process(data_, (Uint*)buffer);
  for (Int i{0}; i < 8; ++i)
  {
    data_[i] = convertEndian(data_[i]);
  }
  return *this;
}
