#include <vector>
#include <algorithm>
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

ssize_t pop_heap_step(int32_t* v, ssize_t index) {
    const ssize_t child1 = 2*index + 1;
    const ssize_t child2 = 2*index + 2;

    const ssize_t initial = index;
    if (v[index] < v[child1]) {
        index = child1;
    }

    if (v[index] < v[child2]) {
        index = child2;
    }

    if (index != initial) {
        const int32_t tmp = v[index];
        v[index] = v[initial];
        v[initial] = tmp;
        return index;
    }

    return -1;
}

void make_heap_7(int32_t* arr) {
    const ssize_t index = pop_heap_step(arr, 0);
    if (index >= 0) {
        pop_heap_step(arr, index);
    }
}

void scalar(vec& v, size_t count) {
    size_t off = 0;
    for (size_t i=0; i < count; i++, off += 8) {
        make_heap_7(&v[off]);
    }
}

void avx2(vec& v, size_t count) {
    size_t off = 0;
    for (size_t i=0; i < count; i++, off += 8) {
        avx2_make_heap_7(&v[off]);
    }
}

bool compare(const char* name, const std::vector<vec>& all, const vec& want, const vec& got) {
    const size_t n = all.size();

    for (size_t i=0; i < n; i++) {
        const size_t ofs = i * 8;
        for (size_t j=0; j < 7; j++) {
            if (got[ofs + j] != want[ofs + j]) {
                printf("%s: case %ld\n", name, i);
                printf("input = "); dump(&all[i][0], 8);
                printf("want  = "); dump(&want[ofs], 7);
                printf("got   = "); dump(&got[ofs], 7);
                return false;
            }
        }
    }

    return true;
}

int main() {
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

    {
        vec got = input;
        avx2(got, n);
        if (not compare("avx2", all, want, got)) {
            return EXIT_FAILURE;
        }
    }

    {
        vec got = input;
        scalar(got, n);
        if (not compare("scalar", all, want, got)) {
            return EXIT_FAILURE;
        }
    }

    puts("All OK");
    return EXIT_SUCCESS;
}
