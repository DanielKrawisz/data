// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_REVERSE
#define DATA_REVERSE

#include <data/functional/stack.hpp>
#include <data/iterable.hpp>

namespace data::interface {
    template <typename X>
    concept has_reverse_method = requires (const X x) {
        { x.reverse () } -> Same<X>;
    };
    
}

namespace data {
    template <typename X>
    X reverse (const X &x) {
        if constexpr (interface::has_reverse_method<X>) {
            return x.reverse ();
        } else if constexpr (Stack<X>) {
            // we had to rely on an iterated function because we got
            // a segmentation fault using lists that were too big.
            X g = x;
            X r {};
            while (data::size (g) > 0) {
                r = prepend (r, first (g));
                g = rest (g);
            }
            return r;
        } else if constexpr (Iterable<X>) {
            auto z = x;
            std::reverse (z.begin (), z.end ());
            return z;
        } else throw exception {} << "invalid reverse type";
    }

}

#endif


