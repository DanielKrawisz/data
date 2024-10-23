// Copyright (c) 2020-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/string.hpp>
#include <data/numbers.hpp>
#include <data/tools/binary_search_tree.hpp>
#include "gtest/gtest.h"

namespace data {
    
    // test whether these data structures satisfy the correct interfaces. 
    TEST (TreeTest, TestTreeInterfaces) {
        
        static_assert (functional::buildable_tree<tree<int>>);
        static_assert (functional::buildable_tree<tree<int &>>);
        static_assert (functional::buildable_tree<tree<int *>>);
        
        static_assert (functional::buildable_tree<tree<const int>>);
        static_assert (functional::buildable_tree<tree<const int &>>);
        static_assert (functional::buildable_tree<tree<const int *>>);
        static_assert (functional::buildable_tree<tree<int *const>>);
        static_assert (functional::buildable_tree<tree<const int *const>>);
        
    }
    
    TEST (TreeTest, TestTreeSize) {

        tree<int> t0 = tree<int> {};
        tree<int> t1 = tree<int> {6};
        tree<int> t2 = tree<int> {4, tree<int> {}, tree<int> {3}};
        tree<int> t3 = tree<int> {2, tree<int> {1}, tree<int> {}};
        
        EXPECT_EQ (t0.size (), 0);
        EXPECT_EQ (t1.size (), 1);
        EXPECT_EQ (t2.size (), 2);
        EXPECT_EQ (t3.size (), 2);
        
        EXPECT_EQ (t0.right ().size (), 0);
        EXPECT_EQ (t1.right ().size (), 0);
        EXPECT_EQ (t2.right ().size (), 1);
        EXPECT_EQ (t3.right ().size (), 0);
        
        EXPECT_EQ (t0.left ().size (), 0);
        EXPECT_EQ (t1.left ().size (), 0);
        EXPECT_EQ (t2.left ().size (), 0);
        EXPECT_EQ (t3.left ().size (), 1);
        
    }
    
    TEST (TreeTest, TestTreeEqual) {
        
        tree<int> t1 {1, tree<int> {2, tree<int> {3}, tree<int> {}}, tree<int> {4}};
        tree<int> t2 {1, tree<int> {2, tree<int> {3}, tree<int> {}}, tree<int> {4}};
        tree<int> t3 {2, tree<int> {3, tree<int> {4}, tree<int> {}}, tree<int> {5}};
        tree<int> t4 {3, tree<int> {3, tree<int> {4}, tree<int> {7}}, tree<int> {5, tree<int> {6}, tree<int> {}}};
        
        EXPECT_EQ (t1, t1);
        EXPECT_EQ (t1, t2);
        EXPECT_NE (t1, t3);
        EXPECT_NE (t3, t4);
        EXPECT_NE (t4, t1);
        
        stack<int> v1 {3, 2, 1, 4};
        stack<int> v3 {4, 3, 2, 5};
        stack<int> v4 {4, 3, 7, 3, 6, 5};
        
        EXPECT_EQ (functional::values_infix<stack<int>> (t1), v1);
        EXPECT_EQ (functional::values_infix<stack<int>> (t2), v1);
        EXPECT_EQ (functional::values_infix<stack<int>> (t3), v3);
        EXPECT_EQ (functional::values_infix<stack<int>> (t4), v4);
        
        EXPECT_NE (functional::values_infix<stack<int>> (t1), v3);
        EXPECT_NE (functional::values_infix<stack<int>> (t2), v4);
        EXPECT_NE (functional::values_infix<stack<int>> (t3), v1);
        EXPECT_NE (functional::values_infix<stack<int>> (t4), v1);
        
    }
    
    void test_copy_linked_tree (tree<int> &p, int max) {
        p = tree<int> {};
        tree<int> new_tree {};
        for (int i = 0; i < max; i++) new_tree = tree<int> {i, new_tree, tree<int> {}};
        p = new_tree;
    }
    
    TEST (TreeTest, TestLinkedTreeCopy) {
        tree<int> p;
        test_copy_linked_tree (p, 7);
        EXPECT_EQ (p.size (), 7);
        EXPECT_EQ (p.left ().size (), 6);
        EXPECT_EQ (p.right ().size (), 0);
    }

    void accept_tree_of_string_views (linked_tree<string_view>) {}

    TEST (TreeTest, TestLinkedTreeConvert) {
        linked_tree<string> test {"1", 
            linked_tree<string> {"2"}, 
            linked_tree<string> {"3", linked_tree<string> {"4"}, linked_tree<string> {}}};

        accept_tree_of_string_views (test);

        linked_tree<N> numbers {1, linked_tree<N> {2}, linked_tree<N> {3, linked_tree<N> {4}, linked_tree<N> {}}};

        EXPECT_EQ (linked_tree<N> (test), numbers);

    }

    template <typename X>
    using bxt = binary_search_tree<X, linked_tree<X>>;

    TEST (TreeTest, TestBinarySearchTree) {

        static_assert (std::forward_iterator<decltype (std::declval<bxt<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<bxt<int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<bxt<int *>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<bxt<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<bxt<const int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<bxt<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<bxt<int *const>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<bxt<const int *const>> ().begin ())>);

        bxt<int> empty {};
        EXPECT_TRUE (empty.valid ());
        EXPECT_EQ (empty.begin (), empty.end ());

        EXPECT_EQ (empty, bxt<int> {});

        EXPECT_NE (bxt<int> {1}, bxt<int> {0});

        EXPECT_TRUE (bxt<int> {1}.valid ());
        EXPECT_TRUE (bxt<int> {0}.valid ());

        bxt<int> t3l {1, 2, 3};
        bxt<int> t3r {3, 1, 2};

        EXPECT_TRUE (bxt<int>::sorted (t3l));
        EXPECT_TRUE (bxt<int>::sorted (t3r));

        EXPECT_TRUE (t3l.valid ());
        EXPECT_TRUE (t3r.valid ());
        EXPECT_EQ (t3l, t3r);

        EXPECT_EQ ((bxt<int> {1, 2, 3, 4, 5, 6, 7}), (bxt<int> {7, 6, 5, 4, 3, 2, 1}));


    }

    namespace test_RB {

        template <typename V>
        void test_case_balanced (linked_tree<RB::colored<V>> t, bool expected) {
            EXPECT_EQ ((RB::balanced<V> (t)), expected);
            //EXPECT_TRUE ((RB::balanced<V> (RB::balance<V> (t))));
        }

        using colored = RB::colored<int>;
        using tree = linked_tree<RB::colored<int>>;

        colored inline black (int i) {
            return colored {RB::color::black, i};
        }

        colored inline red (int i) {
            return colored {RB::color::red, i};
        }

        TEST (TreeTest, TestRBBalanced) {

            test_case_balanced (tree {}, true);
            test_case_balanced (tree {red (1)}, true);
            test_case_balanced (tree {black (0)}, true);

            test_case_balanced (tree {red (0), tree {}, tree {black (3)}}, false);
            test_case_balanced (tree {red (0), tree {black (2)}, tree {}}, false);
            test_case_balanced (tree {red (0), tree {black (2)}, tree {black (3)}}, true);

            test_case_balanced (tree {red (0), tree {}, tree {red (3)}}, false);
            test_case_balanced (tree {red (0), tree {red (2)}, tree {}}, false);
            test_case_balanced (tree {red (0), tree {red (2)}, tree {red (3)}}, false);

        }
    }
    
}
