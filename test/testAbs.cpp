// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/number/abs.hpp"
#include "data/math/number/bytes/N.hpp"
#include "data/math/number/bytes/Z.hpp"
#include "data/math/number/bounded/bounded.hpp"
#include "gtest/gtest.h"

namespace data {
    
    template <size_t size> 
    using integer_little = data::math::number::bounded<true, endian::little, size>;
    
    template <size_t size> 
    using integer_big = data::math::number::bounded<true, endian::big, size>;
    
    template <size_t size> 
    using uint_little = data::math::number::bounded<false, endian::little, size>;
    
    template <size_t size> 
    using uint_big = data::math::number::bounded<false, endian::big, size>;
    
    using Z_bytes_little = data::math::number::Z_bytes<endian::little>;
    using Z_bytes_big = data::math::number::Z_bytes<endian::big>;
    
    using N_bytes_little = data::math::number::N_bytes<endian::little>;
    using N_bytes_big = data::math::number::N_bytes<endian::big>;
    
    TEST(SignTest, TestDecrement) {
        EXPECT_EQ(    integer_big<9>{0}--,     integer_big<9>{"0xffffffffffffffffff"});
        EXPECT_EQ(   integer_big<10>{0}--,    integer_big<10>{"0xffffffffffffffffffff"});
        EXPECT_EQ(   integer_big<11>{0}--,    integer_big<11>{"0xffffffffffffffffffffff"});
        EXPECT_EQ(   integer_big<20>{0}--,    integer_big<20>{"0xffffffffffffffffffffffffffffffffffffffff"});
        EXPECT_EQ( integer_little<9>{0}--,  integer_little<9>{"0xffffffffffffffffff"});
        EXPECT_EQ(integer_little<10>{0}--, integer_little<10>{"0xffffffffffffffffffff"});
        EXPECT_EQ(integer_little<11>{0}--, integer_little<11>{"0xffffffffffffffffffffff"});
        EXPECT_EQ(integer_little<20>{0}--, integer_little<20>{"0xffffffffffffffffffffffffffffffffffffffff"});
        
        EXPECT_LT(integer_big<9>{"0x800000000000000000"}, 
                  integer_big<9>{"0x7fffffffffffffffff"});
        EXPECT_LT(integer<10>{"0x80000000000000000000"}, 
                  integer<10>{"0x7fffffffffffffffffff"});
        EXPECT_LT(integer<11>{"0x8000000000000000000000"}, 
                  integer<11>{"0x7fffffffffffffffffffff"});
        EXPECT_LT(integer<20>{"0x8000000000000000000000000000000000000000"},
                  integer<20>{"0x7fffffffffffffffffffffffffffffffffffffff"});
        EXPECT_LT(integer_little<9>{"0x800000000000000000"}, 
                  integer_little<9>{"0x7fffffffffffffffff"});
        EXPECT_LT(integer_little<10>{"0x80000000000000000000"}, 
                  integer_little<10>{"0x7fffffffffffffffffff"});
        EXPECT_LT(integer_little<11>{"0x8000000000000000000000"}, 
                  integer_little<11>{"0x7fffffffffffffffffffff"});
        EXPECT_LT(integer_little<20>{"0x8000000000000000000000000000000000000000"}, 
                  integer_little<20>{"0x7fffffffffffffffffffffffffffffffffffffff"});
    }

    TEST(SignTest, TestAbs) {
        
        abs<uint64, int64> abs_i{};
        abs<uint_big<20>, integer_big<20>> abs_b20{};
        abs<uint_big<9>, integer_big<9>> abs_b09{};
        abs<uint_big<10>, integer_big<10>> abs_b10{};
        abs<uint_big<11>, integer_big<11>> abs_b11{};
        abs<math::number::bounded<20, endian::order::little, false>, 
            math::number::bounded<20, endian::order::little, true>> abs_l20{};
        abs<math::number::bounded<9, endian::order::little, false>, 
            math::number::bounded<9, endian::order::little, true>> abs_l09{};
        abs<math::number::bounded<10, endian::order::little, false>, 
            math::number::bounded<10, endian::order::little, true>> abs_l10{};
        abs<math::number::bounded<11, endian::order::little, false>, 
            math::number::bounded<11, endian::order::little, true>> abs_l11{};
        abs<N, Z> abs_z{};
        abs<N_bytes<endian::order::little>, Z_bytes<endian::order::little>> abs_zbl{};
        abs<N_bytes<endian::order::big>, Z_bytes<endian::order::big>> abs_zbb{};
        
        EXPECT_EQ(abs_i(0), 0);
        EXPECT_EQ(abs_z(0), 0);
        EXPECT_EQ(abs_zbl(0), 0);
        EXPECT_EQ(abs_zbb(0), 0);
        EXPECT_EQ(abs_b09(0), 0);
        EXPECT_EQ(abs_b10(0), 0);
        EXPECT_EQ(abs_b11(0), 0);
        EXPECT_EQ(abs_b20(0), 0);
        EXPECT_EQ(abs_l09(0), 0);
        EXPECT_EQ(abs_l10(0), 0);
        EXPECT_EQ(abs_l11(0), 0);
        EXPECT_EQ(abs_l20(0), 0);
        
        EXPECT_EQ(abs_i(1), 1);
        EXPECT_EQ(abs_z(1), 1);
        EXPECT_EQ(abs_zbl(1), 1);
        EXPECT_EQ(abs_zbb(1), 1);
        EXPECT_EQ(abs_b09(1), 1);
        EXPECT_EQ(abs_b10(1), 1);
        EXPECT_EQ(abs_b11(1), 1);
        EXPECT_EQ(abs_b20(1), 1);
        EXPECT_EQ(abs_l09(1), 1);
        EXPECT_EQ(abs_l10(1), 1);
        EXPECT_EQ(abs_l11(1), 1);
        EXPECT_EQ(abs_l20(1), 1);
        
        EXPECT_EQ(abs_i(-1), 1);
        EXPECT_EQ(abs_z(-1), 1);
        EXPECT_EQ(abs_zbl(-1), 1);
        EXPECT_EQ(abs_zbb(-1), 1);
        EXPECT_EQ(abs_b09(-1), 1);
        EXPECT_EQ(abs_b10(-1), 1);
        EXPECT_EQ(abs_b11(-1), 1);
        EXPECT_EQ(abs_b20(-1), 1);
        EXPECT_EQ(abs_l09(-1), 1);
        EXPECT_EQ(abs_l10(-1), 1);
        EXPECT_EQ(abs_l11(-1), 1);
        EXPECT_EQ(abs_l20(-1), 1);
        
        EXPECT_EQ(abs_i(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_z(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_zbl(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_zbb(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b09(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b10(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b11(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b20(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l09(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l10(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l11(7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l20(7076852110923542), 7076852110923542);
        
        EXPECT_EQ(abs_i(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_z(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_zbl(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_zbb(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b09(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b10(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b11(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_b20(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l09(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l10(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l11(-7076852110923542), 7076852110923542);
        EXPECT_EQ(abs_l20(-7076852110923542), 7076852110923542);
        
    }
    
    TEST(SignTest, TestArg) {
        
        arg<int64> arg_i{};
        arg<integer<9>> arg_b09{};
        arg<integer<10>> arg_b10{};
        arg<integer<11>> arg_b11{};
        arg<integer<20>> arg_b20{};
        arg<math::number::bounded<9, endian::little, true>> arg_l09{};
        arg<math::number::bounded<10, endian::little, true>> arg_l10{};
        arg<math::number::bounded<11, endian::little, true>> arg_l11{};
        arg<math::number::bounded<20, endian::little, true>> arg_l20{};
        arg<Z> arg_z{};
        arg<Z_bytes<endian::order::little>> arg_zbl{};
        arg<Z_bytes<endian::order::big>> arg_zbb{};
        
        EXPECT_THROW(arg_i(0),   math::division_by_zero);
        EXPECT_THROW(arg_z(0),   math::division_by_zero);
        EXPECT_THROW(arg_zbl(0), math::division_by_zero);
        EXPECT_THROW(arg_zbb(0), math::division_by_zero);
        EXPECT_THROW(arg_b09(0), math::division_by_zero);
        EXPECT_THROW(arg_b10(0), math::division_by_zero);
        EXPECT_THROW(arg_b11(0), math::division_by_zero);
        EXPECT_THROW(arg_b20(0), math::division_by_zero);
        EXPECT_THROW(arg_l09(0), math::division_by_zero);
        EXPECT_THROW(arg_l10(0), math::division_by_zero);
        EXPECT_THROW(arg_l11(0), math::division_by_zero);
        EXPECT_THROW(arg_l20(0), math::division_by_zero);
        
        EXPECT_EQ(arg_i(1),   1);
        EXPECT_EQ(arg_z(1),   1);
        EXPECT_EQ(arg_zbl(1), 1);
        EXPECT_EQ(arg_zbb(1), 1);
        EXPECT_EQ(arg_b20(1), 1);
        EXPECT_EQ(arg_b11(1), 1);
        EXPECT_EQ(arg_b10(1), 1);
        EXPECT_EQ(arg_b09(1), 1);
        EXPECT_EQ(arg_l20(1), 1);
        EXPECT_EQ(arg_l11(1), 1);
        EXPECT_EQ(arg_l10(1), 1);
        EXPECT_EQ(arg_l09(1), 1);
        
        EXPECT_EQ(arg_i(-1),   -1);
        EXPECT_EQ(arg_z(-1),   -1);
        EXPECT_EQ(arg_zbl(-1), -1);
        EXPECT_EQ(arg_zbb(-1), -1);
        EXPECT_EQ(arg_b20(-1), -1);
        EXPECT_EQ(arg_b11(-1), -1);
        EXPECT_EQ(arg_b10(-1), -1);
        EXPECT_EQ(arg_b09(-1), -1);
        EXPECT_EQ(arg_l20(-1), -1);
        EXPECT_EQ(arg_l11(-1), -1);
        EXPECT_EQ(arg_l10(-1), -1);
        EXPECT_EQ(arg_l09(-1), -1);
        
        EXPECT_EQ(arg_i(2),   1);
        EXPECT_EQ(arg_z(2),   1);
        EXPECT_EQ(arg_zbl(2), 1);
        EXPECT_EQ(arg_zbb(2), 1);
        EXPECT_EQ(arg_b20(2), 1);
        EXPECT_EQ(arg_b11(2), 1);
        EXPECT_EQ(arg_b10(2), 1);
        EXPECT_EQ(arg_b09(2), 1);
        EXPECT_EQ(arg_l20(2), 1);
        EXPECT_EQ(arg_l11(2), 1);
        EXPECT_EQ(arg_l10(2), 1);
        EXPECT_EQ(arg_l09(2), 1);
        
        EXPECT_EQ(arg_i(-2),   -1);
        EXPECT_EQ(arg_z(-2),   -1);
        EXPECT_EQ(arg_zbl(-2), -1);
        EXPECT_EQ(arg_zbb(-2), -1);
        EXPECT_EQ(arg_b20(-2), -1);
        EXPECT_EQ(arg_b11(-2), -1);
        EXPECT_EQ(arg_b10(-2), -1);
        EXPECT_EQ(arg_b09(-2), -1);
        EXPECT_EQ(arg_l20(-2), -1);
        EXPECT_EQ(arg_l11(-2), -1);
        EXPECT_EQ(arg_l10(-2), -1);
        EXPECT_EQ(arg_l09(-2), -1);
        
        EXPECT_EQ(arg_i(7076852110923542),   1);
        EXPECT_EQ(arg_z(7076852110923542),   1);
        EXPECT_EQ(arg_zbl(7076852110923542), 1);
        EXPECT_EQ(arg_zbb(7076852110923542), 1);
        EXPECT_EQ(arg_b20(7076852110923542), 1);
        EXPECT_EQ(arg_b11(7076852110923542), 1);
        EXPECT_EQ(arg_b10(7076852110923542), 1);
        EXPECT_EQ(arg_b09(7076852110923542), 1);
        EXPECT_EQ(arg_l20(7076852110923542), 1);
        EXPECT_EQ(arg_l11(7076852110923542), 1);
        EXPECT_EQ(arg_l10(7076852110923542), 1);
        EXPECT_EQ(arg_l09(7076852110923542), 1);
        
        EXPECT_EQ(arg_i(-7076852110923542),   -1);
        EXPECT_EQ(arg_z(-7076852110923542),   -1);
        EXPECT_EQ(arg_zbl(-7076852110923542), -1);
        EXPECT_EQ(arg_zbb(-7076852110923542), -1);
        EXPECT_EQ(arg_b20(-7076852110923542), -1);
        EXPECT_EQ(arg_b11(-7076852110923542), -1);
        EXPECT_EQ(arg_b10(-7076852110923542), -1);
        EXPECT_EQ(arg_b09(-7076852110923542), -1);
        EXPECT_EQ(arg_l20(-7076852110923542), -1);
        EXPECT_EQ(arg_l11(-7076852110923542), -1);
        EXPECT_EQ(arg_l10(-7076852110923542), -1);
        EXPECT_EQ(arg_l09(-7076852110923542), -1);
        
    }
    
    TEST(SignTest, TestMinus) {
        
        EXPECT_EQ(-Z{0}, Z{0});
        EXPECT_EQ(-Z_bytes<endian::big>{0}, Z_bytes<endian::big>{0});
        EXPECT_EQ(-Z_bytes<endian::little>{0}, Z_bytes<endian::little>{0});
        EXPECT_EQ(-integer<9>{0}, integer<9>{0});
        EXPECT_EQ(-integer<10>{0}, integer<10>{0});
        EXPECT_EQ(-integer<11>{0}, integer<11>{0});
        EXPECT_EQ(-integer<20>{0}, integer<20>{0});
        EXPECT_EQ(-integer_little<9>{0}, integer_little<9>{0});
        EXPECT_EQ(-integer_little<10>{0}, integer_little<10>{0});
        EXPECT_EQ(-integer_little<11>{0}, integer_little<11>{0});
        EXPECT_EQ(-integer_little<20>{0}, integer_little<20>{0});
        
        EXPECT_EQ(-Z{1}, Z{-1});
        EXPECT_EQ(-Z_bytes<endian::big>{1}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(-Z_bytes<endian::little>{1}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(-integer<9>{1}, integer<9>{-1});
        EXPECT_EQ(-integer<10>{1}, integer<10>{-1});
        EXPECT_EQ(-integer<11>{1}, integer<11>{-1});
        EXPECT_EQ(-integer<20>{1}, integer<20>{-1});
        EXPECT_EQ(-integer_little<9>{1}, integer_little<9>{-1});
        EXPECT_EQ(-integer_little<10>{1}, integer_little<10>{-1});
        EXPECT_EQ(-integer_little<11>{1}, integer_little<11>{-1});
        EXPECT_EQ(-integer_little<20>{1}, integer_little<20>{-1});
        
        EXPECT_EQ(-Z{-1}, Z{1});
        EXPECT_EQ(-Z_bytes<endian::big>{-1}, Z_bytes<endian::big>{1});
        EXPECT_EQ(-Z_bytes<endian::little>{-1}, Z_bytes<endian::little>{1});
        EXPECT_EQ(-integer<9>{-1}, integer<9>{1});
        EXPECT_EQ(-integer<10>{-1}, integer<10>{1});
        EXPECT_EQ(-integer<11>{-1}, integer<11>{1});
        EXPECT_EQ(-integer<20>{-1}, integer<20>{1});
        EXPECT_EQ(-integer_little<9>{-1}, integer_little<9>{1});
        EXPECT_EQ(-integer_little<10>{-1}, integer_little<10>{1});
        EXPECT_EQ(-integer_little<11>{-1}, integer_little<11>{1});
        EXPECT_EQ(-integer_little<20>{-1}, integer_little<20>{1});
        
        EXPECT_EQ(-Z{2}, Z{-2});
        EXPECT_EQ(-Z_bytes<endian::big>{2}, Z_bytes<endian::big>{-2});
        EXPECT_EQ(-Z_bytes<endian::little>{2}, Z_bytes<endian::little>{-2});
        EXPECT_EQ(-integer<9>{2}, integer<9>{-2});
        EXPECT_EQ(-integer<10>{2}, integer<10>{-2});
        EXPECT_EQ(-integer<11>{2}, integer<11>{-2});
        EXPECT_EQ(-integer<20>{2}, integer<20>{-2});
        EXPECT_EQ(-integer_little<9>{2}, integer_little<9>{-2});
        EXPECT_EQ(-integer_little<10>{2}, integer_little<10>{-2});
        EXPECT_EQ(-integer_little<11>{2}, integer_little<11>{-2});
        EXPECT_EQ(-integer_little<20>{2}, integer_little<20>{-2});
        
        EXPECT_EQ(-Z{-2}, Z{2});
        EXPECT_EQ(-Z_bytes<endian::big>{-2}, Z_bytes<endian::big>{2});
        EXPECT_EQ(-Z_bytes<endian::little>{-2}, Z_bytes<endian::little>{2});
        EXPECT_EQ(-integer<9>{-2}, integer<9>{2});
        EXPECT_EQ(-integer<10>{-2}, integer<10>{2});
        EXPECT_EQ(-integer<11>{-2}, integer<11>{2});
        EXPECT_EQ(-integer<20>{-2}, integer<20>{2});
        EXPECT_EQ(-integer_little<9>{-2}, integer_little<9>{2});
        EXPECT_EQ(-integer_little<10>{-2}, integer_little<10>{2});
        EXPECT_EQ(-integer_little<11>{-2}, integer_little<11>{2});
        EXPECT_EQ(-integer_little<20>{-2}, integer_little<20>{2});
        
        EXPECT_EQ(-Z{1}, Z{-1});
        EXPECT_EQ(-Z_bytes<endian::big>{1}, Z_bytes<endian::big>{-1});
        EXPECT_EQ(-Z_bytes<endian::little>{1}, Z_bytes<endian::little>{-1});
        EXPECT_EQ(-integer<9>{1}, integer<9>{-1});
        EXPECT_EQ(-integer<10>{1}, integer<10>{-1});
        EXPECT_EQ(-integer<11>{1}, integer<11>{-1});
        EXPECT_EQ(-integer<20>{1}, integer<20>{-1});
        EXPECT_EQ(-integer_little<9>{1}, integer_little<9>{-1});
        EXPECT_EQ(-integer_little<10>{1}, integer_little<10>{-1});
        EXPECT_EQ(-integer_little<11>{1}, integer_little<11>{-1});
        EXPECT_EQ(-integer_little<20>{1}, integer_little<20>{-1});
        
        EXPECT_EQ(-Z{-7076852110923542}, Z{7076852110923542});
        EXPECT_EQ(-Z_bytes<endian::big>{-7076852110923542}, Z_bytes<endian::big>{7076852110923542});
        EXPECT_EQ(-Z_bytes<endian::little>{-7076852110923542}, Z_bytes<endian::little>{7076852110923542});
        EXPECT_EQ(-integer<9>{-7076852110923542}, integer<9>{7076852110923542});
        EXPECT_EQ(-integer<10>{-7076852110923542}, integer<10>{7076852110923542});
        EXPECT_EQ(-integer<11>{-7076852110923542}, integer<11>{7076852110923542});
        EXPECT_EQ(-integer<20>{-7076852110923542}, integer<20>{7076852110923542});
        EXPECT_EQ(-integer_little<9>{-7076852110923542}, integer_little<9>{7076852110923542});
        EXPECT_EQ(-integer_little<10>{-7076852110923542}, integer_little<10>{7076852110923542});
        EXPECT_EQ(-integer_little<11>{-7076852110923542}, integer_little<11>{7076852110923542});
        EXPECT_EQ(-integer_little<20>{-7076852110923542}, integer_little<20>{7076852110923542});
        
    }
}

