// Copyright (c) 2019-2020 Daniel Krawisz
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
    list join (const list &a, const list &b) {
        if constexpr (functional::queue<list>) return functional::join_queue (a, b);
        else return functional::join_stack (a, b);
    }
    
    template <functional::pendable list> requires ordered<element_of<list>>
    list merge (const list &a, const list &b) {
        if constexpr (functional::queue<list>) return functional::merge_queue (a, b);
        else return functional::merge_stack (a, b);
    }
    
    template <functional::list L>
    inline L rotate_right (const L x) {
        size_t s = size (x);
        if (s == 0 || s == 1) return x; 
        
        return prepend (rest (x), first (x));
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
        L q = x;
        L z {};
        while (!data::empty (q)) {
            q = rotate_left (q, std::uniform_int_distribution<int> (0, q.size () - 1) (e));
            z = z << q.first ();
            q = q.rest ();
        }
        return z;
    }

    // shuffle using the default engine.
    template <typename L>
    L shuffle (const L x) {
        return shuffle (x, get_random_engine ());
    }

    // generate a random order.
    template <typename engine>
    cross<size_t> random_ordering (size_t size, engine &e) {
        cross<size_t> ordering (size);
        for (size_t &z : ordering) z = -1;

        size_t x = 0;
        size_t i = 0;

        for (size_t x = 0; x < size; x++) {

            int steps = std::uniform_int_distribution<int> (0, size - 1) (e);

            while (steps > 0) {
                if (ordering[i] != -1) steps--;
                i = (i + 1) % size;
            }

            ordering[i] = x;
        }

        return ordering;
    }

    template <typename L>
    L shuffle (const L ll, const cross<size_t> &ordering) {
        if (ll.size () != ordering.size ()) throw exception {} << "sizes must be the same";

        L q = ll;
        L z{};

        cross<size_t> o = ordering;
        size_t x = 0;

        while (!data::empty (q)) {
            size_t steps = 0;
            while (o[x] != x) {
                if (o[x] != -1) steps++;
                x = (x + 1) % o.size ();
            }
            o[x] = -1;
            x++;

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

