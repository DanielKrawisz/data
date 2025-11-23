// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iterator>
#include "data/container.hpp"
#include "data/string.hpp"
#include "data/numbers.hpp"
#include "gtest/gtest.h"

namespace data {

    TEST (LinkedStack, StackInterfaces) {

        static_assert (Sequence<stack<int>, int>);
        static_assert (Sequence<stack<int *>, int *>);
        static_assert (Sequence<stack<const int *>, const int *>);
        static_assert (Sequence<stack<int *const>, int *const>);
        static_assert (Sequence<stack<const int *const>, const int *const>);
        static_assert (Sequence<stack<int &>, int &>);
        static_assert (Sequence<stack<const int &>, const int &>);

        static_assert (Stack<stack<int>>);
        static_assert (Stack<stack<int *>>);
        static_assert (Stack<stack<const int *>>);
        static_assert (Stack<stack<int *const>>);
        static_assert (Stack<stack<const int *const>>);
        static_assert (Stack<stack<int &>>);
        static_assert (Stack<stack<const int &>>);

        static_assert (Container<stack<int>, int>);
        static_assert (Container<stack<int *>, int *>);
        static_assert (Container<stack<const int *>, const int *>);
        static_assert (Container<stack<int *const>, int *const>);
        static_assert (Container<stack<const int *const>, const int *const>);
        static_assert (Container<stack<int &>, int &>);
        static_assert (Container<stack<const int &>, const int &>);

        static_assert (ConstIterable<stack<int>>);
        static_assert (ConstIterable<stack<int *>>);
        static_assert (ConstIterable<stack<const int *>>);
        static_assert (ConstIterable<stack<int *const>>);
        static_assert (ConstIterable<stack<const int *const>>);
        static_assert (ConstIterable<stack<int &>>);
        static_assert (ConstIterable<stack<const int &>>);
/*
        static_assert (Iterable<stack<int>>);
        static_assert (Iterable<stack<int *>>);
        static_assert (Iterable<stack<const int *>>);
        static_assert (Iterable<stack<int *const>>);
        static_assert (Iterable<stack<const int *const>>);
        static_assert (Iterable<stack<int &>>);
        static_assert (Iterable<stack<const int &>>);*/

        // We had some trouble defining Sequence properly and these are some
        // tests that helped to make it work right. 
        static_assert (Same<decltype (std::declval<stack<int>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<const stack<int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<stack<const int>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<stack<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<const stack<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<stack<const int &>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<stack<int *>> ().first ()), int *&>);
        static_assert (Same<decltype (std::declval<const stack<int *>> ().first ()), int *const &>);
        static_assert (Same<decltype (std::declval<stack<const int *>> ().first ()), const int *&>);
        static_assert (Same<decltype (std::declval<const stack<const int *>> ().first ()), const int *const &>);

        static_assert (Same<decltype (std::declval<stack<string>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<const stack<string>> ().first ()), const string &>);
        static_assert (Same<decltype (std::declval<stack<const string>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<stack<string &>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<const stack<string &>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<stack<const string &>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<stack<string *>> ().first ()), string *&>);
        static_assert (Same<decltype (std::declval<const stack<string *>> ().first ()), string *const &>);
        static_assert (Same<decltype (std::declval<stack<const string *>> ().first ()), const string *&>);
        static_assert (Same<decltype (std::declval<const stack<const string *>> ().first ()), const string *const &>);
    
        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<int>> ().first ()), int>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<int &>> ().first ()), int>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<int *>> ().first ()), int *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<const int *>> ().first ()), const int *>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<string>> ().first ()), const string &>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<string &>> ().first ()), const string &>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<string *>> ().first ()), string *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const stack<const string *>> ().first ()), const string *>);

        static_assert (Same<decltype (std::declval<stack<int>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<stack<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<stack<int *>> ().first ()), int *&>);
        static_assert (Same<decltype (std::declval<stack<int *const>> ().first ()), int *const &>);
                
        static_assert (Same<decltype (std::declval<stack<const int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<stack<const int &>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<stack<const int *>> ().first ()), const int *&>);
        static_assert (Same<decltype (std::declval<stack<const int *const>> ().first ()), const int *const &>);

        static_assert (Same<decltype (std::declval<const stack<int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<const stack<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<const stack<int *>> ().first ()), int *const &>);
        static_assert (Same<decltype (std::declval<const stack<int *const>> ().first ()), int *const &>);
        
        static_assert (Same<decltype (std::declval<const stack<const int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<const stack<const int &>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<const stack<const int *>> ().first ()), const int *const &>);
        static_assert (Same<decltype (std::declval<const stack<const int *const>> ().first ()), const int *const &>);

        static_assert (std::forward_iterator<decltype (std::declval<const stack<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<const int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const stack<const int *const >> ().begin ())>);

    }

    TEST (LinkedStack, LinkedStack) {
        
        EXPECT_TRUE (stack<int> {} == stack<int> ());
        
        EXPECT_TRUE (stack<int> (1) == stack<int> (1));
        EXPECT_FALSE (stack<int> (1) == stack<int> {});
        EXPECT_FALSE (stack<int> (1) == stack<int> (0));
        
        EXPECT_TRUE (stack<int> (1).first () == 1);
        EXPECT_TRUE (stack<int> (1).rest () == stack<int> {});
        EXPECT_TRUE (stack<int> (1).size () == 1);

        EXPECT_EQ (prepend (stack<int> {}, 1), stack<int> {1});
        EXPECT_EQ (stack<int> {} >> 1, stack<int> {1});

        EXPECT_EQ (prepend (stack<int> {0}, 1), (stack<int> {1, 0}));
        EXPECT_EQ (stack<int> {0} >> 1, (stack<int> {1, 0}));
        
    }

    // c++ containers aren't allowed to contain references. 
    // Functional data structures don't have to be c++ 
    // containers so we also have a version of functional
    // linked list for references. 
    TEST (LinkedStack, LinkedStackR) {
        
        int One = 1;
        int Zero = 0;
        
        auto empty = stack<int &> {};
        auto zero = stack<int &> {Zero, {}};
        auto one = stack<int &> {One, {}};
        auto one_zero = stack<int &> {One, zero};
        
        EXPECT_TRUE (empty == empty);
        EXPECT_TRUE (one == one);
        EXPECT_FALSE (one == empty);
        EXPECT_FALSE (one == zero);
        EXPECT_TRUE (one.first () == One);
        
        EXPECT_TRUE (stack<int &> (One).first () == One);
        EXPECT_TRUE (one.rest () == empty);
        EXPECT_TRUE (one_zero.rest () == zero);
        EXPECT_TRUE (stack<int &> (One).size () == 1);
        
    }

    TEST (LinkedStack, LinkedStackConstruct) {
    
        stack<int> l {1, 2, 3};
        stack<int> r {};
        stack<int> a {1};
        stack<int> b {1, 2};
        EXPECT_EQ (size (l), 3);
        EXPECT_EQ (size (r), 0);
        EXPECT_EQ (size (a), 1);
        EXPECT_EQ (size (b), 2);
        EXPECT_TRUE (l != r);
        EXPECT_TRUE (l == r >> 3 >> 2 >> 1);

        stack<string> {"a", "b", "c"};
        stack<string> {};
        stack<string> {"1"};
        stack<string> {"1", "2"};

        int iv[] {1, 2, 3, 4, 5, 6, 7, 8, 9};
        string zv [] {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

        stack<int> {1, 2, 3, 4, 5, 6, 7};
        stack<int *> {iv, iv + 2, iv + 4, iv + 3, iv + 8, iv + 1};
        stack<int &> {iv[0], iv[1], iv[3], iv[7], iv[4], iv[5], iv[0]};

        stack<string> {"1", "2", "3", "4", "5", "6", "7"};
        stack<string *> {zv, zv + 2, zv + 4, zv + 3, zv + 8, zv + 1};
        stack<string &> {zv[0], zv[1], zv[3], zv[7], zv[4], zv[5], zv[0]};

        stack<stack<int>> {{1, 2, 3}, {4, 5}, {6}};
        stack<stack<string>> {{"1", "2", "3"}, {"4", "5"}, {"6"}};

    }
    
    TEST (LinkedStack, LinkedStackEqual) {
        
        stack<int> t1 {2, 1, 3, 5, 1, 7};
        stack<int> t2 {2, 1, 3, 5, 1, 7};
        stack<int> t3 {5, 2, 3, 5, 8, 3};
        stack<int> t4 {5, 2, 3, 5};
        
        EXPECT_TRUE (t1 == t1);
        EXPECT_TRUE (t1 == t2);
        EXPECT_TRUE (t1 != t3);
        EXPECT_TRUE (t3 != t4);
        EXPECT_TRUE (t4 != t1);
    }
    
    void test_copy_stack (stack<int> &p, int max) {
        p = stack<int> {};
        stack<int> new_stack {};
        for(int i = 1; i <= max; i++) new_stack = new_stack >> i;
        p = new_stack;
    }
    
    // There was a bug in which linked lists would be destroyed
    // if they were copied. This test ensures that this does not happen.
    TEST (LinkedStack, LinkedStackCopy) {
        stack<int> p;
        test_copy_stack (p, 7);
        EXPECT_EQ (p.size (), 7);
        EXPECT_EQ (p.first (), 7);
        EXPECT_EQ (p.rest ().size (), 6);
        EXPECT_EQ (p.rest ().first (), 6);
    }
    
    TEST (LinkedStack, LinkedStackIteration) {
        
        stack<int> t {1, 2, 3};
        auto i = t.begin ();
        EXPECT_NE (i, t.end ());
        EXPECT_EQ (*i, 1);
        i++;
        EXPECT_NE (i, t.end ());
        EXPECT_EQ (*i, 2);
        i++;
        EXPECT_NE (i, t.end ());
        EXPECT_EQ (*i, 3);
        i++;
        EXPECT_EQ (i, t.end ());
        
        for (int x : t) (void) x;
        for (const int &x : t) (void) x;
    }

    void accept_stack_of_string_views (stack<string_view>) {}

    TEST (LinkedStack, LinkedStackConvert) {
        stack<string> test {"1", "2", "3", "4"};

        accept_stack_of_string_views (test);

        stack<N> numbers {1u, 2u, 3u, 4u};

        EXPECT_EQ (stack<N> (test), numbers);

    }

    TEST (LinkedStack, LinkedStackNumbers) {
        stack<Z_bytes_little> {};
        stack<Z_bytes_little> ();
        stack<Z_bytes_little> {1};
        stack<Z_bytes_little> (1);
        stack<Z_bytes_little> {1, 2};
    }
}

template <typename X> struct Stack : ::testing::Test {
    using type = data::stack<X>;
    using element = X;
};

using Stack_cases = ::testing::Types<
    int, const int, int &, const int &, int *, int *const, const int *, const int *const, 
    data::string, const data::string, data::string &, const data::string &, data::string *, 
    data::string *const, const data::string *, const data::string *const>;

TYPED_TEST_SUITE (Stack, Stack_cases);

TYPED_TEST (Stack, StackValid) {
    using type = typename TestFixture::type;
    auto is_valid = valid (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_valid), bool>);
    EXPECT_TRUE (is_valid);
}

TYPED_TEST (Stack, StackEmpty) {
    using type = typename TestFixture::type;
    auto is_empty = empty (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_empty), bool>);
    EXPECT_TRUE (is_empty);
}

TYPED_TEST (Stack, StackSize) {
    using type = typename TestFixture::type;
    auto empty_size = size (type {});
    static_assert (data::ImplicitlyConvertible<decltype (empty_size), size_t>); 
    EXPECT_EQ (empty_size, 0);
}

TYPED_TEST (Stack, StackFirst) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;

    type z {};
    const type cz {};

    EXPECT_THROW (first (z), data::empty_sequence_exception);
    EXPECT_THROW (first (cz), data::empty_sequence_exception);
    
    using return_type = decltype (first (z));
    using const_return_type = decltype (first (cz));

    EXPECT_THROW (z[size_t {0}], data::empty_sequence_exception);
    EXPECT_THROW (cz[size_t {0}], data::empty_sequence_exception);

    static_assert (data::Same<decltype (z[size_t {0}]), return_type>);
    static_assert (data::Same<decltype (cz[size_t {0}]), const_return_type>);
    
    static_assert (data::ImplicitlyConvertible<return_type, element>);
    static_assert (data::ImplicitlyConvertible<const_return_type, const element>);

    static_assert (data::Reference<return_type>);
    static_assert (data::Reference<const_return_type>);

    if constexpr (data::Reference<element>) {
        static_assert (data::Same<element, return_type>);
        static_assert (data::Same<return_type, const_return_type>);
    } else if constexpr (data::Const<element>) {
        static_assert (data::Same<element &, return_type>);
        static_assert (data::Same<return_type, const_return_type>);
    } else {
        static_assert (data::Same<element &, return_type>);
        static_assert (data::Same<const element &, const_return_type>);
    }
}

TYPED_TEST (Stack, StackRest) {
    using type = typename TestFixture::type;
    using return_type = decltype (rest (type {}));
    static_assert (data::ImplicitlyConvertible<return_type, const type>);
}

TYPED_TEST (Stack, StackValues) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (values (type {}));
    static_assert (data::Sequence<return_type, element>);
}

TYPED_TEST (Stack, StackReverse) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (reverse (type {}));
    static_assert (data::Sequence<return_type, element>);
}

TYPED_TEST (Stack, StackContains) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    static_assert (data::ImplicitlyConvertible<decltype (contains (type {}, std::declval<element> ())), bool>);
}

TYPED_TEST (Stack, StackPrepend) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (prepend (type {}, std::declval<element> ()));
    using has_rshift = decltype (type {} >> std::declval<element> ());
    type stack {};
    using has_rshift_equals = decltype (stack >>= std::declval<element> ());
    static_assert (data::Same<return_type, type>);
}

TYPED_TEST (Stack, StackTakeDrop) {
    using type = typename TestFixture::type;
    using has_take = decltype (take (type {}, size_t (0)));
    using has_drop = decltype (drop (type {}, size_t (0)));
}

TYPED_TEST (Stack, StackJoin) {
    using type = typename TestFixture::type;
    (void) join (type {}, type {});
    (void) (type {} + type {});
}

TYPED_TEST (Stack, StackSort) {
    using type = typename TestFixture::type;
    (void) sort (type {});
    EXPECT_TRUE (sorted (type {}));
}

TYPED_TEST (Stack, StackRemove) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_select = decltype (remove (type {}, size_t {0}));
}

TYPED_TEST (Stack, StackErase) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_erase = decltype (erase (type {}, std::declval<element> ()));
}
// TODO
/*
TYPED_TEST (Stack, TestStackSelect) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_select = decltype (select (type {}, [] (const auto &&) {
        return true;
    }));
}*/

        /*
            fold
            reduce
            for_each
            map_thread
            transpose
        */
