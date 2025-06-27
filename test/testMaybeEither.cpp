// Copyright (c) 2024 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/maybe.hpp"
#include "data/either.hpp"
#include "gtest/gtest.h"

namespace data {
    // Note: there seems to be some confusion about whether std::variant 
    // has a default constructor. 
/*
    TEST (MaybeEitherTest, TestEither) {

        either<void> mono {};
        mono.get<void> ();

    }
*/
    TEST (MaybeEitherTest, TestMaybeInt) {

        int x = 10;
        maybe<int> no {};
        maybe<int> yes (x);

        EXPECT_FALSE (bool (no));
        EXPECT_TRUE (bool (yes));

        EXPECT_EQ (*yes, 10);

    }

    TEST (MaybeEitherTest, TestMaybeRef) {

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

