// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/encoding/endian/arithmetic.hpp>
#include "gtest/gtest.h"
#include <iostream>

namespace data {

    TEST(EndianTest, TestEndian) {
        uint32_little xla{33993};
        uint32_little xlb{834030483};
        uint32_little xlc{536903935};
        EXPECT_EQ(xla, uint32_little{uint32_big{xla}});
        EXPECT_EQ(xlb, uint32_little{uint32_big{xlb}});
        EXPECT_EQ(xlc, uint32_little{uint32_big{xlc}});
        EXPECT_NE(xlb, uint32_little{uint32_big{xla}});
        EXPECT_NE(xla, uint32_little{uint32_big{xlb}});
    }

}
