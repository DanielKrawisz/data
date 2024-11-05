// Copyright (c) 2018-2020 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_VALID
#define DATA_VALID

#include <concepts>

#include <data/types.hpp>
#include <data/concepts.hpp>

namespace data {
    
    // when we have modules, this will be unexported.
    namespace interface {
        template <typename X> concept has_valid_method = requires (const X &x) {
            { x.valid () } -> implicitly_convertible_to<bool>;
        };
    }

    template <typename X> bool inline valid (const X &x) {
        return true;
    }

    template <typename X> bool inline valid (const X *x) {
        return x != nullptr;
    }

    template <typename X> bool inline valid (const ptr<X> x) {
        return x != nullptr;
    }

    template <interface::has_valid_method X> auto inline valid (const X &x) {
        return x.valid ();
    }

    template <interface::has_valid_method X> bool inline valid (const X *x) {
        return x == nullptr ? false : x->valid ();
    }

    template <interface::has_valid_method X> bool inline valid (const ptr<X> x) {
        return x == nullptr ? false : x->valid ();
    }

}

#endif
