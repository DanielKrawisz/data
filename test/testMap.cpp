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

        static_assert (functional::map<map<uint32, const int>>);
        static_assert (functional::map<map<uint32, const int *>>);
        static_assert (functional::map<map<uint32, const int &>>);

        static_assert (std::forward_iterator<map<uint32, int>::iterator>);
        static_assert (std::forward_iterator<map<uint32, int *>::iterator>);
        static_assert (std::forward_iterator<map<uint32, int &>::iterator>);

        static_assert (std::forward_iterator<map<uint32, const int>::iterator>);
        static_assert (std::forward_iterator<map<uint32, const int *>::iterator>);
        static_assert (std::forward_iterator<map<uint32, const int &>::iterator>);
        
    }

    TEST (MapTest, TestOrderedSetInterface) {

        static_assert (functional::ordered_set<set<int>>);
        static_assert (functional::ordered_set<set<int *>>);
        static_assert (functional::ordered_set<set<int &>>);

        static_assert (functional::ordered_set<set<const int>>);
        static_assert (functional::ordered_set<set<const int *>>);
        static_assert (functional::ordered_set<set<const int &>>);
        
    }
    
    TEST (MapTest, TestMapEqual) {
        
        map<int, int> m1 {{2, 1}, {3, 5}, {1, 7}};
        map<int, int> m2 {{3, 5}, {2, 1}, {1, 7}};
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

        std::cout << "map 1 is " << m1 << std::endl;

        auto m1v = m1.values ();
        auto m2v = m2.values ();
        auto m3v = m3.values ();
        auto m4v = m4.values ();
        
        EXPECT_TRUE (m1v == v1) << "expected " << m1v << " == " << v1;
        EXPECT_TRUE (m2v == v1) << "expected " << m2v << " == " << v1;
        EXPECT_TRUE (m3v == v3) << "expected " << m3v << " == " << v3;
        EXPECT_TRUE (m4v == v4) << "expected " << m4v << " == " << v4;
        
        EXPECT_TRUE (m1v != v3);
        EXPECT_TRUE (m2v != v4);
        EXPECT_TRUE (m3v != v1);
        EXPECT_TRUE (m4v != v1);
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

    TEST (MapTest, TestInsert) {
        using key_already_exists = map<int, int>::key_already_exists;

        // it is not ok to insert the same key into a map twice
        EXPECT_THROW ((map<int, int> {}.insert (0, 1).insert (0, 2)), key_already_exists);
        // it's ok for a set though.
        EXPECT_NO_THROW (set<int> {}.insert (1).insert (1));
        // However, we can make it work by adding a special function.
        EXPECT_NO_THROW ((map<int, int> {}. insert (0, 1).insert (0, 2, [] (const int &a, const int &b) -> const int & {
            return a;
        })));

    }
}

