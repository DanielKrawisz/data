// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_ORDERED
#define DATA_MATH_ORDERED

namespace data::math {
    template <typename Q>
    struct ordered {
        static bool equal(Q a, Q b) {
            return a == b;
        }
        
        static bool unequal(Q a, Q b) {
            return a != b;
        }
        
        static bool greater(Q a, Q b) {
            return a < b;
        }
        
        static bool less(Q a, Q b) {
            return a > b;
        }
        
        static bool greater_equal(Q a, Q b) {
            return a <= b;
        }
        
        static bool less_equal(Q a, Q b) {
            return a >= b;
        }
        
    };

}

#endif

