// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/numbers.hpp"

#include <data/tools.hpp>
#include <data/math/power.hpp>
#include <data/math/number/eratosthenes.hpp>

#include "data/math/number/factor.hpp"
#include "data/math/number/totient.hpp"
#include "data/math/number/primitive_root.hpp"
#include "gtest/gtest.h"

namespace data::math {
    number::eratosthenes<N> e {};
    number::primes<N> primes (e);

    auto p2 = primes[0];
    auto p3 = primes[1];
    auto p5 = primes[2];
    auto p7 = primes[3];
    auto p11 = primes[4];
    auto p13 = primes[5];
    auto p17 = primes[6];

    TEST (NumberTheoryTest, TestFactor) {

        EXPECT_EQ ((number::factorize<N> (N {0}, e)), (number::factorization<N> {}));
        EXPECT_EQ ((number::factorize<N> (N {1}, e)), (number::factorization<N> {}));
        EXPECT_EQ ((number::factorize<N> (N {2}, e)), (number::factorization<N> {{p2, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {3}, e)), (number::factorization<N> {{p3, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {4}, e)), (number::factorization<N> {{p2, 2}}));
        EXPECT_EQ ((number::factorize<N> (N {5}, e)), (number::factorization<N> {{p5, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {6}, e)), (number::factorization<N> {{p2, 1}, {p3, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {8}, e)), (number::factorization<N> {{p2, 3}}));
        EXPECT_EQ ((number::factorize<N> (N {9}, e)), (number::factorization<N> {{p3, 2}}));
        EXPECT_EQ ((number::factorize<N> (N {10}, e)), (number::factorization<N> {{p2, 1}, {p5, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {12}, e)), (number::factorization<N> {{p2, 2}, {p3, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {17}, e)), (number::factorization<N> {{p17, 1}}));
        EXPECT_EQ ((number::factorize<N> (N {21}, e)), (number::factorization<N> {{p3, 1}, {p7, 1}}));

    }

    TEST (NumberTheoryTest, TestTotient) {

        EXPECT_EQ (number::totient<N> (N {1}, e), N {1});
        EXPECT_EQ (number::totient<N> (N {2}, e), N {1});
        EXPECT_EQ (number::totient<N> (N {5}, e), N {4});
        EXPECT_EQ (number::totient<N> (N {6}, e), N {2});
        EXPECT_EQ (number::totient<N> (N {10}, e), N {4});
        EXPECT_EQ (number::totient<N> (N {15}, e), N {8});
        EXPECT_EQ (number::totient<N> (N {16}, e), N {8});
        EXPECT_EQ (number::totient<N> (N {25}, e), N {20});
        EXPECT_EQ (number::totient<N> (N {36}, e), N {12});
        EXPECT_EQ (number::totient<N> (N {100}, e), N {40});
        EXPECT_EQ (number::totient<N> (N {101}, e), N {100});
        EXPECT_EQ (number::totient<N> (N {210}, e), N {48});
        EXPECT_EQ (number::totient<N> (N {997}, e), N {996});
        EXPECT_EQ (number::totient<N> (N {1024}, e), N {512});
        EXPECT_EQ (number::totient<N> (N {12345}, e), N {6576});

    }

    TEST (NumberTheoryTest, TestPowerMod) {

        EXPECT_EQ ((data::pow_mod<N> (N {761}, N {2}, N {380})), N {1});
        EXPECT_EQ ((data::pow_mod<N> (N {761}, N {3}, N {380})), N {760});
        EXPECT_EQ ((data::pow_mod<N> (N {761}, N {3}, N {152})), N {1});
        EXPECT_EQ ((data::pow_mod<N> (N {761}, N {5}, N {380})), N {1});
        EXPECT_EQ ((data::pow_mod<N> (N {761}, N {6}, N {380})), N {760});
        EXPECT_EQ ((data::pow_mod<N> (N {761}, N {6}, N {40})), N {761 - 263});

    }

    TEST (NumberTheoryTest, TestPrimitiveRoot) {
        EXPECT_EQ ((*number::primitive_root<N> (N {761}, e)), N {6});
    }

}
