#include <immintrin.h>

uint32_t heap_7_shuffle[512][8] = {
#include "avx2.inl.cpp"
};

void avx2_make_heap_7(int32_t* arr) {
    // 1. load 7 initial elements of heaps
    //    [a][b c][d e f g]
    const __m256i head = _mm256_loadu_si256((const __m256i*)arr);

    constexpr size_t a = 0;
    constexpr size_t b = 1;
    constexpr size_t c = 2;
    constexpr size_t d = 3;
    constexpr size_t e = 4;
    constexpr size_t f = 5;
    constexpr size_t g = 6;

    // 2. compare the following pairs:
    //    1. a < b [scalar comparison]
    //    2. a < c
    //    3. b < c
    //
    //    4. a < d
    //    5. a < e
    //    6. d < e
    //
    //    7. a < f
    //    8. a < g
    //    9. f < g
    
    const __m256i abaadaaf = _mm256_permutevar8x32_epi32(head, _mm256_setr_epi32(a, b, a, a, d, a, a, f));
    const __m256i ccdeefgg = _mm256_permutevar8x32_epi32(head, _mm256_setr_epi32(c, c, d, e, e, f, g, g));
    const unsigned ab_lt   = arr[a] < arr[b];
    const __m256i lt_mask  = _mm256_cmpgt_epi32(ccdeefgg, abaadaaf);

    const unsigned id = (_mm256_movemask_ps(_mm256_castsi256_ps(lt_mask)) << 1) | ab_lt;

    const __m256i pattern  = _mm256_load_si256((const __m256i*)&heap_7_shuffle[id]);
    const __m256i shuffled = _mm256_permutevar8x32_epi32(head, pattern);

    _mm256_storeu_si256((__m256i*)arr, shuffled);
}
