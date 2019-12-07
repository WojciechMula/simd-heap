#include <cstdlib>
#include <immintrin.h>

__m512i sort_values[16];

void push_heap_avx512(uint32_t* start, uint32_t* end) {

    ssize_t index = (end - start) - 1;
    if ((index >= 32767) && (index <= 65535)) { // we're handling insertions only at level #15

        // 1. Build indices values: [i, (i-1)/2, (i-3)/4, (i-7)/8, ..., (i-15)/16, 0].
        //    The first index points to the new value (new child), the last index point
        //    to the root (index=0), all values between are k-th parents: (i - 2^k - 1)/2^k.
        __m512i indices;
        {
            // new_values = [i, i, i, ..., i]
            const __m512i t0 = _mm512_set1_epi32(index);

            // t1 = [i, (i-1), (i-3), ..., (i-15)]
            const __m512i c1 = _mm512_setr_epi32(0, 1, 3, 7, 15, 31, 63, 127, 255, 511,
                                                 1023, 2047, 4095, 16383, 32767, 32767);
            const __m512i t1 = _mm512_sub_epi32(t0, c1);

            // indices = [i, (i-1)/2, (i-3)/4, (i-7)/8, (i-15)/16, ...]
            const __m512i c2 = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
            indices = _mm512_srlv_epi32(t1, c2);
            puts("");
            dump_epu32(t1);
            dump_epu32(indices);
        }

        // 2. Load values from path between the new element and the root.
        const __m512i values = _mm512_i32gather_epi32(indices, start, sizeof(uint32_t));

        // 3. Check if the heap propery is violated.
        const __m512i new_value = _mm512_set1_epi32(start[index]);
        const __mmask16 any_parent_less = _mm512_cmplt_epu32_mask(values, new_value);
        if (any_parent_less == 0)
            return;

        // 4. Elements on the path be should sorted, we need to locate where to insert the new value.
        const int new_index = __builtin_ctz(any_parent_less);
        const __m512i sorted = _mm512_permutexvar_epi32(sort_values[new_index], values);
        _mm512_i32scatter_epi32(start, indices, sorted, sizeof(uint32_t));
    } else {
        std::push_heap(start, end);
        printf("%ld\n", index);
        abort();
    }
}

void push_heap_avx512_setup() {
    sort_values[0]  = _mm512_setr_epi32(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[1]  = _mm512_setr_epi32(1, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[2]  = _mm512_setr_epi32(1, 2, 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[3]  = _mm512_setr_epi32(1, 2, 3, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[4]  = _mm512_setr_epi32(1, 2, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[5]  = _mm512_setr_epi32(1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[6]  = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 0, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[7]  = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 0, 8, 9, 10, 11, 12, 13, 14, 15);
    sort_values[8]  = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 0, 9, 10, 11, 12, 13, 14, 15);
    sort_values[9]  = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 10, 11, 12, 13, 14, 15);
    sort_values[10] = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 11, 12, 13, 14, 15);
    sort_values[11] = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15);
    sort_values[12] = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 13, 14, 15);
    sort_values[13] = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 14, 15);
    sort_values[14] = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 15);
    sort_values[15] = _mm512_setr_epi32(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0);
}
