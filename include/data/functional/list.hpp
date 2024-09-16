// Copyright (c) 2019-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_FUNCTIONAL_LIST
#define DATA_FUNCTIONAL_LIST

#include <data/functional/stack.hpp>
#include <data/functional/queue.hpp>
#include <data/cross.hpp>

namespace data::functional {
    
    template <typename Q, typename elem = std::remove_reference_t<decltype (std::declval<Q> ().first ())>>
    concept list = stack<Q, elem> && queue<Q, elem>;
    
    template <typename Q, typename elem = std::remove_reference_t<decltype (std::declval<Q> ().first ())>>
    concept pendable = stack<Q, elem> || queue<Q, elem>;
    
}

namespace data {
    
    template <functional::pendable list> 
    list take (const list &l, size_t x);

    template <functional::pendable list>
    list inline take (const list &l, size_t from, size_t to) {
        return take (drop (l, from), to - from);
    }
    
    template <functional::pendable list>
    list join (const list &a, const list &b) {
        if constexpr (functional::queue<list>) return functional::join_queue (a, b);
        else return functional::join_stack (a, b);
    }

    template <functional::pendable list>
    list inline remove_index (const list &l, size_t x) {
        return join (take (l, x), drop (l, x + 1));
    }
    
    template <functional::pendable list> requires ordered<element_of<list>>
    list merge (const list &a, const list &b) {
        if constexpr (functional::queue<list>) return functional::merge_queue (a, b);
        else return functional::merge_stack (a, b);
    }

    template <functional::pendable L> requires ordered<element_of<L>>
    L merge_sort (const L &x) {
        size_t z = size (x);
        if (z < 2) return x;

        size_t half = z / 2;
        return merge (merge_sort (take (x, half)), merge_sort (drop (x, half)));
    }
    
    // randomly re-order a list using a random engine.
    template <typename L, std::uniform_random_bit_generator engine>
    L shuffle (const L x, engine &e) {
        if (data::size (x) < 2) return x;
        L q = x;
        L z {};

        do {
            q = rotate_left (q, std::uniform_int_distribution<int> (0, q.size () - 1) (e));
            z <<= q.first ();
            q = q.rest ();

        // std::uniform_int_distribution locks up if you try to generate a number from
        // zero to zero, so you have to make sure that the size of the list is at least two.
        } while (data::size (q) > 1);

        return z << data::first (q);
    }

    // shuffle using the default engine.
    template <typename L>
    L shuffle (const L x) {
        return shuffle (x, get_random_engine ());
    }

    using ordering = cross<size_t>;

    // generate a random order.
    template <typename engine>
    ordering random_ordering (size_t size, engine &e) {
        if (size == 0) return ordering {};
        if (size == 1) return ordering {0};

        ordering o (size, -1);

        size_t x = 0;
        size_t i = -1;

        for (size_t x = 0; x < size; x++) {

            int limit = size  - x;
            int steps = limit == 1 ? 0 : std::uniform_int_distribution<int> (0, limit) (e);

            while (true) {
                i = (i + 1) % size;
                if (o[i] == -1) {
                    if (steps == 0) break;
                    else steps--;
                }
            }

            o[i] = x;
        }

        for (size_t x = 0; x < size; x++)
        return o;
    }

    template <typename L>
    L shuffle (const L ll, const ordering &ord) {
        if (ll.size () != ord.size ()) throw exception {} << "sizes must be the same size ";
        L q = ll;
        L z {};

        ordering o = ord;
        size_t i = 0;
        size_t x = 0;

        while (!data::empty (q)) {
            size_t steps = 0;
            while (o[x] != i) {
                if (o[x] != -1) steps++;
                x = (x + 1) % o.size ();
            }

            o[x] = -1;
            i++;

            q = rotate_left (q, steps);
            z = z << q.first ();
            q = q.rest ();
        }

        return z;
    }

    template <typename L, typename elem>
    L riffle (L l, const elem &e) {
        L result {};
        while (l.size () > 1) {
            result <<= l.first ();
            result <<= e;
            l = l.rest ();
        }
        if (l.size () == 1) result <<= l.first ();
        return result;
    }
}

template <data::functional::pendable L> L inline operator + (const L &a, const L &b) {
    return data::join (a, b);
}

#endif

