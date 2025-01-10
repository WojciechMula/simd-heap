#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>
#include <cstdio>
#include <cassert>

#include "avx2.cpp"

using vec = std::vector<int32_t>;

vec copy_except(vec v, size_t index) {
    v.erase(v.begin() + index);
    return v;
}

void permutations(vec& prefix, vec elems, std::vector<vec>& result) {
    if (elems.size() == 1) {
        vec copy = prefix;
        copy.push_back(elems[0]);

        // [a][b c][d e f g]
        // `a` is a new element, elements b..g makes proper heap
        constexpr size_t b = 1;
        constexpr size_t c = 2;
        constexpr size_t d = 3;
        constexpr size_t e = 4;
        constexpr size_t f = 5;
        constexpr size_t g = 6;

        if (copy[b] < copy[d] || copy[b] < copy[e]) {
            return; // not a heap
        }
        if (copy[c] < copy[f] || copy[c] < copy[g]) {
            return; // not a heap
        }

        const auto head = copy[0];
        copy.push_back(head);
        assert(copy.size() == 8);

        result.push_back(copy);
        return;
    }

    const size_t n = prefix.size();
    prefix.push_back(0);
    for (size_t i=0; i < elems.size(); i++) {
        prefix[n] = elems[i];
        permutations(prefix, copy_except(elems, i), result);
    }

    prefix.pop_back();
}

void dump(const int32_t* v, size_t size) {
    printf("[");
    bool first = true;
    for (size_t i=0; i < size; i++) {
        const int32_t m = v[i];
        if (first) {
            printf("%d", m);
            first = false;
        } else {
            printf(", %d", m);
        }
    }
    printf("]\n");
}

void reference(vec& v, size_t count) {
    size_t off = 0;
    for (size_t i=0; i < count; i++, off += 8) {
        std::pop_heap(&v[off], &v[off + 8]);
    }
}

void avx2(vec& v, size_t count) {
    size_t off = 0;
    for (size_t i=0; i < count; i++, off += 8) {
        make_heap_7(&v[off]);
        break; // XXX
    }
}

int main() {
    if (0) {
    vec v = {1, 4, 7, 2, 3, 6, 5, 1};
    dump(v.data(), v.size());
    std::pop_heap(v.begin(), v.end());
    dump(v.data(), v.size());

    return 0;
    }

    vec prefix;
    vec elems = {1, 2, 3, 4, 5, 6, 7};
    std::vector<vec> all;

    permutations(prefix, elems, all);

    vec input;
    input.reserve(all.size() * elems.size() + 1);

    for (const auto& v: all) {
        for (const auto m: v) {
            input.push_back(m);
        }
    }

    const size_t n = all.size();

    vec want = input;
    reference(want, n);

    vec got = input;
    avx2(got, n);

    for (size_t i=0; i < n; i++) {
        const size_t ofs = i * 8;
        for (size_t j=0; j < 7; j++) {
            if (got[ofs + j] != want[ofs + j]) {
                printf("case %ld\n", i);
                printf("input = "); dump(&all[i][0], 8);
                printf("want  = "); dump(&want[ofs], 7);
                printf("got   = "); dump(&got[ofs], 7);
                return EXIT_FAILURE;
            }
        }
    }
}
