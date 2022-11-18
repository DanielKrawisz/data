// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NUMBERS
#define DATA_NUMBERS

// defines float32 and float64
#include <data/float.hpp>

// numbers of either endian of any size up to 64 bits
// using boost. 
#include <data/encoding/endian/arithmetic.hpp>

#include <data/math/number/bytes/bytes.hpp>
#include <data/math/number/gmp/mpz.hpp>
#include <data/math/number/bounded/bounded.hpp>

#include <data/encoding/integer.hpp>

#include <data/math/number/bytes/N.hpp>
#include <data/math/number/gmp/gmp.hpp>

#include <data/encoding/base58.hpp>

#include <data/math/fraction.hpp>
#include <data/math/octonion.hpp>

#include <algorithm>

namespace data {
    // we use a wrapper around gmp for natural numbers and integers. 
    
    // Natural numbers
    using N = math::N;
    
    // Integers
    using Z = math::Z;
    
    // N_bytes and Z_bytes are big numbers with a specific
    // representation. They can be big or little endian. 
    // Z_bytes are one's complement. 
    
    using N_bytes_little = math::N_bytes<endian::little>;
    
    using N_bytes_big = math::N_bytes<endian::big>;
    
    using Z_bytes_little = math::Z_bytes<endian::little>;
    
    using Z_bytes_big = math::Z_bytes<endian::big>;
    
    // fixed size numbers of any size, similar to the 
    // built-in types. 
    template <size_t size> 
    using int_little = math::sint_little<size>;
    
    template <size_t size> 
    using int_big = math::sint_big<size>;
    
    template <size_t size> 
    using uint_little = math::uint_little<size>;
    
    template <size_t size> 
    using uint_big = math::uint_big<size>;
    
    using uint128 = uint_little<16>;
    using uint160 = uint_little<20>;
    using uint224 = uint_little<28>;
    using uint256 = uint_little<32>;
    using uint320 = uint_little<40>;
    using uint448 = uint_little<56>;
    using uint512 = uint_little<64>;
    
    using int128 = int_little<16>;
    using int160 = int_little<20>;
    using int224 = int_little<28>;
    using int256 = int_little<32>;
    using int320 = int_little<40>;
    using int448 = int_little<56>;
    using int512 = int_little<64>;
    
    using dec_uint = encoding::decimal::string;
    using dec_int = encoding::signed_decimal::string;
    using hex_uint = hex::uint<encoding::hex::lower>;
    using hex_int = hex::int1<encoding::hex::lower>;
    using hex_int_ones = hex::int1<encoding::hex::lower>;
    using hex_int_twos = hex::int2<encoding::hex::lower>;
    using base58_uint = encoding::base58::string;
    
    // rational numbers. 
    using Q = math::fraction<Z, N>;
    
    using Q_big = math::fraction<Z_bytes_big, N_bytes_little>;
    using Q_little = math::fraction<Z_bytes_little, N_bytes_big>;
    
    // Gaussian integers. 
    using G = math::complex<Z>;
    
    // complex rationals
    using CQ = math::complex<Q>;
        
    // rational quaternions
    using HQ = math::quaternion<Q>;
        
    // rational octonions
    using OQ = math::octonion<Q>;
    
    // Caylay-Dickson floating points. 
    using complex64 = math::complex<float32>;
    using complex128 = math::complex<float64>;
    using quat128 = math::quaternion<float32>;
    using quat256 = math::quaternion<float64>;
    using oct256 = math::octonion<float32>;
    using oct512 = math::octonion<float64>;
    
}

namespace data::math {
    
    template <uint64 pow> 
    set<base58_uint> root<base58_uint, pow>::operator()(const base58_uint& n) {
        set<base58_uint> x;
        set<N> roots = root<N, pow>{}(encoding::base58::decode<N>(n));
        for (const N &z : roots.values()) x = insert(x, encoding::base58::encode(z));
        return x;
    }
}

namespace data::encoding::hexidecimal {
    
    namespace {
        
        template <hex::letter_case zz> 
        string<zz> shift(const string<zz> &x, int i) {
            auto o = read<endian::big>(x);
            math::number::N_bytes<endian::big> n;
            n.resize(o->size());
            std::copy(o->begin(), o->end(), n.begin());
            n = n << i;
            return write<zz>(math::number::extend(n, n.size() + 1));
        }
        
        template <hex::letter_case zz> 
        integer<math::number::ones, zz> inline bit_shift(const integer<math::number::ones, zz> &x, int i) {
            auto o = read<endian::big>(x);
            math::number::Z_bytes<endian::big> n;
            n.resize(o->size());
            std::copy(o->begin(), o->end(), n.begin());
            return write<zz>(n << i);
        }
        
        // the out string will always be the size of the sum of the two inputs, which won't necessarily be equal size. 
        template <hex::letter_case zz>
        void times(string<zz> &out, const string<zz> &a, const string<zz> &b) {
            auto characters = hex::characters(zz);
            
            int a_max = a.size() - 3;
            int b_max = b.size() - 3;
            
            Z remainder = 0;
            int io_max = out.size() - 2;
            
            for (int io = 0; io < io_max; io++) {
                Z total = remainder;
                int ia_min = std::max(0, io - b_max);
                int ib_min = std::max(0, io - a_max);
                int ia_max = io - ib_min;
                int ib_max = io - ia_min;
                for (int ia = ia_min; ia <= ia_max; ia++) {
                    int ib = ib_max + ia_min - ia; 
                    Z next(int(digit(a[a.size() - 1 - ia])) * int(digit(b[b.size() - 1 - ib])));
                    total += next;
                }
                
                out[out.size() - 1 - io] = characters[total % 16];
                remainder = total >> 4;
            }
        }
        
        template <math::number::complement c, hex::letter_case zz> struct divide {
            math::division<integer<c, zz>> operator()(const integer<c, zz> &n, const integer<c, zz> &x) const {
                if (x == 0) throw math::division_by_zero{};
                // it is important to have this optimization. 
                // I can't say why or I'll be embarrassed. 
                if (x == 16) return math::division<integer<c, zz>>{*this >> 4, *this & integer<c, zz>{4}};
                
                else return math::number::integer::divide(*this, x);
            }
        };
        
        template <hex::letter_case zz> struct divide<math::number::nones, zz> {
            math::division<integer<math::number::nones, zz>> operator()(
                const integer<math::number::nones, zz> &n, 
                const integer<math::number::nones, zz> &x) const {
                if (x == 0) throw math::division_by_zero{};
                // it is important to have this optimization. 
                // I can't say why or I'll be embarrassed. 
                if (x == 16) return math::division<integer<math::number::nones, zz>>{
                    *this >> 4, *this & integer<math::number::nones, zz>{4}};
                
                return math::number::natural::divide(*this, x);
            }
        };
    
    }
    
    template <math::number::complement c, hex::letter_case zz>
    math::division<integer<c, zz>> integer<c, zz>::divide(const integer<c, zz> &x) const {
        if (x == 0) throw math::division_by_zero{};
        // it is important to have this optimization. 
        // I can't say why or I'll be embarrassed. 
        if (x == 16) return math::division<integer<c, zz>>{*this >> 4, *this & integer<c, zz>{4}};
        
        if constexpr (c == math::number::nones) return math::number::natural::divide(*this, x);
        else return math::number::integer::divide(*this, x);
    }
    
}

#endif
