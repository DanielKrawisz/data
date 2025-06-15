// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"
#include "data/math/number/extended_euclidian.hpp"
#include "gtest/gtest.h"

namespace data::math {
    
    using Nb = number::N_bytes<endian::big, byte>;
    using Nl = number::N_bytes<endian::little, byte>;
    
    using Zb1 = Z_bytes_big;
    using Zl1 = Z_bytes_little;
    using Zb2 = Z_bytes_BC_big;
    using Zl2 = Z_bytes_BC_little;

    template <typename N, typename Z>
    void extended_euclidian_test () {
        using X = math::number::euclidian::extended<N, Z>;
        EXPECT_THROW (X::algorithm (N {0}, N {0}).GCD, math::division_by_zero);

        EXPECT_EQ (X::algorithm (N {1}, N {1}).GCD, 1);

        EXPECT_EQ (X::algorithm (N {2}, N {4}).GCD, 2);

        EXPECT_EQ (X::algorithm (N {1145}, N {916}).GCD, 229);
    }
    
    template <typename Z, typename N>
    struct test_extended_euclidian {
        test_extended_euclidian () {
            extended_euclidian_test<N, Z> ();
            extended_euclidian_test<Z, Z> ();
        }
    };
    
    TEST (ExtendedEuclidianTest, TestExtendedEuclidian) {

        test_extended_euclidian<uint64, uint64> {};
        test_extended_euclidian<int64, int64> {};

        test_extended_euclidian<uint64_little, uint64_little> {};
        test_extended_euclidian<int64_little, int64_little> {};

        test_extended_euclidian<uint64_big, uint64_big> {};
        test_extended_euclidian<int64_big, int64_big> {};

        test_extended_euclidian<int80, int80> {};
        test_extended_euclidian<int80_little, int80_little> {};
        test_extended_euclidian<int80_big, int80_big> {};

        test_extended_euclidian<int128, int128> {};
        test_extended_euclidian<int128_little, int128_little> {};
        test_extended_euclidian<int128_big, int128_big> {};

        test_extended_euclidian<uint80, uint80> {};
        test_extended_euclidian<uint80_little, uint80_little> {};
        test_extended_euclidian<uint80_big, uint80_big> {};

        test_extended_euclidian<uint128, uint128> {};
        test_extended_euclidian<uint128_little, uint128_little> {};
        test_extended_euclidian<uint128_big, uint128_big> {};

        test_extended_euclidian<Z, N> {};

        test_extended_euclidian<Zl1, Nl> {};
        test_extended_euclidian<Zb1, Nb> {};

        test_extended_euclidian<Z_bytes_BC_little, Z_bytes_BC_little> {};
        test_extended_euclidian<Z_bytes_BC_big, Z_bytes_BC_big> {};

        test_extended_euclidian<dec_int, dec_uint> {};
        test_extended_euclidian<hex_int, hex_uint> {};

        test_extended_euclidian<hex_int_BC, hex_int_BC> {};
        
    }
    
}
