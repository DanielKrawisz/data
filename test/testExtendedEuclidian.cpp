// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/number/extended_euclidian.hpp"
#include "data/math/number/bytes/N.hpp"
#include "data/math/number/bytes/Z.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST(ExtendedEuclinianTest, TestAbs) {
        using namespace data::exported;
        using namespace data::math::number;
        abs<data::uint64, data::int64> abs_i{};
        abs<data::uint<20>, data::integer<20>> abs_b{};
        abs<N, Z> abs_z{};
        abs<N_bytes<data::endian::order::little>, Z_bytes<data::endian::order::little>> abs_zbl{};
        abs<N_bytes<data::endian::order::big>, Z_bytes<data::endian::order::big>> abs_zbb{};
        
        EXPECT_EQ(abs_i(0), 0);
        EXPECT_EQ(abs_b(0), 0);
        EXPECT_EQ(abs_z(0), 0);
        EXPECT_EQ(abs_zbl(0), 0);
        EXPECT_EQ(abs_zbb(0), 0);
        
        EXPECT_EQ(abs_i(1), 1);
        EXPECT_EQ(abs_b(1), 1);
        EXPECT_EQ(abs_z(1), 1);
        EXPECT_EQ(abs_zbl(1), 1);
        EXPECT_EQ(abs_zbb(1), 1);
        
        EXPECT_EQ(abs_i(-1), 1);
        EXPECT_EQ(abs_b(-1), 1);
        EXPECT_EQ(abs_z(-1), 1);
        EXPECT_EQ(abs_zbl(-1), 1);
        EXPECT_EQ(abs_zbb(-1), 1);
        
    }
    
    // TODO gaussian rationals
    TEST(ExtendedEuclinianTest, TestArg) {
        using namespace data::exported;
        using namespace data::math::number;
        abs<data::uint64, data::int64> arg_i{};
        abs<data::uint<20>, data::integer<20>> arg_b{};
        abs<N, Z> arg_z{};
        abs<N_bytes<data::endian::order::little>, Z_bytes<data::endian::order::little>> arg_zbl{};
        abs<N_bytes<data::endian::order::big>, Z_bytes<data::endian::order::big>> arg_zbb{};
        
        EXPECT_THROW(arg_i(0), math::division_by_zero);
        EXPECT_THROW(arg_b(0), math::division_by_zero);
        EXPECT_THROW(arg_z(0), math::division_by_zero);
        EXPECT_THROW(arg_zbl(0), math::division_by_zero);
        EXPECT_THROW(arg_zbb(0), math::division_by_zero);
        
        EXPECT_EQ(arg_i(1), 1);
        EXPECT_EQ(arg_b(1), 1);
        EXPECT_EQ(arg_z(1), 1);
        EXPECT_EQ(arg_zbl(1), 1);
        EXPECT_EQ(arg_zbb(1), 1);
        
        EXPECT_EQ(arg_i(-1), 1);
        EXPECT_EQ(arg_b(-1), 1);
        EXPECT_EQ(arg_z(-1), 1);
        EXPECT_EQ(arg_zbl(-1), 1);
        EXPECT_EQ(arg_zbb(-1), 1);
        
        EXPECT_EQ(arg_i(2), 1);
        EXPECT_EQ(arg_b(2), 1);
        EXPECT_EQ(arg_z(2), 1);
        EXPECT_EQ(arg_zbl(2), 1);
        EXPECT_EQ(arg_zbb(2), 1);
        
        EXPECT_EQ(arg_i(-2), 1);
        EXPECT_EQ(arg_b(-2), 1);
        EXPECT_EQ(arg_z(-2), 1);
        EXPECT_EQ(arg_zbl(-2), 1);
        EXPECT_EQ(arg_zbb(-2), 1);
        
    }
    
    TEST(NTest, TestDivision) {
        using namespace data::exported;
        using namespace data::math::number;
        
        math::division<N> div = N{"0x805AA786A57B3BFC0DFDF2EC86760339F018114A7E30C2D2701CF294DC60829D9B011CD8E391"}.divide(58);
        
        EXPECT_EQ(div.Quotient, N{"281747799128083566676930618873129531941258515171631968206030619132595956076515625422029804"});
        EXPECT_EQ(div.Remainder, N{25});
        
    }
    
    TEST(ExtendedEuclinianTest, TestExtendedEuclidian) {
        using namespace data::exported;
        using namespace data::math::number;
        using extended_i = euclidian::extended<data::uint64, data::int64>;
        using extended_b = euclidian::extended<data::uint<20>, data::integer<20>>;
        using extended_z = euclidian::extended<N, Z>;
        using extended_zbl = euclidian::extended<N_bytes<data::endian::order::little>, Z_bytes<data::endian::order::little>>;
        using extended_zbb = euclidian::extended<N_bytes<data::endian::order::big>, Z_bytes<data::endian::order::big>>;
        
        EXPECT_EQ(extended_i::algorithm(0, 0).GCD, 0);
        EXPECT_EQ(extended_b::algorithm(0, 0).GCD, 0);
        EXPECT_EQ(extended_z::algorithm(0, 0).GCD, 0);
        EXPECT_EQ(extended_zbl::algorithm(0, 0).GCD, 0);
        EXPECT_EQ(extended_zbb::algorithm(0, 0).GCD, 0);
        
        EXPECT_EQ(extended_i::algorithm(1, 1).GCD, 1);
        EXPECT_EQ(extended_b::algorithm(1, 1).GCD, 1);
        EXPECT_EQ(extended_z::algorithm(1, 1).GCD, 1);
        EXPECT_EQ(extended_zbl::algorithm(1, 1).GCD, 1);
        EXPECT_EQ(extended_zbb::algorithm(1, 1).GCD, 1);
        
        EXPECT_EQ(extended_i::algorithm(2, 4).GCD, 4);
        EXPECT_EQ(extended_b::algorithm(2, 4).GCD, 4);
        EXPECT_EQ(extended_z::algorithm(2, 4).GCD, 4);
        EXPECT_EQ(extended_zbl::algorithm(2, 4).GCD, 4);
        EXPECT_EQ(extended_zbb::algorithm(2, 4).GCD, 4);
        
        EXPECT_EQ(extended_i::algorithm(1145, 916).GCD, 229);
        EXPECT_EQ(extended_b::algorithm(1145, 916).GCD, 229);
        EXPECT_EQ(extended_z::algorithm(1145, 916).GCD, 229);
        EXPECT_EQ(extended_zbl::algorithm(1145, 916).GCD, 229);
        EXPECT_EQ(extended_zbb::algorithm(1145, 916).GCD, 229);
        
    }
    
}
