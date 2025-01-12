def main():
    with open('avx2.inl.cpp', 'w') as f:
        for comb in range(2**9):
            for bit, key in enumerate(compare):
                compare[key] = int(comb & (1 << bit))

            L = input_order[:]
            make_heap_7(L)

            f.write(shuffle_pattern(L) + '\n')


input_order = ['a', 'b', 'c', 'd', 'e', 'f', 'g']


compare = {
    ('a', 'b'): 0,
    ('a', 'c'): 0,
    ('b', 'c'): 0,

    ('a', 'd'): 0,
    ('a', 'e'): 0,
    ('d', 'e'): 0,

    ('a', 'f'): 0,
    ('a', 'g'): 0,
    ('f', 'g'): 0,
}


def less(x, y):
    return compare[(x, y)]


def make_heap(array, index):
    child1 = 2*index + 1
    child2 = 2*index + 2

    initial = index
    if less(array[index], array[child1]):
        index = child1

    if less(array[index], array[child2]):
        index = child2

    if index != initial:
        array[index], array[initial] = array[initial], array[index]
        return index


def make_heap_7(array):
    # [a][b c][d e f g]
    # assume subarray [b..g] is a proper heap, but the [a] may be invalid

    index = make_heap(array, 0)
    if index is None:
        return

    make_heap(array, index)


def shuffle_pattern(L):
    return "{%d, %d, %d, %d, %d, %d, %d, 0}," % (
        input_order.index(L[0]),
        input_order.index(L[1]),
        input_order.index(L[2]),
        input_order.index(L[3]),
        input_order.index(L[4]),
        input_order.index(L[5]),
        input_order.index(L[6]),
    )


if __name__ == '__main__':
    main()
