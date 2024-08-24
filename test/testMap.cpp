// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "interface_tests.hpp"
#include <data/numbers.hpp>
#include "gtest/gtest.h"

namespace data {
    
    TEST (MapTest, TestMapInterface) {
        
        is_map<map<uint32, int>> ();
        is_map<map<uint32, int *>> ();
        is_map<map<uint32, int &>> ();

        is_map<map<uint32, const int>> ();
        is_map<map<uint32, const int *>> ();
        is_map<map<uint32, const int &>> ();

        static_assert (std::forward_iterator<map<uint32, int>::iterator>);
        static_assert (std::forward_iterator<map<uint32, int *>::iterator>);
        static_assert (std::forward_iterator<map<uint32, int &>::iterator>);

        static_assert (std::forward_iterator<map<uint32, const int>::iterator>);
        static_assert (std::forward_iterator<map<uint32, const int *>::iterator>);
        static_assert (std::forward_iterator<map<uint32, const int &>::iterator>);
        
    }

    TEST (MapTest, TestOrderedSetInterface) {

        is_ordered_set<set<int>> ();
        is_ordered_set<set<int *>> ();
        is_ordered_set<set<int &>> ();

        is_ordered_set<set<const int>> ();
        is_ordered_set<set<const int *>> ();
        is_ordered_set<set<const int &>> ();
        
    }

    template <typename K, typename V>
    void test_case_balanced (linked_tree<tool::RB::entry<const K, V>> t, bool expected) {
        bool balanced = map<K, V>::balanced (t);
        EXPECT_EQ (balanced, expected);
        bool balanced_next = map<K, V>::balanced (map<K, V>::balance (t));
        EXPECT_TRUE (balanced_next);
    }

    TEST (MapTest, TestMapBalanced) {

        test_case_balanced (map<int, int> (), true);
        test_case_balanced (map<int, int> {{1, 1}}, true);

        // need more test cases. (both negative and positive)
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
        
        using e = entry<const int, int>;
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
        
        entry<const int, int> first_entry {7, 7};
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

        set<const int> ax;
        set<const string> bx;
        set<const Z> cx;
        set<const N> dx;
    }
}

