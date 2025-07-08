// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/string.hpp>
#include "gtest/gtest.h"

static_assert (std::convertible_to<int &, const int &>);

/* 

  sequence of X: 
    in: const X &
    out: X &
    const out: X &

  sequence of const X: 
    in: const X &
    out: const X &
    const out: const X &

  sequence of X &
    in: X &
    out: X &
    const out: X &

  sequence of X &
    in: X &
    out: X &
    const out: X &

*/

// test whether data structures satisfy the correct interfaces. 
namespace data {

/*
    // a pointer to a const int
    const int *
    int const *

    // a const pointer to int
    int *const 

    // a const pointer to const int 
    const int *const
    int const *const
*/
    
    TEST (FunctionalInterfaceTest, TestSequence) {

        static_assert (Stack<stack<int>>);
        static_assert (Stack<stack<int *>>);
        static_assert (Stack<stack<int &>>);
        
        static_assert (Stack<stack<const int>>);
        static_assert (Stack<stack<const int *>>);
        static_assert (Stack<stack<const int &>>);
        
        static_assert (Stack<stack<string>>);
        static_assert (Stack<stack<string *>>);
        static_assert (Stack<stack<string &>>);
        
        static_assert (Stack<stack<const string>>);
        static_assert (Stack<stack<const string *>>);
        static_assert (Stack<stack<const string &>>);
        
        static_assert (List<list<int>>);
        static_assert (List<list<int *>>);
        static_assert (List<list<int &>>);
        
        static_assert (List<list<const int>>);
        static_assert (List<list<const int *>>);
        static_assert (List<list<const int &>>);
        
        static_assert (List<list<string>>);
        static_assert (List<list<string *>>);
        static_assert (List<list<string &>>);
        
        static_assert (List<list<const string>>);
        static_assert (List<list<const string *>>);
        static_assert (List<list<const string &>>);
        
        static_assert (Sequence<ordered_list<int>>);
        static_assert (Sequence<ordered_list<int *>>);
        static_assert (Sequence<ordered_list<int &>>);
        
        static_assert (Sequence<ordered_list<const int>>);
        static_assert (Sequence<ordered_list<const int *>>);
        static_assert (Sequence<ordered_list<const int &>>);
        
        static_assert (Sequence<ordered_list<string>>);
        static_assert (Sequence<ordered_list<string *>>);
        static_assert (Sequence<ordered_list<string &>>);
        
        static_assert (Sequence<ordered_list<const string>>);
        static_assert (Sequence<ordered_list<const string *>>);
        static_assert (Sequence<ordered_list<const string &>>);

        static_assert (Container<ordered_list<int>, int>);
        static_assert (Container<ordered_list<int *>, int *>);
        static_assert (Container<ordered_list<int &>, int &>);
        
        static_assert (Container<ordered_list<const int>, const int>);
        static_assert (Container<ordered_list<const int *>, const int *>);
        static_assert (Container<ordered_list<const int &>, const int &>);
        
        static_assert (Container<ordered_list<string>, string>);
        static_assert (Container<ordered_list<string *>, string *>);
        static_assert (Container<ordered_list<string &>, string &>);
        
        static_assert (Container<ordered_list<const string>, const string>);
        static_assert (Container<ordered_list<const string *>, const string *>);
        static_assert (Container<ordered_list<const string &>, const string &>);
        
        static_assert (Sequence<priority_queue<int>>);
        static_assert (Sequence<priority_queue<int *>>);
        static_assert (Sequence<priority_queue<int &>>);
        
        static_assert (Sequence<priority_queue<const int>>);
        static_assert (Sequence<priority_queue<const int *>>);
        static_assert (Sequence<priority_queue<const int &>>);
        
        static_assert (Sequence<priority_queue<string>>);
        static_assert (Sequence<priority_queue<string *>>);
        static_assert (Sequence<priority_queue<string &>>);
        
        static_assert (Sequence<priority_queue<const string>>);
        static_assert (Sequence<priority_queue<const string *>>);
        static_assert (Sequence<priority_queue<const string &>>);
        
        static_assert (Container<priority_queue<int>, int>);
        static_assert (Container<priority_queue<int *>, int *>);
        static_assert (Container<priority_queue<int &>, int &>);
        
        static_assert (Container<priority_queue<const int>, const int>);
        static_assert (Container<priority_queue<const int *>, const int *>);
        static_assert (Container<priority_queue<const int &>, const int &>);
        
        static_assert (Container<priority_queue<string>, string>);
        static_assert (Container<priority_queue<string *>, string *>);
        static_assert (Container<priority_queue<string &>, string &>);
        
        static_assert (Container<priority_queue<const string>, const string>);
        static_assert (Container<priority_queue<const string *>, const string *>);
        static_assert (Container<priority_queue<const string &>, const string &>);
        
    }

    TEST (FunctionalInterfaceTest, TestIterable) {

        static_assert (std::forward_iterator<decltype (std::declval<const stack<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<int &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const stack<int>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const stack<int *>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const stack<int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const stack<string>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<string *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<string &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const stack<string>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const stack<string &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const list<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<int &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const list<const int>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const list<const int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const list<string>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<string *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<string &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const list<const string>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const list<const string &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<int &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const ordered_list<int>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const ordered_list<int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const int &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const ordered_list<const int>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const ordered_list<const int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<string>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<string *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<string &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const string>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const string *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_list<const string &>> ().begin ())>);

        static_assert (std::indirectly_readable<decltype (std::declval<const set<int>> ().begin ())>);
        static_assert (std::indirectly_readable<decltype (std::declval<const set<int *>> ().begin ())>);
        static_assert (std::indirectly_readable<decltype (std::declval<const set<int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const set<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<int &>> ().begin ())>);

        static_assert (Reference<decltype (*std::declval<const set<int>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const set<int *>> ().begin ())>);
        static_assert (Reference<decltype (*std::declval<const set<int &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const set<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<const int &>> ().begin ())>);

        static_assert (Same<decltype (*std::declval<const set<int>> ().begin ()), const int &>);
        static_assert (Same<decltype (*std::declval<const set<const int>> ().begin ()), const int &>);
        
        static_assert (Same<decltype (*std::declval<const set<int &>> ().begin ()), int &>);
        static_assert (Same<decltype (*std::declval<const set<const int &>> ().begin ()), const int &>);

        static_assert (Same<decltype (*std::declval<const set<int *>> ().begin ()), int *const &>);
        static_assert (Same<decltype (*std::declval<const set<const int *>> ().begin ()), const int *const &>);

        static_assert (std::forward_iterator<decltype (std::declval<const set<string>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<string *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<string &>> ().begin ())>);

        static_assert (std::forward_iterator<decltype (std::declval<const set<const string>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<const string *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const set<const string &>> ().begin ())>);

    }

    template <typename X> struct test_has_functions {

        // basic container functions
        using stack_has_empty = decltype (empty (std::declval<stack<X>> ()));
        using list_has_empty = decltype (empty (std::declval<list<X>> ()));
        using tree_has_empty = decltype (empty (std::declval<tree<X>> ()));
        using set_has_empty = decltype (empty (std::declval<set<X>> ()));
        using ordered_list_has_empty = decltype (empty (std::declval<ordered_list<X>> ()));
        using priority_queue_has_empty = decltype (empty (std::declval<priority_queue<X>> ()));
        using map_has_empty = decltype (empty (std::declval<map<X, X>> ()));
    
        using stack_has_size = decltype (size (std::declval<stack<X>> ()));
        using list_has_size = decltype (size (std::declval<list<X>> ()));/*
        using tree_has_size = decltype (size (std::declval<tree<X>> ()));
        using set_has_size = decltype (size (std::declval<set<X>> ()));
        using ordered_list_has_size = decltype (size (std::declval<ordered_list<X>> ()));
        using priority_queue_has_size = decltype (size (std::declval<priority_queue<X>> ()));
        using map_has_size = decltype (size (std::declval<map<X, X>> ()));*/
    
        // equality (only supported for types with their own == operator)
        using stack_has_equal = decltype (equal (std::declval<stack<X>> (), std::declval<stack<X>> ()));
        using list_has_equal = decltype (equal (std::declval<list<X>> (), std::declval<list<X>> ()));
        using tree_has_equal = decltype (equal (std::declval<tree<X>> (), std::declval<tree<X>> ()));
        using set_has_equal = decltype (equal (std::declval<set<X>> (), std::declval<set<X>> ()));
        using ordered_list_has_equal = decltype (equal (std::declval<ordered_list<X>> (), std::declval<ordered_list<X>> ()));
        using priority_queue_has_equal = decltype (equal (std::declval<priority_queue<X>> (), std::declval<priority_queue<X>> ()));
        using map_has_equal = decltype (equal (std::declval<map<X, X>> (), std::declval<map<X, X>> ()));

        using stack_has_unequal = decltype (unequal (std::declval<stack<X>> (), std::declval<stack<X>> ()));
        using list_has_unequal = decltype (unequal (std::declval<list<X>> (), std::declval<list<X>> ()));
        using tree_has_unequal = decltype (unequal (std::declval<tree<X>> (), std::declval<tree<X>> ()));
        using set_has_unequal = decltype (unequal (std::declval<set<X>> (), std::declval<set<X>> ()));
        using ordered_list_has_unequal = decltype (unequal (std::declval<ordered_list<X>> (), std::declval<ordered_list<X>> ()));
        using priority_queue_has_unequal = decltype (unequal (std::declval<priority_queue<X>> (), std::declval<priority_queue<X>> ()));
        using map_has_unequal = decltype (unequal (std::declval<map<X, X>> (), std::declval<map<X, X>> ()));

        // tree doesn't have a values method because it doesn't have an 
        // iteration order. 
        using stack_has_values = decltype (values (std::declval<stack<X>> ()));
        using list_has_values = decltype (values (std::declval<list<X>> ()));
        using ordered_list_has_values = decltype (values (std::declval<ordered_list<X>> ()));
        using priority_queue_has_values = decltype (values (std::declval<priority_queue<X>> ()));
        using set_has_values = decltype (values (std::declval<set<X>> ()));
        using map_has_values = decltype (values (std::declval<map<X, X>> ()));

        // stack and queue functions
        using stack_has_first = decltype (first (std::declval<stack<X>> ()));
        using list_has_first = decltype (first (std::declval<list<X>> ()));
        using ordered_list_has_first = decltype (first (std::declval<ordered_list<X>> ()));
        using priority_queue_has_first = decltype (first (std::declval<priority_queue<X>> ()));

        using stack_has_rest = decltype (rest (std::declval<stack<X>> ()));
        using list_has_rest = decltype (rest (std::declval<list<X>> ()));
        using ordered_list_has_rest = decltype (rest (std::declval<ordered_list<X>> ()));
        using priority_queue_has_rest = decltype (rest (std::declval<priority_queue<X>> ()));

        using stack_has_take = decltype (take (std::declval<stack<X>> (), 0));
        using list_has_take = decltype (take (std::declval<list<X>> (), 0));
        using ordered_list_has_take = decltype (take (std::declval<ordered_list<X>> (), 0));
        using priority_queue_has_take = decltype (take (std::declval<priority_queue<X>> (), 0));

        using stack_has_drop = decltype (drop (std::declval<stack<X>> (), 0));
        using list_has_drop = decltype (drop (std::declval<list<X>> (), 0));
        using ordered_list_has_drop = decltype (drop (std::declval<ordered_list<X>> (), 0));
        using priority_queue_has_drop = decltype (drop (std::declval<priority_queue<X>> (), 0));
        
        using stack_has_reverse = decltype (reverse (std::declval<stack<X>> ()));
        using list_has_reverse = decltype (reverse (std::declval<list<X>> ()));
        
        using stack_has_join = decltype (join (std::declval<stack<X>> (), std::declval<stack<X>> ()));
        using list_has_join = decltype (join (std::declval<list<X>> (), std::declval<list<X>> ()));
        
        using stack_has_plus = decltype (std::declval<stack<X>> () + std::declval<stack<X>> ());
        using list_has_plus = decltype (std::declval<list<X>> () + std::declval<list<X>> ());

        using ordered_list_has_merge = decltype (merge (std::declval<ordered_list<X>> (), std::declval<ordered_list<X>> ()));
        using priority_queue_has_merge = decltype (merge (std::declval<priority_queue<X>> (), std::declval<priority_queue<X>> ()));
        using set_has_merge = decltype (merge (std::declval<set<X>> (), std::declval<set<X>> ()));
        using map_has_merge = decltype (merge (std::declval<map<X, X>> (), std::declval<map<X, X>> ()));

        using ordered_list_has_and = decltype (std::declval<ordered_list<X>> () & std::declval<ordered_list<X>> ());
        using priority_queue_has_and = decltype (std::declval<priority_queue<X>> () & std::declval<priority_queue<X>> ());
        using set_has_and = decltype (std::declval<set<X>> () & std::declval<set<X>> ());
        using map_has_and = decltype (std::declval<map<X, X>> () & std::declval<map<X, X>> ());

        using ordered_list_has_or = decltype (std::declval<ordered_list<X>> () | std::declval<ordered_list<X>> ());
        using priority_queue_has_or = decltype (std::declval<priority_queue<X>> () | std::declval<priority_queue<X>> ());
        using set_has_or = decltype (std::declval<set<X>> () | std::declval<set<X>> ());
        using map_has_or = decltype (std::declval<map<X, X>> () | std::declval<map<X, X>> ());

        using ordered_list_has_xor = decltype (std::declval<ordered_list<X>> () ^ std::declval<ordered_list<X>> ());
        using priority_queue_has_xor = decltype (std::declval<priority_queue<X>> () ^ std::declval<priority_queue<X>> ());
        using set_has_xor = decltype (std::declval<set<X>> () ^ std::declval<set<X>> ());
        using map_has_xor = decltype (std::declval<map<X, X>> () ^ std::declval<map<X, X>> ());
  
        using stack_has_sort = decltype (sort (std::declval<stack<X>> ()));
        using list_has_sort = decltype (sort (std::declval<list<X>> ()));
        
        using stack_has_sorted = decltype (sorted (std::declval<stack<X>> ()));
        using list_has_sorted = decltype (sorted (std::declval<list<X>> ()));
        using ordered_list_has_sorted = decltype (sorted (std::declval<ordered_list<X>> ()));
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
        using map_has_replace = decltype (replace (std::declval<map<X, X>> (), {}));
    };

    template <typename X> bool doop (const X &) {
        return true;
    }

    template <typename X> struct test_has_contains {

        template <typename Y> 
        test_has_contains (Y y) {

            using stack_has_contains = decltype (contains (std::declval<stack<X>> (), y));
            using list_has_contains = decltype (contains (std::declval<list<X>> (), y));
            using tree_has_contains = decltype (contains (std::declval<tree<X>> (), y));
            using set_has_contains = decltype (contains (std::declval<set<X>> (), y));
            using ordered_list_has_contains = decltype (contains (std::declval<ordered_list<X>> (), y));
            using priority_queue_has_contains = decltype (contains (std::declval<priority_queue<X>> (), y));
            using map_has_contains = decltype (contains (std::declval<map<X, X>> (), y));

            //using set_has_insert = decltype (insert (std::declval<set<X>> (), y));
            //using ordered_list_has_insert = decltype (insert (std::declval<ordered_list<X>> (), y));
            //using priority_queue_has_insert = decltype (insert (std::declval<priority_queue<X>> (), y));
            //using map_has_insert = decltype (insert (std::declval<map<X, X>> (), y, y));

            //using stack_has_prepend = decltype (prepend (std::declval<stack<X>> (), y));
            //using list_has_prepend = decltype (prepend (std::declval<list<X>> (), y));
            //using ordered_list_has_prepend = decltype (prepend (std::declval<ordered_list<X>> (), y));

            //using list_has_append = decltype (append (std::declval<list<X>> (), y));
/*
            using list_has_select = decltype (select (std::declval<list<X>> (), &doop<y>));
            using set_has_select = decltype (select (std::declval<set<X>> (), &doop<y>));
            using ordered_list_has_select = decltype (select (std::declval<ordered_list<X>> (), &doop<y>));
            using priority_queue_has_select = decltype (select (std::declval<priority_queue<X>> (), &doop<y>));
            using map_has_select = decltype (select (std::declval<map<X, X>> (), &doop<y>));
*/
        }

    };

    TEST (FunctionalInterfaceTest, TestContainer) {

        test_has_functions<int> {};
        test_has_functions<const int> {};
        test_has_functions<int &> {};
        test_has_functions<const int &> {};
        test_has_functions<int *> {};
        test_has_functions<int *const> {};
        test_has_functions<int const *> {};
        test_has_functions<int const *const> {};

        test_has_contains<int> {0};
        test_has_contains<const int> {0};
        test_has_contains<int &> {0};
        test_has_contains<const int &> {0};
        test_has_contains<int *> {0};
        test_has_contains<int * const> {0};
        test_has_contains<int const *> {0};

        test_has_functions<string> {};
        test_has_functions<const string> {};
        test_has_functions<string &> {};
        test_has_functions<const string &> {};
        test_has_functions<string *> {};
        test_has_functions<string *const> {};
        test_has_functions<string const *> {};
        test_has_functions<string const *const> {};

        test_has_contains<string> {""};
        test_has_contains<const string> {""};
        test_has_contains<string &> {""};
        test_has_contains<const string &> {""};
        test_has_contains<string *> {""};
        test_has_contains<string * const> {""};
        test_has_contains<string const *> {""};

    }
}
