// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/number/eratosthenes.hpp"
#include "data/data.hpp"
#include "gtest/gtest.h"
#include <iostream>

TEST(EratosthenesTest, DISABLED_First100Primes) {
    using namespace data;

    const data::uint32 max_primes = 100;
    
    list<N> primes = for_each([](const prime p)->N{
            return p.Prime;
        }, Eratosthenes{N{max_primes}}.Primes);
    
    EXPECT_TRUE(primes.size() == max_primes);
    
    list<N> expected = list<N>::make(
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 
        67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 
        139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 
        223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 
        293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 
        383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 
        463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541);
    
    EXPECT_EQ(primes, expected);
}

