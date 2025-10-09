
// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/tools/store.hpp"
#include "gtest/gtest.h"

namespace data {

    struct fwipz {
        int M = 23;
        bool Q {true};

        bool meep () {
            return Q = false;
        }

        template <typename X>
        X zoob_q (function<X (int )> z) {
            store<X> x {z, M};
            meep ();
            return x.retrieve ();
        }
    };

    TEST (Store, Store) {

        fwipz Twoozy;
        Twoozy.zoob_q<int> ([] (int i) -> int {
            return i;
        });

        Twoozy.zoob_q<void> ([] (int i) {});

    }

}

