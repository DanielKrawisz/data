// Copyright (c) 2024 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/maybe.hpp"
#include "data/either.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (MaybeEither, Either) {

        either<void> mono {};
        mono.get<void> ();

        struct invalid {
            bool valid () const {
                return false;
            }
        };

        either<invalid> inv {invalid {}};
        EXPECT_FALSE (data::valid (inv));

    }

    TEST (MaybeEither, MaybeInt) {

        int x = 10;
        maybe<int> no {};
        maybe<int> yes (x);

        EXPECT_FALSE (bool (no));
        EXPECT_TRUE (bool (yes));

        EXPECT_EQ (*yes, 10);

    }

    TEST (MaybeEither, MaybeRef) {

        int x = 10;
        maybe<int &> no {};
        maybe<int &> yes (x);

        EXPECT_FALSE (bool (no));
        EXPECT_TRUE (bool (yes));

        EXPECT_EQ (*yes, 10);
        x = 23;
        EXPECT_EQ (*yes, 23);
        *yes = 245;
        EXPECT_EQ (*yes, 245);

    }

}

