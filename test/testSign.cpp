// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/data.hpp"
#include "data/math/number/bytes/N.hpp"
#include "data/math/number/bytes/Z.hpp"
#include "data/math/number/bounded/bounded.hpp"
#include "gtest/gtest.h"

namespace data {
    
    template <size_t size> using integer_little = data::math::number::bounded<true, endian::little, size>;
    
    template <size_t size> using integer_big = data::math::number::bounded<true, endian::big, size>;
    
    template <size_t size> using uint_little = data::math::number::bounded<false, endian::little, size>;
    
    template <size_t size> using uint_big = data::math::number::bounded<false, endian::big, size>;
    
    using Z_bytes_little = data::math::number::Z_bytes<endian::little>;
    using Z_bytes_big = data::math::number::Z_bytes<endian::big>;
    
    using N_bytes_little = data::math::number::N_bytes<endian::little>;
    using N_bytes_big = data::math::number::N_bytes<endian::big>;
    
    typedef integer_big<9> b09;
    typedef integer_big<10> b10;
    typedef integer_big<11> b11;
    typedef integer_big<20> b20;
    typedef integer_little<9> l09;
    typedef integer_little<10> l10;
    typedef integer_little<11> l11;
    typedef integer_little<20> l20;
    
    typedef uint_big<9> ub09;
    typedef uint_big<10> ub10;
    typedef uint_big<11> ub11;
    typedef uint_big<20> ub20;
    typedef uint_little<9> ul09;
    typedef uint_little<10> ul10;
    typedef uint_little<11> ul11;
    typedef uint_little<20> ul20;
    
    TEST(SignTest, TestSign) {
        EXPECT_EQ(sign(int64{0}),                            math::zero);
        EXPECT_EQ(sign(int64{1}),                            math::positive);
        EXPECT_EQ(sign(int64{-1}),                           math::negative);
        EXPECT_EQ(sign(int64{2}),                            math::positive);
        EXPECT_EQ(sign(int64{-2}),                           math::negative);
        EXPECT_EQ(sign(int64{7076852110923542}),             math::positive);
        EXPECT_EQ(sign(int64{-7076852110923542}),            math::negative);
        
        EXPECT_EQ(sign(double{0}),                           math::zero);
        EXPECT_EQ(sign(double{1}),                           math::positive);
        EXPECT_EQ(sign(double{-1}),                          math::negative);
        EXPECT_EQ(sign(double{2}),                           math::positive);
        EXPECT_EQ(sign(double{-2}),                          math::negative);
        
        EXPECT_EQ(sign(float{0}),                            math::zero);
        EXPECT_EQ(sign(float{1}),                            math::positive);
        EXPECT_EQ(sign(float{-1}),                           math::negative);
        EXPECT_EQ(sign(float{2}),                            math::positive);
        EXPECT_EQ(sign(float{-2}),                           math::negative);
        
        EXPECT_EQ(sign(uint64{0}),                           math::zero);
        EXPECT_EQ(sign(uint64{1}),                           math::positive);
        EXPECT_EQ(sign(uint64{2}),                           math::positive);
        EXPECT_EQ(sign(uint64{7076852110923542}),            math::positive);
        
        EXPECT_EQ(sign(Z{0}),                                math::zero);
        EXPECT_EQ(sign(Z{1}),                                math::positive);
        EXPECT_EQ(sign(Z{-1}),                               math::negative);
        EXPECT_EQ(sign(Z{2}),                                math::positive);
        EXPECT_EQ(sign(Z{-2}),                               math::negative);
        EXPECT_EQ(sign(Z{7076852110923542}),                 math::positive);
        EXPECT_EQ(sign(Z{-7076852110923542}),                math::negative);
        
        EXPECT_EQ(sign(N{0}),                                math::zero);
        EXPECT_EQ(sign(N{1}),                                math::positive);
        EXPECT_EQ(sign(N{2}),                                math::positive);
        EXPECT_EQ(sign(N{7076852110923542}),                 math::positive);
        
        EXPECT_EQ(sign(Z_bytes_little{0}),                   math::zero);
        EXPECT_EQ(sign(Z_bytes_little{1}),                   math::positive);
        EXPECT_EQ(sign(Z_bytes_little{-1}),                  math::negative);
        EXPECT_EQ(sign(Z_bytes_little{2}),                   math::positive);
        EXPECT_EQ(sign(Z_bytes_little{-2}),                  math::negative);
        EXPECT_EQ(sign(Z_bytes_little{7076852110923542}),    math::positive);
        EXPECT_EQ(sign(Z_bytes_little{-7076852110923542}),   math::negative);
        
        EXPECT_EQ(sign(N_bytes_little{0}),                   math::zero);
        EXPECT_EQ(sign(N_bytes_little{1}),                   math::positive);
        EXPECT_EQ(sign(N_bytes_little{2}),                   math::positive);
        EXPECT_EQ(sign(N_bytes_little{7076852110923542}),    math::positive);
        
        EXPECT_EQ(sign(Z_bytes_big{0}),                      math::zero);
        EXPECT_EQ(sign(Z_bytes_big{1}),                      math::positive);
        EXPECT_EQ(sign(Z_bytes_big{-1}),                     math::negative);
        EXPECT_EQ(sign(Z_bytes_big{2}),                      math::positive);
        EXPECT_EQ(sign(Z_bytes_big{-2}),                     math::negative);
        EXPECT_EQ(sign(Z_bytes_big{7076852110923542}),       math::positive);
        EXPECT_EQ(sign(Z_bytes_big{-7076852110923542}),      math::negative);
        
        EXPECT_EQ(sign(N_bytes_big{0}),                      math::zero);
        EXPECT_EQ(sign(N_bytes_big{1}),                      math::positive);
        EXPECT_EQ(sign(N_bytes_big{2}),                      math::positive);
        EXPECT_EQ(sign(N_bytes_big{7076852110923542}),       math::positive);
        
        EXPECT_EQ(sign(Q{0}),                                math::zero);
        EXPECT_EQ(sign(Q{1}),                                math::positive);
        EXPECT_EQ(sign(Q{-1}),                               math::negative);
        EXPECT_EQ(sign(Q{2}),                                math::positive);
        EXPECT_EQ(sign(Q{-2}),                               math::negative);
        EXPECT_EQ(sign(Q{2, 3}),                             math::positive);
        EXPECT_EQ(sign(Q{-2, 3}),                            math::negative);
        
        EXPECT_EQ(sign(b09(0)),                              math::zero);
        EXPECT_EQ(sign(b10(0)),                              math::zero);
        EXPECT_EQ(sign(b11(0)),                              math::zero);
        EXPECT_EQ(sign(b20(0)),                              math::zero);
        EXPECT_EQ(sign(l09(0)),                              math::zero);
        EXPECT_EQ(sign(l10(0)),                              math::zero);
        EXPECT_EQ(sign(l11(0)),                              math::zero);
        EXPECT_EQ(sign(l20(0)),                              math::zero);
        
        EXPECT_EQ(sign(ub09(0)),                             math::zero);
        EXPECT_EQ(sign(ub10(0)),                             math::zero);
        EXPECT_EQ(sign(ub11(0)),                             math::zero);
        EXPECT_EQ(sign(ub20(0)),                             math::zero);
        EXPECT_EQ(sign(ul09(0)),                             math::zero);
        EXPECT_EQ(sign(ul10(0)),                             math::zero);
        EXPECT_EQ(sign(ul11(0)),                             math::zero);
        EXPECT_EQ(sign(ul20(0)),                             math::zero);
        
        EXPECT_EQ(sign(b20(1)),                              math::positive);
        EXPECT_EQ(sign(b11(1)),                              math::positive);
        EXPECT_EQ(sign(b10(1)),                              math::positive);
        EXPECT_EQ(sign(b09(1)),                              math::positive);
        EXPECT_EQ(sign(l20(1)),                              math::positive);
        EXPECT_EQ(sign(l11(1)),                              math::positive);
        EXPECT_EQ(sign(l10(1)),                              math::positive);
        EXPECT_EQ(sign(l09(1)),                              math::positive);
        
        EXPECT_EQ(sign(ub09(1)),                             math::positive);
        EXPECT_EQ(sign(ub10(1)),                             math::positive);
        EXPECT_EQ(sign(ub11(1)),                             math::positive);
        EXPECT_EQ(sign(ub20(1)),                             math::positive);
        EXPECT_EQ(sign(ul09(1)),                             math::positive);
        EXPECT_EQ(sign(ul10(1)),                             math::positive);
        EXPECT_EQ(sign(ul11(1)),                             math::positive);
        EXPECT_EQ(sign(ul20(1)),                             math::positive);
        
        EXPECT_EQ(sign(b09(-1)),                             math::negative);
        EXPECT_EQ(sign(b10(-1)),                             math::negative);
        EXPECT_EQ(sign(b11(-1)),                             math::negative);
        EXPECT_EQ(sign(b20(-1)),                             math::negative);
        EXPECT_EQ(sign(l09(-1)),                             math::negative);
        EXPECT_EQ(sign(l10(-1)),                             math::negative);
        EXPECT_EQ(sign(l11(-1)),                             math::negative);
        EXPECT_EQ(sign(l20(-1)),                             math::negative);
        
        EXPECT_EQ(sign(b09(2)),                              math::positive);
        EXPECT_EQ(sign(b10(2)),                              math::positive);
        EXPECT_EQ(sign(b11(2)),                              math::positive);
        EXPECT_EQ(sign(b20(2)),                              math::positive);
        EXPECT_EQ(sign(l09(2)),                              math::positive);
        EXPECT_EQ(sign(l10(2)),                              math::positive);
        EXPECT_EQ(sign(l11(2)),                              math::positive);
        EXPECT_EQ(sign(l20(2)),                              math::positive);
        
        EXPECT_EQ(sign(ub09(2)),                             math::positive);
        EXPECT_EQ(sign(ub10(2)),                             math::positive);
        EXPECT_EQ(sign(ub11(2)),                             math::positive);
        EXPECT_EQ(sign(ub20(2)),                             math::positive);
        EXPECT_EQ(sign(ul09(2)),                             math::positive);
        EXPECT_EQ(sign(ul10(2)),                             math::positive);
        EXPECT_EQ(sign(ul11(2)),                             math::positive);
        EXPECT_EQ(sign(ul20(2)),                             math::positive);
        
        EXPECT_EQ(sign(b09(-2)),                             math::negative);
        EXPECT_EQ(sign(b10(-2)),                             math::negative);
        EXPECT_EQ(sign(b11(-2)),                             math::negative);
        EXPECT_EQ(sign(b20(-2)),                             math::negative);
        EXPECT_EQ(sign(l09(-2)),                             math::negative);
        EXPECT_EQ(sign(l10(-2)),                             math::negative);
        EXPECT_EQ(sign(l11(-2)),                             math::negative);
        EXPECT_EQ(sign(l20(-2)),                             math::negative);
        
        EXPECT_EQ(sign(b09(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b10(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b11(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b20(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b09(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b10(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b11(7076852110923542)),               math::positive);
        EXPECT_EQ(sign(b20(7076852110923542)),               math::positive);
        
        EXPECT_EQ(sign(ub09(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ub10(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ub11(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ub20(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ul09(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ul10(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ul11(7076852110923542)),              math::positive);
        EXPECT_EQ(sign(ul20(7076852110923542)),              math::positive);
        
        EXPECT_EQ(sign(b09(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(b10(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(b11(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(b20(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(l09(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(l10(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(l11(-7076852110923542)),              math::negative);
        EXPECT_EQ(sign(l20(-7076852110923542)),              math::negative);
        
    }
}
