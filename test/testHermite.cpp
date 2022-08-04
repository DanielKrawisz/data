// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include <data/math/Hermite.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
#include <stdexcept>

namespace data::math {
    
    template <typename Z, typename N> void test_hermite() {
        auto x = polynomial<Z, N, 'x'>::variable();
        auto unit = polynomial<Z, N, 'x'>::unit();
        
        h0g = Hermite<Z, N>(0u);
        h0e = unit;
        EXPECT_EQ(h0g, h0e);
        
        h1g = Hermite<Z, N>(1u);
        h1e = x * 2;
        EXPECT_EQ(h1g, h1e);
        
        h2g = Hermite<Z, N>(2u);
        h2e = (x ^ 2) * 4 - 2;
        EXPECT_EQ(h2g, h2e);
        
        h3g = Hermite<Z, N>(3u);
        h3e = (x ^ 3) * 8 - x * 12;
        EXPECT_EQ(h3g, h3e);
        
        h4g = Hermite<Z, N>(4u);
        h4e = (x ^ 4) * 16 - (x ^ 2) * 48 + 12;
        EXPECT_EQ(h4g, h4e);
    }
    
    TEST(HermiteTest, TestHermite) {
        
        test_bit_negate<Z_bytes_big, N_bytes_big>();
        test_bit_negate<Z_bytes_little, N_bytes_little>();
        test_bit_negate<integer_big<9>, uint_big<9>>();
        test_bit_negate<integer_little<9>, uint_big<9>>();
        
    }
    
}

