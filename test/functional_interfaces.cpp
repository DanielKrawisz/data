// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/string.hpp"
#include "data/set.hpp"
#include "data/priority_queue.hpp"
#include "data/dispatch.hpp"
#include "data/cycle.hpp"
#include "data/remove.hpp"
#include "data/replace.hpp"
#include "data/select.hpp"
#include "data/lift.hpp"
#include "gtest/gtest.h"

static_assert (std::convertible_to<int &, const int &>);

// test whether data structures satisfy the correct interfaces. 
namespace data {

    static_assert (Sequence<decltype (values (std::declval<stack<int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<stack<const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<stack<string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<stack<const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<stack<const string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<list<int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<list<const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<list<string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<list<const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<list<const string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<ordered_sequence<const string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<priority_queue<int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<priority_queue<const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<priority_queue<string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<priority_queue<const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<priority_queue<const string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<set<int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<set<const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<set<string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<set<const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<set<const string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<map<int, int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<map<int, const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<map<int, string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<map<int, const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<map<int, const string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<dispatch<int, int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<dispatch<int, const int>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, const int *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, const int &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<dispatch<int, string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, string &>> ()))>);

    static_assert (Sequence<decltype (values (std::declval<dispatch<int, const string>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, const string *>> ()))>);
    static_assert (Sequence<decltype (values (std::declval<dispatch<int, const string &>> ()))>);

    template <typename X> struct test_has_functions {

        // equality (only supported for types with their own == operator)
        using stack_has_equal = decltype (equal (std::declval<stack<X>> (), std::declval<stack<X>> ()));
        using list_has_equal = decltype (equal (std::declval<list<X>> (), std::declval<list<X>> ()));
        using tree_has_equal = decltype (equal (std::declval<tree<X>> (), std::declval<tree<X>> ()));
        using set_has_equal = decltype (equal (std::declval<set<X>> (), std::declval<set<X>> ()));
        using ordered_sequence_has_equal = decltype (equal (std::declval<ordered_sequence<X>> (), std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_equal = decltype (equal (std::declval<priority_queue<X>> (), std::declval<priority_queue<X>> ()));
        using map_has_equal = decltype (equal (std::declval<map<size_t, X>> (), std::declval<map<size_t, X>> ()));
        using cycle_has_equal = decltype (equal (std::declval<cycle<X>> (), std::declval<cycle<X>> ()));
        using dispatch_has_equal = decltype (equal (std::declval<dispatch<size_t, X>> (), std::declval<dispatch<size_t, X>> ()));

        using stack_has_unequal = decltype (unequal (std::declval<stack<X>> (), std::declval<stack<X>> ()));
        using list_has_unequal = decltype (unequal (std::declval<list<X>> (), std::declval<list<X>> ()));
        using tree_has_unequal = decltype (unequal (std::declval<tree<X>> (), std::declval<tree<X>> ()));
        using set_has_unequal = decltype (unequal (std::declval<set<X>> (), std::declval<set<X>> ()));
        using ordered_sequence_has_unequal = decltype (unequal (std::declval<ordered_sequence<X>> (), std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_unequal = decltype (unequal (std::declval<priority_queue<X>> (), std::declval<priority_queue<X>> ()));
        using map_has_unequal = decltype (unequal (std::declval<map<size_t, X>> (), std::declval<map<size_t, X>> ()));
        using cycle_has_unequal = decltype (unequal (std::declval<cycle<X>> (), std::declval<cycle<X>> ()));
        using dispatch_has_unequal = decltype (unequal (std::declval<dispatch<size_t, X>> (), std::declval<dispatch<size_t, X>> ()));

        // basic container functions
        using stack_has_empty = decltype (empty (std::declval<stack<X>> ()));
        using list_has_empty = decltype (empty (std::declval<list<X>> ()));
        using tree_has_empty = decltype (empty (std::declval<tree<X>> ()));
        using set_has_empty = decltype (empty (std::declval<set<X>> ()));
        using ordered_sequence_has_empty = decltype (empty (std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_empty = decltype (empty (std::declval<priority_queue<X>> ()));
        using map_has_empty = decltype (empty (std::declval<map<size_t, X>> ()));
        using cycle_has_empty = decltype (empty (std::declval<cycle<X>> ()));
        using dispatch_has_empty = decltype (empty (std::declval<dispatch<size_t, X>> ()));
    
        using stack_has_size = decltype (size (std::declval<stack<X>> ()));
        using list_has_size = decltype (size (std::declval<list<X>> ()));
        using tree_has_size = decltype (size (std::declval<tree<X>> ()));
        using set_has_size = decltype (size (std::declval<set<X>> ()));
        using ordered_sequence_has_size = decltype (size (std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_size = decltype (size (std::declval<priority_queue<X>> ()));
        using map_has_size = decltype (size (std::declval<map<size_t, X>> ()));
        using cycle_has_size = decltype (size (std::declval<cycle<X>> ()));
        using dispatch_has_size = decltype (size (std::declval<dispatch<size_t, X>> ()));

        // tree doesn't have a values method because it doesn't have an
        // iteration order.
        using stack_has_values = decltype (values (std::declval<stack<X>> ()));
        using list_has_values = decltype (values (std::declval<list<X>> ()));
        using ordered_sequence_has_values = decltype (values (std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_values = decltype (values (std::declval<priority_queue<X>> ()));
        using set_has_values = decltype (values (std::declval<set<X>> ()));
        using map_has_values = decltype (values (std::declval<map<size_t, X>> ()));
        using dispatch_has_values = decltype (values (std::declval<dispatch<size_t, X>> ()));

        // stack and queue functions
        using stack_has_first = decltype (first (std::declval<stack<X>> ()));
        using list_has_first = decltype (first (std::declval<list<X>> ()));
        using ordered_sequence_has_first = decltype (first (std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_first = decltype (first (std::declval<priority_queue<X>> ()));
        using dispatch_has_first = decltype (first (std::declval<dispatch<size_t, X>> ()));

        using stack_has_rest = decltype (rest (std::declval<stack<X>> ()));
        using list_has_rest = decltype (rest (std::declval<list<X>> ()));
        using ordered_sequence_has_rest = decltype (rest (std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_rest = decltype (rest (std::declval<priority_queue<X>> ()));
        using dispatch_has_rest = decltype (rest (std::declval<dispatch<size_t, X>> ()));

        using stack_has_take = decltype (take (std::declval<stack<X>> (), 0));
        using list_has_take = decltype (take (std::declval<list<X>> (), 0));
        using ordered_sequence_has_take = decltype (take (std::declval<ordered_sequence<X>> (), 0));
        using dispatch_has_take = decltype (take (std::declval<dispatch<size_t, X>> (), 0));

        using stack_has_drop = decltype (drop (std::declval<stack<X>> (), 0));
        using list_has_drop = decltype (drop (std::declval<list<X>> (), 0));
        using ordered_sequence_has_drop = decltype (drop (std::declval<ordered_sequence<X>> (), 0));
        using dispatch_has_drop = decltype (drop (std::declval<dispatch<size_t, X>> (), 0));
        
        using stack_has_reverse = decltype (reverse (std::declval<stack<X>> ()));
        using list_has_reverse = decltype (reverse (std::declval<list<X>> ()));
        using dispatch_has_reverse = decltype (reverse (std::declval<dispatch<size_t, X>> ()));
        using cycle_has_reverse = decltype (reverse (std::declval<cycle<X>> ()));
        
        using stack_has_join = decltype (join (std::declval<stack<X>> (), std::declval<stack<X>> ()));
        using list_has_join = decltype (join (std::declval<list<X>> (), std::declval<list<X>> ()));
        using dispatch_has_join = decltype (join (std::declval<dispatch<size_t, X>> (), std::declval<dispatch<size_t, X>> ()));
        
        using stack_has_plus = decltype (std::declval<stack<X>> () + std::declval<stack<X>> ());
        using list_has_plus = decltype (std::declval<list<X>> () + std::declval<list<X>> ());
        using dispatch_has_plus = decltype (std::declval<dispatch<size_t, X>> () + std::declval<dispatch<size_t, X>> ());

        using ordered_sequence_has_merge = decltype (merge (std::declval<ordered_sequence<X>> (), std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_merge = decltype (merge (std::declval<priority_queue<X>> (), std::declval<priority_queue<X>> ()));
        using set_has_merge = decltype (merge (std::declval<set<X>> (), std::declval<set<X>> ()));
        using map_has_merge = decltype (merge (std::declval<map<size_t, X>> (), std::declval<map<size_t, X>> ()));

        using ordered_sequence_has_and = decltype (std::declval<ordered_sequence<X>> () & std::declval<ordered_sequence<X>> ());
        using priority_queue_has_and = decltype (std::declval<priority_queue<X>> () & std::declval<priority_queue<X>> ());
        using set_has_and = decltype (std::declval<set<X>> () & std::declval<set<X>> ());
        using map_has_and = decltype (std::declval<map<size_t, X>> () & std::declval<map<size_t, X>> ());

        using ordered_sequence_has_or = decltype (std::declval<ordered_sequence<X>> () | std::declval<ordered_sequence<X>> ());
        using priority_queue_has_or = decltype (std::declval<priority_queue<X>> () | std::declval<priority_queue<X>> ());
        using set_has_or = decltype (std::declval<set<X>> () | std::declval<set<X>> ());
        using map_has_or = decltype (std::declval<map<size_t, X>> () | std::declval<map<size_t, X>> ());

        using ordered_sequence_has_xor = decltype (std::declval<ordered_sequence<X>> () ^ std::declval<ordered_sequence<X>> ());
        using priority_queue_has_xor = decltype (std::declval<priority_queue<X>> () ^ std::declval<priority_queue<X>> ());
        using set_has_xor = decltype (std::declval<set<X>> () ^ std::declval<set<X>> ());
        using map_has_xor = decltype (std::declval<map<size_t, X>> () ^ std::declval<map<size_t, X>> ());
  
        using stack_has_sort = decltype (sort (std::declval<stack<X>> ()));
        using list_has_sort = decltype (sort (std::declval<list<X>> ()));
        using ordered_sequence_has_sort = decltype (sort (std::declval<ordered_sequence<X>> ()));
        using prority_queue_has_sort = decltype (sort (std::declval<priority_queue<X>> ()));
        
        using stack_has_sorted = decltype (sorted (std::declval<stack<X>> ()));
        using list_has_sorted = decltype (sorted (std::declval<list<X>> ()));
        using ordered_sequence_has_sorted = decltype (sorted (std::declval<ordered_sequence<X>> ()));
        using priority_queue_has_sorted = decltype (sorted (std::declval<priority_queue<X>> ()));

        using list_has_rotate_right = decltype (rotate_right (std::declval<list<X>> (), 0));
        using list_has_rotate_left = decltype (rotate_left (std::declval<list<X>> (), 0));

        // tree functions
        using tree_has_root = decltype (root (std::declval<tree<X>> ()));
        using tree_has_left = decltype (left (std::declval<tree<X>> ()));
        using tree_has_right = decltype (right (std::declval<tree<X>> ()));
        
        using stack_has_replace = decltype (replace (std::declval<stack<X>> (), {}));
        using list_has_replace = decltype (replace (std::declval<list<X>> (), {}));
        using tree_has_replace = decltype (replace (std::declval<tree<X>> (), {}));
        using map_has_replace = decltype (replace (std::declval<map<size_t, X>> (), {}));
        using cycle_has_replace = decltype (replace (std::declval<cycle<X>> (), {}));

        using map_has_contains = decltype (contains (std::declval<map<size_t, X>> (), size_t {0}));
    };

    template <typename X> bool doop (X &&) {
        return true;
    }

    template <typename X> struct test_has_contains {

        template <typename Y> 
        test_has_contains (Y y) {

            using set_has_insert = decltype (insert (std::declval<set<X>> (), y));
            using ordered_sequence_has_insert = decltype (insert (std::declval<ordered_sequence<X>> (), y));
            using priority_queue_has_insert = decltype (insert (std::declval<priority_queue<X>> (), y));
            using map_has_insert = decltype (insert (std::declval<map<size_t, X>> (), size_t {0}, y));

            using stack_has_prepend = decltype (prepend (std::declval<stack<X>> (), y));
            using list_has_prepend = decltype (prepend (std::declval<list<X>> (), y));

            using list_has_append = decltype (append (std::declval<list<X>> (), y));

            using stack_has_contains = decltype (contains (std::declval<stack<X>> (), y));
            using list_has_contains = decltype (contains (std::declval<list<X>> (), y));
            using tree_has_contains = decltype (contains (std::declval<tree<X>> (), y));
            using set_has_contains = decltype (contains (std::declval<set<X>> (), y));
            using ordered_sequence_has_contains = decltype (contains (std::declval<ordered_sequence<X>> (), y));
            using priority_queue_has_contains = decltype (contains (std::declval<priority_queue<X>> (), y));

            using stack_has_remove = decltype (remove (std::declval<stack<X>> (), 0));
            using list_has_remove = decltype (remove (std::declval<list<X>> (), 0));
            using ordered_sequence_has_remove = decltype (remove (std::declval<ordered_sequence<X>> (), 0));
            using set_has_remove = decltype (remove (std::declval<set<X>> (), 0));
            using map_has_remove = decltype (remove (std::declval<map<size_t, X>> (), 0));

            // NOTE: these two functions should not exist, so why do they?
            using tree_has_remove = decltype (remove (std::declval<tree<X>> (), 0));
            using priority_queue_has_remove = decltype (remove (std::declval<priority_queue<X>> (), 0));

            using stack_has_erase = decltype (erase (std::declval<stack<X>> (), y));
            using list_has_erase = decltype (erase (std::declval<list<X>> (), y));
            using ordered_sequence_has_erase = decltype (erase (std::declval<ordered_sequence<X>> (), y));
            using set_has_erase = decltype (erase (std::declval<set<X>> (), y));
            using map_has_erase = decltype (erase (std::declval<map<size_t, X>> (), y));

            using stack_has_replace = decltype (replace (std::declval<stack<X>> (), {{y, y}}));
            using list_has_replace = decltype (replace (std::declval<list<X>> (), {{y, y}}));
            using tree_has_replace = decltype (replace (std::declval<tree<X>> (), {{y, y}}));
            using map_has_replace = decltype (replace (std::declval<map<size_t, X>> (), {{y, y}}));
/*
            using stack_has_replace_part = decltype (replace_part (std::declval<stack<X>> (), size_t {0}, y));
            using list_has_replace_part = decltype (replace_part (std::declval<list<X>> (), size_t {0}, y));
            using map_has_replace_part = decltype (replace_part (std::declval<map<size_t, X>> (), size_t {0}, y));

            using stack_has_select = decltype (select (std::declval<stack<X>> (), &doop<y>));
            using list_has_select = decltype (select (std::declval<list<X>> (), &doop<y>));
            using set_has_select = decltype (select (std::declval<set<X>> (), &doop<y>));
            using ordered_sequence_has_select = decltype (select (std::declval<ordered_sequence<X>> (), &doop<y>));
            using priority_queue_has_select = decltype (select (std::declval<priority_queue<X>> (), &doop<y>));
            using map_has_select = decltype (select (std::declval<map<size_t, X>> (), &doop<y>));*/

        }

    };

    TEST (FunctionalInterface, Container) {

        test_has_functions<int> {};
        test_has_functions<const int> {};
        test_has_functions<int &> {};
        test_has_functions<const int &> {};
        test_has_functions<int *> {};
        test_has_functions<int *const> {};
        test_has_functions<int const *> {};
        test_has_functions<int const *const> {};

        test_has_functions<string> {};
        test_has_functions<const string> {};
        test_has_functions<string &> {};
        test_has_functions<const string &> {};
        test_has_functions<string *> {};
        test_has_functions<string *const> {};
        test_has_functions<string const *> {};
        test_has_functions<string const *const> {};

        test_has_contains<int> {0};
        test_has_contains<const int> {0};

        int test_var = 0;
        test_has_contains<int &> {test_var};
        test_has_contains<const int &> {test_var};

        int *test_ptr = &test_var;
        test_has_contains<int *> {test_ptr};
        test_has_contains<int * const> {test_ptr};
        test_has_contains<int const *> {test_ptr};

        test_has_contains<string> {""};
        test_has_contains<const string> {""};

        string test_str {};
        test_has_contains<string &> {test_str};
        test_has_contains<const string &> {test_str};

        string *test_str_ptr = &test_str;
        test_has_contains<string *> {test_str_ptr};
        test_has_contains<string * const> {test_str_ptr};
        test_has_contains<string const *> {test_str_ptr};

    }
}
