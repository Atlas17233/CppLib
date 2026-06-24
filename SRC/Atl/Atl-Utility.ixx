export module Atl:Utility;

import :Def;
import :Type;

namespace Atl
{
  export
  {
    template <Swappable Type>
    constexpr Void swap(Type& left, Type& right) noexcept {
      Type tmp = move(left);
      left = move(right);
      right = move(tmp);
    }

    template <class _Ty>
    Size _Mismatch_impl(
      const void* const _First1, const void* const _First2, const Size _Count) noexcept {
      Size _Result = 0;

      const auto _First1_ch = static_cast<const char*>(_First1);
      const auto _First2_ch = static_cast<const char*>(_First2);

      if (_Use_avx2()) {
        _Zeroupper_on_exit _Guard; // TRANSITION, DevCom-10331414

        const Size _Count_bytes          = _Count * sizeof(_Ty);
        const Size _Count_bytes_avx_full = _Count_bytes & ~Size{0x1F};

        for (; _Result != _Count_bytes_avx_full; _Result += 0x20) {
          const __m256i _Elem1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(_First1_ch + _Result));
          const __m256i _Elem2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(_First2_ch + _Result));
          const __m256i _Cmp   = _mm256_cmpeq_epi8(_Elem1, _Elem2);
          const auto _Bingo    = ~static_cast<unsigned int>(_mm256_movemask_epi8(_Cmp));
          if (_Bingo != 0) {
            return (_Result + _tzcnt_u32(_Bingo)) / sizeof(_Ty);
          }
        }

        const Size _Count_tail = _Count_bytes & Size{0x1C};

        if (_Count_tail != 0) {
          const __m256i _Tail_mask = _Avx2_tail_mask_32(_Count_tail);
          const __m256i _Elem1 =
            _mm256_maskload_epi32(reinterpret_cast<const int*>(_First1_ch + _Result), _Tail_mask);
          const __m256i _Elem2 =
            _mm256_maskload_epi32(reinterpret_cast<const int*>(_First2_ch + _Result), _Tail_mask);

          const __m256i _Cmp = _mm256_cmpeq_epi8(_Elem1, _Elem2);
          const auto _Bingo  = ~static_cast<unsigned int>(_mm256_movemask_epi8(_Cmp));
          if (_Bingo != 0) {
            return (_Result + _tzcnt_u32(_Bingo)) / sizeof(_Ty);
          }

          _Result += _Count_tail;
        }

        _Result /= sizeof(_Ty);

        if constexpr (sizeof(_Ty) >= 4) {
          return _Result;
        }
      } else if (_Use_sse42()) {
        const Size _Count_bytes_sse = (_Count * sizeof(_Ty)) & ~Size{0xF};

        for (; _Result != _Count_bytes_sse; _Result += 0x10) {
          const __m128i _Elem1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_First1_ch + _Result));
          const __m128i _Elem2 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(_First2_ch + _Result));
          const __m128i _Cmp   = _mm_cmpeq_epi8(_Elem1, _Elem2);
          const auto _Bingo    = static_cast<unsigned int>(_mm_movemask_epi8(_Cmp)) ^ 0xFFFF;
          if (_Bingo != 0) {
            unsigned long _Offset;
            // CodeQL [SM02313] _Offset is always initialized: we just tested `if (_Bingo != 0)`.
            _BitScanForward(&_Offset, _Bingo);
            return (_Result + _Offset) / sizeof(_Ty);
          }
        }

        _Result /= sizeof(_Ty);
      }

      const auto _First1_el = static_cast<const _Ty*>(_First1);
      const auto _First2_el = static_cast<const _Ty*>(_First2);

      for (; _Result != _Count; ++_Result) {
        if (_First1_el[_Result] != _First2_el[_Result]) {
          break;
        }
      }

      return _Result;
    }

    template <Size size>
    Size _Mismatch_vectorized(Void* begin1, Void* begin2, Size count) noexcept
    {
      if constexpr (size % 8 == 0) {
        return __std_mismatch_8(begin1, begin2, count * (size >> 3)) / (size >> 3);
      } else if constexpr (size % 4 == 0) {
        return __std_mismatch_4(begin1, begin2, count * (size >> 2)) / (size >> 2);
      } else if constexpr (size % 2 == 0) {
        return __std_mismatch_2(begin1, begin2, count * (size >> 1)) / (size >> 1);
      } else {
        return __std_mismatch_1(begin1, begin2, count * size) / size;
      }
    }
  }
}