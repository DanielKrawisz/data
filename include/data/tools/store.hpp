// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_STORE
#define DATA_TOOLS_STORE

#include <data/meta.hpp>

namespace data {

    // store a value returned from a function and retrieve it later.
    // You can use this to make a member function that take a lambda
    // and returns the value from the lambda with some preparation and
    // potential cleanup after. The type branches on void.
    template <typename X, typename... args> struct store<X (args...)> {
        meta::contain<X> Value;

        store {function<X (args...)> f, args... a} : Value {std::invoke (f, a...)} {}

        meta::retrieve<X> retrieve () {
            return (meta::retrieve<X>) Value;
        }
    };

    template <typename X, typename... args> struct store<void (args...)> {
        store {function<void (args...)> f, args... a} {
            std::invoke (f, a...);
        }

        void retrieve () {}
    };

}

#endif
