// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/types.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST(ExceptionTest, TestException) {
        try {
            throw (exception{} << "Hi " << 26 << " love love love");
        } catch (std::exception &ex) {
            EXPECT_EQ(string{ex.what()}, "Hi 26 love love love");
        }
        
    }
    
}

