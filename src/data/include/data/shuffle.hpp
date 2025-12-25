// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SHUFFLE
#define DATA_SHUFFLE

#include <data/functional/list.hpp>
#include <data/random.hpp>

namespace data {

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
        return shuffle (x, random::get ());
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

}

#endif
