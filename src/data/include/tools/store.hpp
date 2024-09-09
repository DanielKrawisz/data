// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_STORE
#define DATA_TOOLS_STORE

#include <data/meta.hpp>

namespace data {

    // store is for storing a value to retrieve later, or not, depending
    // on the return value of the function. It is for operations that take
    // a lambda but require some preparation and cleanup but may need to
    // return the value returned by the lambda, but the lambda may also
    // return void. If you want to pass in references, wrap them in other
    // types.
    template <typename X> struct store {
        meta::contain<X> Value;

        template <typename ...args>
        store (function<X (args ...)> f, args ...a) : Value {std::invoke (f, a...)} {}

        meta::retrieve<X> retrieve () {
            return (meta::retrieve<X>) Value;
        }
    };

    template <> struct store<void> {

        template <typename ...args>
        store (function<void (args ...)> f, args ...a) {
            std::invoke (f, a...);
        }

        void retrieve () {}
    };

}

#endif
