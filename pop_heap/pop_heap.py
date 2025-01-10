compare = {
    ('a', 'c'): 0,
    ('b', 'c'): 0,

    ('a', 'd'): 0,
    ('a', 'e'): 0,
    ('d', 'e'): 0,

    ('a', 'f'): 0,
    ('a', 'g'): 0,
    ('f', 'g'): 0,

    ('a', 'b'): 0,
}

def less(x, y):
    return compare[(x, y)]


def make_heap_7(array):
    # [a][b c][d e f g]
    # assume subarray [b..g] is a proper heap, but the [a] may be invalid
    a = 0
    b = 1
    c = 2
    d = 3
    e = 4
    f = 5
    g = 6

    node    = a
    child1  = b
    child2  = c
    initial = node
    if less(array[node], array[child1]):
        node = child1

    if less(array[node], array[child2]):
        node = child2

    if node == initial:
        return

    if node == b:
        child1 = d
        child2 = e
    elif node == c:
        child1 = f
        child2 = g
    else:
        assert 0

    array[initial], array[node] = array[node], array[initial]

    initial = node
    if less(array[node], array[child1]):
        node = child1

    if less(array[node], array[child2]):
        node = child2

    if initial != node:
        array[initial], array[node] = array[node], array[initial]


input_order = ['a', 'b', 'c', 'd', 'e', 'f', 'g']

def print_shuffle_array(L):
    print("{%d, %d, %d, %d, %d, %d, %d, 0}," % (
        input_order.index(L[0]),
        input_order.index(L[1]),
        input_order.index(L[2]),
        input_order.index(L[3]),
        input_order.index(L[4]),
        input_order.index(L[5]),
        input_order.index(L[6]),
    ))


for comb in range(2**9):
    for bit, key in enumerate(compare):
        compare[key] = int(comb & (1 << bit))

    L = ['a', 'b', 'c', 'd', 'e', 'f', 'g']
    make_heap_7(L)
    print_shuffle_array(L)
