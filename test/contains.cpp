// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/string.hpp"
#include "data/cross.hpp"
#include "data/list.hpp"
#include "data/ordered_sequence.hpp"
#include "data/priority_queue.hpp"
#include "data/set.hpp"
#include "data/remove.hpp"
#include "gtest/gtest.h"

    template <typename elem> using vector = std::vector<elem>;
    template <typename elem> using cross = data::cross<elem>;
    template <typename elem> using stack = data::stack<elem>;
    template <typename elem> using list = data::list<elem>;
    template <typename elem> using set = data::set<elem>;
    template <typename elem> using ordered_sequence = data::ordered_sequence<elem>;
    template <typename elem> using priority_queue = data::priority_queue<elem>;
    using string = data::string;

    static_assert (data::ConstIterable<cross<string>>);
    static_assert (!data::ConstIterable<priority_queue<string>>);

    TEST (Contains, Contains) {
        
        EXPECT_TRUE (contains (stack<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (stack<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (data::contains (stack<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (data::contains (stack<string> {"a", "b", "c", "d"}, "f"));
        
        EXPECT_TRUE (contains (cross<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (cross<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (data::contains (cross<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (data::contains (cross<string> {"a", "b", "c", "d"}, "f"));
        
        EXPECT_TRUE (contains (list<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (list<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (data::contains (list<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (data::contains (list<string> {"a", "b", "c", "d"}, "f"));
        
        EXPECT_TRUE (contains (ordered_sequence<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (ordered_sequence<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (data::contains (ordered_sequence<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (data::contains (ordered_sequence<string> {"a", "b", "c", "d"}, "f"));
        
        EXPECT_TRUE (contains (set<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (set<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (data::contains (set<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (data::contains (set<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (contains (priority_queue<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (contains (priority_queue<string> {"a", "b", "c", "d"}, "f"));

        EXPECT_TRUE (data::contains (priority_queue<string> {"a", "b", "c", "d"}, "a"));
        EXPECT_FALSE (data::contains (priority_queue<string> {"a", "b", "c", "d"}, "f"));

        // TODO tree

        // contains works differently with map. 
        //EXPECT_TRUE (contains (map<string, int> {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}}, "a"));
        //EXPECT_FALSE (contains (map<string, int> {{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}}, "f"));
    }

    TEST (Contains, Erase) {

        EXPECT_EQ ((erase (stack<string> {"a", "b", "c", "d"}, "c")), (stack<string> {"a", "b", "d"}));
        EXPECT_EQ ((erase (stack<string> {"a", "b", "c", "d"}, "f")), (stack<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((data::erase (stack<string> {"a", "b", "c", "d"}, "c")), (stack<string> {"a", "b", "d"}));
        EXPECT_EQ ((data::erase (stack<string> {"a", "b", "c", "d"}, "f")), (stack<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((erase (list<string> {"a", "b", "c", "d"}, "c")), (list<string> {"a", "b", "d"}));
        EXPECT_EQ ((erase (list<string> {"a", "b", "c", "d"}, "f")), (list<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((data::erase (list<string> {"a", "b", "c", "d"}, "c")), (list<string> {"a", "b", "d"}));
        EXPECT_EQ ((data::erase (list<string> {"a", "b", "c", "d"}, "f")), (list<string> {"a", "b", "c", "d"}));

        //EXPECT_EQ ((erase (ordered_sequence<string> {"a", "b", "c", "d"}, "c")), (ordered_sequence<string> {"a", "b", "d"}));
        //EXPECT_EQ ((erase (ordered_sequence<string> {"a", "b", "c", "d"}, "f")), (ordered_sequence<string> {"a", "b", "c", "d"}));

        //EXPECT_EQ ((erase (set<string> {"a", "b", "c", "d"}, "c")), (set<string> {"a", "b", "d"}));
        //EXPECT_EQ ((erase (set<string> {"a", "b", "c", "d"}, "f")), (set<string> {"a", "b", "c", "d"}));

        // TODO map

    }

    TEST (Contains, Remove) {

        EXPECT_EQ ((remove (stack<string> {"a", "b", "c", "d"}, 0)), (stack<string> {"b", "c", "d"}));
        EXPECT_EQ ((remove (stack<string> {"a", "b", "c", "d"}, 5)), (stack<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((data::remove (stack<string> {"a", "b", "c", "d"}, 0)), (stack<string> {"b", "c", "d"}));
        EXPECT_EQ ((data::remove (stack<string> {"a", "b", "c", "d"}, 5)), (stack<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((remove (list<string> {"a", "b", "c", "d"}, 0)), (list<string> {"b", "c", "d"}));
        EXPECT_EQ ((remove (list<string> {"a", "b", "c", "d"}, 5)), (list<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((data::remove (list<string> {"a", "b", "c", "d"}, 0)), (list<string> {"b", "c", "d"}));
        EXPECT_EQ ((data::remove (list<string> {"a", "b", "c", "d"}, 5)), (list<string> {"a", "b", "c", "d"}));
/*
        EXPECT_EQ ((remove (ordered_sequence<string> {"a", "b", "c", "d"}, 0)), (ordered_sequence<string> {"b", "c", "d"}));
        EXPECT_EQ ((remove (ordered_sequence<string> {"a", "b", "c", "d"}, 5)), (ordered_sequence<string> {"a", "b", "c", "d"}));

        EXPECT_EQ ((data::remove (ordered_sequence<string> {"a", "b", "c", "d"}, 0)), (ordered_sequence<string> {"b", "c", "d"}));
        EXPECT_EQ ((data::remove (ordered_sequence<string> {"a", "b", "c", "d"}, 5)), (ordered_sequence<string> {"a", "b", "c", "d"}));*/

        // TODO map

    }


