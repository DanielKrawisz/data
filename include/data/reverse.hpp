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
        { x.reverse () } -> std::same_as<X>;
    };
    
}

namespace data {
    // functional solution
    /* template <functional::stack list>
    list reverse (const list &given, const list &reversed) {
        std::cout << "reversing list of size " << data::size (given) << std::endl;
        if (data::empty (given)) return reversed;
        return data::reverse (rest (given), prepend (reversed, first (given)));
    }*/

    // we had to rely on iterated because we get a segmentation fault if the list is too big for functional.
    template <functional::stack list>
    list reverse (const list &given) {
        list g = given;
        list r {};
        while (data::size (g) > 0) {
            r = prepend (r, first (g));
            g = rest (g);
        }
        return r;
    }

    template <interface::has_reverse_method X> 
    X reverse (const X &x) {
        return x.reverse ();
    }

    template <iterable X> 
    X reverse (const X &x) {
        auto z = x;
        std::reverse (z.begin (), z.end ());
        return z;
    }

}

#endif


