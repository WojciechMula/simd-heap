#pragma once

#include <iterator>

template <typename ForwardIterator, typename Compare = std::less<>>
bool is_heap_fwd(ForwardIterator start, ForwardIterator end, Compare cmp) {
    if (start == end)
        return true;

    auto parent  = start;
    auto current = std::next(start);
    while (current != end) {
        if (cmp(*parent, *current))
            return false;

        current = std::next(current);
        if (current == end)
            break;

        if (cmp(*parent, *current))
            return false;

        parent = std::next(parent);
        current = std::next(current);
    }

    return true;
}

