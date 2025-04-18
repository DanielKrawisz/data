// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    
    TEST (MapTest, TestMapInterface) {
        
        static_assert (functional::map<map<uint32, int>>);
        static_assert (functional::map<map<uint32, int *>>);
        static_assert (functional::map<map<uint32, int &>>);
/*
        static_assert (functional::map<map<uint32, const int>>);
        static_assert (functional::map<map<uint32, const int *>>);
        static_assert (functional::map<map<uint32, const int &>>);*/

        static_assert (std::forward_iterator<tool::rb_map_iterator<uint32, int>>);
        static_assert (std::forward_iterator<tool::rb_map_iterator<uint32, int *>>);
        static_assert (std::forward_iterator<tool::rb_map_iterator<uint32, int &>>);

        static_assert (std::forward_iterator<tool::rb_map_iterator<uint32, const int>>);
        static_assert (std::forward_iterator<tool::rb_map_iterator<uint32, const int *>>);
        static_assert (std::forward_iterator<tool::rb_map_iterator<uint32, const int &>>);
        
    }

    TEST (MapTest, TestOrderedSetInterface) {

        static_assert (functional::ordered_set<set<int>>);
        static_assert (functional::ordered_set<set<int *>>);/*
        static_assert (functional::ordered_set<set<int &>>);

        static_assert (functional::ordered_set<set<const int>>);
        static_assert (functional::ordered_set<set<const int *>>);
        static_assert (functional::ordered_set<set<const int &>>);*/
        
    }
    
    TEST (MapTest, TestMapEqual) {
        
        map<int, int> m1 {{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m2 {{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m3 {{5, 2}, {3, 5}, {8, 3}};
        map<int, int> m4 {{5, 2}, {3, 5}};
        
        EXPECT_EQ (m1, m1);
        EXPECT_EQ (m1, m2);
        EXPECT_NE (m1, m3);
        EXPECT_NE (m3, m4);
        EXPECT_NE (m4, m1);
        
        using e = entry<int, int>;
        stack<e> v1 {{1, 7}, {2, 1}, {3, 5}};
        stack<e> v3 {{3, 5}, {5, 2}, {8, 3}};
        stack<e> v4 {{3, 5}, {5, 2}};
        
        EXPECT_TRUE (m1.values () == v1);
        EXPECT_TRUE (m2.values () == v1);
        EXPECT_TRUE (m3.values () == v3);
        EXPECT_TRUE (m4.values () == v4);
        
        EXPECT_TRUE (m1.values () != v3);
        EXPECT_TRUE (m2.values () != v4);
        EXPECT_TRUE (m3.values () != v1);
        EXPECT_TRUE (m4.values () != v1);
    }
    
    TEST (MapTest, TestMapEmpty) {
        int i = 3;
        map<int, int*> m {{0, &i}};
        EXPECT_EQ (m[0], &i);
        EXPECT_EQ (m.contains (1), nullptr);
    }
    
    TEST (MapTest, TestRemoveFromMap) {
        
        map<int, int> m1 {{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m2 {{2, 1}, {1, 7}};
        
        auto m1r2 = m1.remove (3);
        
        EXPECT_EQ (m1r2, m2);
    }
    
    TEST (MapTest, TestIterate) {
        map<int, int> empty_map {};
        
        EXPECT_EQ (empty_map.begin (), empty_map.end ());
        
        entry<int, int> first_entry {7, 7};
        map<int, int> small_map {first_entry};
        
        auto small_begin = small_map.begin ();
        auto small_end = small_map.end ();
        
        EXPECT_NE (small_begin, small_end);

        EXPECT_EQ (*small_begin, first_entry);
        
        ++small_begin;
        EXPECT_EQ (small_begin, small_end);
        
    }

    TEST (MapTest, TestMapSet) {
        set<int> a;
        set<string> b;
        set<Z> c;
        set<N> d;
/*
        set<const int> ax;
        set<const string> bx;
        set<const Z> cx;
        set<const N> dx;*/
    }

    TEST (MapTest, TestInsert) {
        // it is not ok to insert the same key into a map twice
        EXPECT_THROW ((map<int, int> {}.insert (0, 1).insert (0, 2)), exception);
        // it's ok for a set though.
        EXPECT_NO_THROW (set<int> {}.insert (1).insert (1));
        // However, we can make it work by adding a special function.
        EXPECT_NO_THROW ((map<int, int> {}. insert (0, 1).insert (0, 2, [] (const int &a, const int &b) {
            return a;
        })));

    }
}

