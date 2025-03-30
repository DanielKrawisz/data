// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    
    TEST (ZTest, TestZToHexString) {
        
        string Zh_0 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"0"});
        EXPECT_EQ (Zh_0, std::string {"0x"});
        
        string Zh_ff = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"-1"});
        EXPECT_EQ (Zh_ff, std::string {"0xff"});
        
        string Zh_n2 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"-2"});
        EXPECT_EQ (Zh_n2, std::string {"0xfe"});
        
        string Zh_127 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"127"});
        EXPECT_EQ (Zh_127, std::string {"0x7f"});
        
        string Zh_128 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"128"});
        EXPECT_EQ (Zh_128, std::string {"0x0080"});
        
        string Zh_80 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"-128"});
        EXPECT_EQ (Zh_80, std::string {"0x80"});
        
        string Zh_ff7f = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"-129"});
        EXPECT_EQ (Zh_ff7f, std::string {"0xff7f"});
        
        string Zh_qqqq = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {"0x7f33"} << 128);
        EXPECT_EQ (Zh_qqqq, std::string {"0x7f3300000000000000000000000000000000"});
        
    }
    
    TEST (ZTest, TestStringToZ) {
        
        EXPECT_THROW (Z {""}, exception);
        EXPECT_THROW (Z {"-0x01"}, exception);
        
        EXPECT_NO_THROW (Z {"-1"});
        
        EXPECT_EQ (Z {}, Z {"0"});
        EXPECT_EQ (Z {-1}, Z {"-1"});
        EXPECT_EQ (Z {"-1"}, Z {"0xff"});
        
        EXPECT_EQ (encoding::signed_decimal::write (Z {0}), "0");
        EXPECT_EQ (encoding::signed_decimal::write (Z {1}), "1");
        EXPECT_EQ (encoding::signed_decimal::write (Z {-1}), "-1");
        
        string Zh_0 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {0});
        EXPECT_EQ (Zh_0, "0x");
        
        string Zh_1 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {1});
        EXPECT_EQ (Zh_1, "0x01");
        
        string Zh_n1 = encoding::hexidecimal::write<math::number::complement::ones, hex_case::lower> (Z {-1});
        EXPECT_EQ (Zh_n1, "0xff");
        
        EXPECT_TRUE (Z {"0x80000000000000000000"} < Z {"0x7fffffffffffffffffff"});
        EXPECT_TRUE (Z {"0xff"} < Z {"0x00ff"});
        
    }
    
}
