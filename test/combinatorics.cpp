// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/math/combinatorics.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (Combinatorics, Factorial) {

        EXPECT_THROW (math::factorial<int> (-1), math::negative_factorial);
        EXPECT_EQ (math::factorial<int> (0), 1);
        EXPECT_EQ (math::factorial<int> (1), 1);
        EXPECT_EQ (math::factorial<int> (2), 2);
        EXPECT_EQ (math::factorial<int> (3), 6);
        EXPECT_EQ (math::factorial<int> (4), 24);
        EXPECT_EQ (math::factorial<int> (5), 120);

    }

    TEST (Combinatorics, Binomial) {

        EXPECT_EQ (math::binomial<int> (0, 0), 1);
        EXPECT_EQ (math::binomial<int> (1, 0), 1);
        EXPECT_EQ (math::binomial<int> (2, 0), 1);
        EXPECT_EQ (math::binomial<int> (3, 0), 1);

        EXPECT_EQ (math::binomial<int> (1, 1), 1);
        EXPECT_EQ (math::binomial<int> (2, 2), 1);
        EXPECT_EQ (math::binomial<int> (3, 3), 1);
        EXPECT_EQ (math::binomial<int> (4, 4), 1);

        EXPECT_EQ (math::binomial<int> (2, 1), 2);
        EXPECT_EQ (math::binomial<int> (3, 1), 3);
        EXPECT_EQ (math::binomial<int> (4, 1), 4);

        EXPECT_EQ (math::binomial<int> (3, 2), 3);
        EXPECT_EQ (math::binomial<int> (4, 3), 4);


    }

    TEST (Combinatorics, Multichoose) {

        EXPECT_THROW (math::multichoose<int> (0, 0), math::negative_factorial);
        EXPECT_THROW (math::multichoose<int> (0, 1), math::negative_factorial);
        EXPECT_THROW (math::multichoose<int> (0, 2), math::negative_factorial);

        EXPECT_EQ (math::multichoose<int> (1, 0), 1);
        EXPECT_EQ (math::multichoose<int> (2, 0), 1);
        EXPECT_EQ (math::multichoose<int> (3, 0), 1);

        EXPECT_EQ (math::multichoose<int> (1, 1), 1);
        EXPECT_EQ (math::multichoose<int> (1, 2), 1);
        EXPECT_EQ (math::multichoose<int> (1, 3), 1);

        EXPECT_EQ (math::multichoose<int> (2, 1), 2);
        EXPECT_EQ (math::multichoose<int> (3, 1), 3);
        EXPECT_EQ (math::multichoose<int> (4, 1), 4);

    }

    TEST (Combinatorics, Polytopic) {

        // zeroth polytopic number for any dimension is zero.
        EXPECT_EQ (math::polytopic_number<int> (0, 0), 0);
        EXPECT_EQ (math::polytopic_number<int> (1, 0), 0);
        EXPECT_EQ (math::polytopic_number<int> (2, 0), 0);
        EXPECT_EQ (math::polytopic_number<int> (3, 0), 0);

        // first polytopic number is always one.
        EXPECT_EQ (math::polytopic_number<int> (0, 1), 1);
        EXPECT_EQ (math::polytopic_number<int> (1, 1), 1);
        EXPECT_EQ (math::polytopic_number<int> (2, 1), 1);
        EXPECT_EQ (math::polytopic_number<int> (3, 1), 1);

        // every subsequent polytopic number in dimension zero is one.
        EXPECT_EQ (math::polytopic_number<int> (0, 2), 1);
        EXPECT_EQ (math::polytopic_number<int> (0, 3), 1);
        EXPECT_EQ (math::polytopic_number<int> (0, 4), 1);

        // in dimension 1, the polytopic numbers are linear.
        EXPECT_EQ (math::polytopic_number<int> (1, 2), 2);
        EXPECT_EQ (math::polytopic_number<int> (1, 3), 3);
        EXPECT_EQ (math::polytopic_number<int> (1, 4), 4);
        EXPECT_EQ (math::polytopic_number<int> (1, 5), 5);

        // in dimension two, they are the triangular numbers.
        EXPECT_EQ (math::polytopic_number<int> (2, 2), 3);
        EXPECT_EQ (math::polytopic_number<int> (2, 3), 6);
        EXPECT_EQ (math::polytopic_number<int> (2, 4), 10);

        EXPECT_EQ (math::polytopic_number<int> (3, 2), 4);
        EXPECT_EQ (math::polytopic_number<int> (3, 3), 10);

    }

    void permutations_test_case (stack<int> x) {
        permutations<int> p {x};
        list<list<int>> pp = static_cast<list<list<int>>> (p);

        EXPECT_EQ (pp.size (), p.size<size_t> ());
    }

    TEST (Combinatorics, Permutations) {

        permutations_test_case (stack<int> {});
        permutations_test_case (stack<int> {1});
        permutations_test_case (stack<int> {1, 2});
        permutations_test_case (stack<int> {1, 2, 3});
        permutations_test_case (stack<int> {1, 2, 3, 4});

    }

    void sublists_test_case (stack<int> x, size_t size) {
        sublists<int> p {x, size};
        list<stack<int>> pp = static_cast<list<stack<int>>> (p);

        EXPECT_EQ (pp.size (), p.size<size_t> ());

        auto pi = p.begin ();
        auto ppi = pp.begin ();
        while (pi != p.end ()) {
            EXPECT_EQ (*ppi, *pi);
            pi++;
            ppi++;
        }

        EXPECT_EQ (ppi, pp.end ());
    }

    TEST (Combinatorics, Sublists) {

        sublists_test_case (stack<int> {}, 0);
        sublists_test_case (stack<int> {}, 1);

        sublists_test_case (stack<int> {1}, 0);
        sublists_test_case (stack<int> {1}, 1);
        sublists_test_case (stack<int> {1}, 2);

        sublists_test_case (stack<int> {1, 2}, 0);
        sublists_test_case (stack<int> {1, 2}, 1);
        sublists_test_case (stack<int> {1, 2}, 2);
        sublists_test_case (stack<int> {1, 2}, 3);

        sublists_test_case (stack<int> {1, 2, 3}, 0);
        sublists_test_case (stack<int> {1, 2, 3}, 1);
        sublists_test_case (stack<int> {1, 2, 3}, 2);
        sublists_test_case (stack<int> {1, 2, 3}, 3);
        sublists_test_case (stack<int> {1, 2, 3}, 4);

        sublists_test_case (stack<int> {1, 2, 3, 4}, 0);
        sublists_test_case (stack<int> {1, 2, 3, 4}, 1);
        sublists_test_case (stack<int> {1, 2, 3, 4}, 2);
        sublists_test_case (stack<int> {1, 2, 3, 4}, 3);
        sublists_test_case (stack<int> {1, 2, 3, 4}, 4);
        sublists_test_case (stack<int> {1, 2, 3, 4}, 5);

    }

    void partitions_test_case (stack<int> x, math::nonzero<size_t> size, math::nonzero<size_t> offset) {
        partitions<int> p {x, size, offset};
        list<stack<int>> pp = static_cast<list<stack<int>>> (p);

        EXPECT_EQ (pp.size (), p.size<size_t> ());

        auto pi = p.begin ();
        auto ppi = pp.begin ();
        while (pi != p.end ()) {
            EXPECT_EQ (*ppi, *pi);
            pi++;
            ppi++;
        }

        EXPECT_EQ (ppi, pp.end ());
    }

    TEST (Combinatorics, Partitions) {

        partitions_test_case (stack<int> {}, math::nonzero<size_t> {1}, math::nonzero<size_t> {1});
        partitions_test_case (stack<int> {1}, math::nonzero<size_t> {1}, math::nonzero<size_t> {1});
        partitions_test_case (stack<int> {1, 2}, math::nonzero<size_t> {1}, math::nonzero<size_t> {1});
        partitions_test_case (stack<int> {1, 2}, math::nonzero<size_t> {2}, math::nonzero<size_t> {1});
        partitions_test_case (stack<int> {1, 2}, math::nonzero<size_t> {2}, math::nonzero<size_t> {2});
        partitions_test_case (stack<int> {1, 2, 3, 4, 5, 6, 7}, math::nonzero<size_t> {3}, math::nonzero<size_t> {3});
        partitions_test_case (stack<int> {1, 2, 3, 4, 5, 6, 7}, math::nonzero<size_t> {3}, math::nonzero<size_t> {2});
        partitions_test_case (stack<int> {1, 2, 3, 4, 5, 6, 7}, math::nonzero<size_t> {3}, math::nonzero<size_t> {1});
        partitions_test_case (stack<int> {1, 2, 3, 4, 5, 6, 7, 8}, math::nonzero<size_t> {3}, math::nonzero<size_t> {3});
        partitions_test_case (stack<int> {1, 2, 3, 4, 5, 6, 7, 8}, math::nonzero<size_t> {3}, math::nonzero<size_t> {2});
        partitions_test_case (stack<int> {1, 2, 3, 4, 5, 6, 7, 8}, math::nonzero<size_t> {3}, math::nonzero<size_t> {1});

    }
}

