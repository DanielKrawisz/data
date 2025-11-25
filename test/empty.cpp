// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/stack.hpp"
#include "data/list.hpp"
#include "data/tree.hpp"
#include "data/set.hpp"
#include "data/map.hpp"
#include "data/priority_queue.hpp"
#include "data/dispatch.hpp"
#include "data/cycle.hpp"
#include "data/cross.hpp"
#include "data/string.hpp"
#include "gtest/gtest.h"

    TEST (Empty, Empty) {

        EXPECT_TRUE  (empty (data::stack<int> {}));
        EXPECT_FALSE (empty (data::stack<int> {1}));

        EXPECT_TRUE  (data::empty (data::stack<int> {}));
        EXPECT_FALSE (data::empty (data::stack<int> {1}));

        EXPECT_TRUE  (empty (data::list<int> {}));
        EXPECT_FALSE (empty (data::list<int> {1}));

        EXPECT_TRUE  (data::empty (data::list<int> {}));
        EXPECT_FALSE (data::empty (data::list<int> {1}));

        EXPECT_TRUE  (empty (data::tree<int> {}));
        EXPECT_FALSE (empty (data::tree<int> {1}));

        EXPECT_TRUE  (data::empty (data::tree<int> {}));
        EXPECT_FALSE (data::empty (data::tree<int> {1}));

        EXPECT_TRUE  (empty (data::set<int> {}));
        EXPECT_FALSE (empty (data::set<int> {1}));

        EXPECT_TRUE  (data::empty (data::set<int> {}));
        EXPECT_FALSE (data::empty (data::set<int> {1}));

        EXPECT_TRUE  (empty (data::ordered_sequence<int> {}));
        EXPECT_FALSE (empty (data::ordered_sequence<int> {1}));

        EXPECT_TRUE  (data::empty (data::ordered_sequence<int> {}));
        EXPECT_FALSE (data::empty (data::ordered_sequence<int> {1}));

        EXPECT_TRUE  (empty (data::priority_queue<int> {}));
        EXPECT_FALSE (empty (data::priority_queue<int> {1}));

        EXPECT_TRUE  (data::empty (data::priority_queue<int> {}));
        EXPECT_FALSE (data::empty (data::priority_queue<int> {1}));

        EXPECT_TRUE  (empty (data::map<int, int> {}));
        EXPECT_FALSE (empty (data::map<int, int> {{1, 1}}));

        EXPECT_TRUE  (empty (data::cycle<int> {}));
        EXPECT_FALSE (empty (data::cycle<int> {1}));

        EXPECT_TRUE  (empty (data::dispatch<int, int> {}));
        EXPECT_FALSE (empty (data::dispatch<int, int> {{1, 1}}));

        // TODO the reason we have to use data::empty is
        // that these types inheret from std types, which
        // is considered bad practice anyway.
        EXPECT_TRUE (data::empty (data::cross<int> {}));
        EXPECT_TRUE (data::empty (data::string {}));

    }

    TEST (Empty, Size) {

        EXPECT_EQ (size (data::stack<int> {}), 0);
        EXPECT_EQ (size (data::stack<int> {1}), 1);

        EXPECT_EQ (data::size (data::stack<int> {}), 0);
        EXPECT_EQ (data::size (data::stack<int> {1}), 1);

        EXPECT_EQ (size (data::list<int> {}), 0);
        EXPECT_EQ (size (data::list<int> {1}), 1);

        EXPECT_EQ (data::size (data::list<int> {}), 0);
        EXPECT_EQ (data::size (data::list<int> {1}), 1);

        EXPECT_EQ (size (data::tree<int> {}), 0);
        EXPECT_EQ (size (data::tree<int> {1}), 1);

        EXPECT_EQ (data::size (data::tree<int> {}), 0);
        EXPECT_EQ (data::size (data::tree<int> {1}), 1);

        EXPECT_EQ (size (data::set<int> {}), 0);
        EXPECT_EQ (size (data::set<int> {1}), 1);

        EXPECT_EQ (data::size (data::set<int> {}), 0);
        EXPECT_EQ (data::size (data::set<int> {1}), 1);

        EXPECT_EQ (size (data::ordered_sequence<int> {}), 0);
        EXPECT_EQ (size (data::ordered_sequence<int> {1}), 1);

        EXPECT_EQ (data::size (data::ordered_sequence<int> {}), 0);
        EXPECT_EQ (data::size (data::ordered_sequence<int> {1}), 1);

        EXPECT_EQ (size (data::priority_queue<int> {}), 0);
        EXPECT_EQ (size (data::priority_queue<int> {1}), 1);

        EXPECT_EQ (size (data::map<int, int> {}), 0);
        EXPECT_EQ (size (data::map<int, int> {{1, 1}}), 1);

        EXPECT_EQ (size (data::cycle<int> {}), 0);
        EXPECT_EQ (size (data::cycle<int> {1}), 1);

        EXPECT_EQ (size (data::dispatch<int, int> {}), 0);
        EXPECT_EQ (size (data::dispatch<int, int> {{1, 1}}), 1);

        // TODO same problem here.
        EXPECT_EQ (data::size (data::cross<int> {}), 0);
        EXPECT_EQ (data::size (data::string {}), 0);

    }

