// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/list.hpp>
#include <data/container.hpp>
#include <data/string.hpp>
#include <data/numbers.hpp>
#include <data/shuffle.hpp>
#include "gtest/gtest.h"

namespace data {

    TEST (List, Interfaces) {

        static_assert (Container<list<int>, int>);
        static_assert (Container<list<const int>, const int>);
        static_assert (Container<list<int *>, int *>);
        static_assert (Container<list<const int *>, const int *>);
        static_assert (Container<list<int *const>, int *const>);
        static_assert (Container<list<const int *const>, const int *const>);
        static_assert (Container<list<int &>, int &>);
        static_assert (Container<list<const int &>, const int &>);

        static_assert (List<list<int>>);
        static_assert (List<list<const int>>);
        static_assert (List<list<int *>>);
        static_assert (List<list<const int *>>);
        static_assert (List<list<int *const>>);
        static_assert (List<list<const int *const>>);
        static_assert (List<list<int &>>);
        static_assert (List<list<const int &>>);

        static_assert (ConstIterable<list<int>>);
        static_assert (ConstIterable<list<int *>>);
        static_assert (ConstIterable<list<const int *>>);
        static_assert (ConstIterable<list<int *const>>);
        static_assert (ConstIterable<list<const int *const>>);
        static_assert (ConstIterable<list<int &>>);
        static_assert (ConstIterable<list<const int &>>);

        static_assert (Same<decltype (std::declval<const list<int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<const list<const int>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<const list<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<const list<const int &>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<const list<int *>> ().first ()), int *const &>);
        static_assert (Same<decltype (std::declval<const list<const int *>> ().first ()), const int *const &>);

        static_assert (Same<decltype (std::declval<const list<string>> ().first ()), const string &>);
        static_assert (Same<decltype (std::declval<const list<const string>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<const list<string &>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<const list<const string &>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<const list<string *>> ().first ()), string *const &>);
        static_assert (Same<decltype (std::declval<const list<const string *>> ().first ()), const string *const &>);
    
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<int>> ().first ()), int>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<int &>> ().first ()), int>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<int *>> ().first ()), int *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<const int *>> ().first ()), const int *>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<string>> ().first ()), const string &>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<string &>> ().first ()), const string &>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<string *>> ().first ()), string *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const list<const string *>> ().first ()), const string *>);

        static_assert (std::forward_iterator<decltype (std::declval<const list<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const list<const int *const >> ().begin ())>);

    }

    TEST (List, Construct) {

        list<int> l {1, 2, 3};
        list<int> r {};
        list<int> a {1};
        list<int> b {1, 2};

        EXPECT_EQ (l.size (), 3);
        EXPECT_EQ (r.size (), 0);
        EXPECT_EQ (a.size (), 1);
        EXPECT_EQ (b.size (), 2);
        EXPECT_TRUE (l != r);
        EXPECT_TRUE (l == r << 1 << 2 << 3);

        int val[] {1, 2, 3, 4, 5, 6, 7, 8, 9};

        list<int> {1, 2, 3, 4, 5, 6, 7};
        list<int *> {val, val + 2, val + 4, val + 3, val + 8, val + 1};
        list<int &> {val[0], val[1], val[3], val[7], val[4], val[5], val[0]};

        const char *zv[] {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

        list<string> {"1", "2", "3", "4", "5", "6", "7"};
        list<const char **> {zv, zv + 2, zv + 4, zv + 3, zv + 8, zv + 1};
        list<const char *&> {zv[0], zv[1], zv[3], zv[7], zv[4], zv[5], zv[0]};

        list<list<int>> {{1, 2, 3}, {4, 5}, {6}};
        list<list<string>> {{"1", "2", "3"}, {"4", "5"}, {"6"}};

    }

    TEST (List, Pend) {

        EXPECT_EQ (prepend (list<int> {}, 1), list<int> {1});
        EXPECT_EQ (list<int> {} >> 1, list<int> {1});

        EXPECT_EQ (prepend (list<int> {0}, 1), (list<int> {1, 0}));
        EXPECT_EQ (list<int> {0} >> 1, (list<int> {1, 0}));

        EXPECT_EQ (append (list<int> {}, 1), list<int> {1});
        EXPECT_EQ (list<int> {} << 1, list<int> {1});

        EXPECT_EQ (append (list<int> {0}, 1), (list<int> {0, 1}));
        EXPECT_EQ (list<int> {0} << 1, (list<int> {0, 1}));
    }

    void accept_stack_of_string_views (list<string_view>) {}

    TEST (List, Convert) {
        list<string> test {{"1", "2", "3", "4"}};

        accept_stack_of_string_views (test);

        list<N> numbers {1, 2, 3, 4};

        EXPECT_EQ (list<N> (test), numbers);

    }

    TEST (List, Rotate) {

        list<string> test {"1", "2", "3", "4"};
        list<string> rl {"2", "3", "4", "1"};
        list<string> rr {"4", "1", "2", "3"};

        EXPECT_EQ (rotate_right (test), rr);
        EXPECT_EQ (rotate_left (test), rl);

    }

    TEST (List, Ordering) {

        list<int> l0 {};
        list<int> l1 {1};
        list<int> l2 {1, 2};
        list<int> l3 {1, 2, 3};

        EXPECT_EQ (shuffle (l0, {}), l0);
        EXPECT_EQ (shuffle (l1, {0}), l1);
        EXPECT_EQ (shuffle (l2, {0, 1}), l2);
        EXPECT_EQ (shuffle (l2, {1, 0}), reverse (l2));
        EXPECT_EQ (shuffle (l3, {0, 1, 2}), l3);
        EXPECT_EQ (shuffle (l3, {2, 1, 0}), reverse (l3));
        EXPECT_EQ (shuffle (l3, {0, 2, 1}), (list<int> {1, 3, 2}));
        EXPECT_EQ (shuffle (l3, {1, 0, 2}), (list<int> {2, 1, 3}));
        EXPECT_EQ (shuffle (l3, {1, 2, 0}), (list<int> {3, 1, 2}));
        EXPECT_EQ (shuffle (l3, {2, 0, 1}), (list<int> {2, 3, 1}));

    }

}

template <typename X> struct List : ::testing::Test {
    using type = data::list<X>;
    using element = X;
};

using List_cases = ::testing::Types<
    int, const int, int &, const int &, int *, int *const, const int *, const int *const, 
    data::string, const data::string, data::string &, const data::string &, data::string *, 
    data::string *const, const data::string *, const data::string *const>;

TYPED_TEST_SUITE (List, List_cases);

TYPED_TEST (List, Valid) {
    using type = typename TestFixture::type;
    auto is_valid = valid (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_valid), bool>);
    EXPECT_TRUE (is_valid);
}

TYPED_TEST (List, Empty) {
    using type = typename TestFixture::type;
    auto is_empty = empty (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_empty), bool>);
    EXPECT_TRUE (is_empty);
}

TYPED_TEST (List, ListSize) {
    using type = typename TestFixture::type;
    auto empty_size = size (type {});
    static_assert (data::ImplicitlyConvertible<decltype (empty_size), size_t>); 
    EXPECT_EQ (empty_size, 0);
}

TYPED_TEST (List, First) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    EXPECT_THROW (first (type {}), data::empty_sequence_exception);

    using return_type = decltype (first (type {}));

    EXPECT_THROW (type {}[size_t {0}], data::empty_sequence_exception);

    static_assert (data::Same<decltype (type {}[size_t {0}]), return_type>);
    
    static_assert (data::ImplicitlyConvertible<return_type, const element>);
    static_assert (data::Reference<return_type>);

    if constexpr (data::Reference<element>) {
        static_assert (data::Same<element, return_type>);
    } else if constexpr (data::Const<element>) {
        static_assert (data::Same<element &, return_type>);
    } else {
        static_assert (data::Same<const element &, return_type>);
    }
}

TYPED_TEST (List, Rest) {
    using type = typename TestFixture::type;
    using return_type = decltype (rest (type {}));
    static_assert (data::ImplicitlyConvertible<return_type, const type>);
}

TYPED_TEST (List, Values) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (values (type {}));
    static_assert (data::Sequence<return_type, element>);
}

TYPED_TEST (List, Reverse) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (reverse (type {}));
    static_assert (data::Sequence<return_type, element>);
}

TYPED_TEST (List, Contains) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    static_assert (data::ImplicitlyConvertible<decltype (contains (type {}, std::declval<element> ())), bool>);
}

TYPED_TEST (List, Prepend) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_prepend = decltype (prepend (type {}, std::declval<element> ()));
    using has_rshift = decltype (type {} >> std::declval<element> ());
    type list {};
    using has_rshift_equals = decltype (list >>= std::declval<element> ());
}

TYPED_TEST (List, Appepend) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_append = decltype (append (type {}, std::declval<element> ()));
    using has_lshift = decltype (type {} << std::declval<element> ());
    type list {};
    using has_lshift_equals = decltype (list <<= std::declval<element> ());
}

TYPED_TEST (List, TakeDrop) {
    using type = typename TestFixture::type;
    using has_take = decltype (take (type {}, size_t (0)));
    using has_drop = decltype (drop (type {}, size_t (0)));
}

TYPED_TEST (List, Join) {
    using type = typename TestFixture::type;
    (void) join (type {}, type {});
    (void) (type {} + type {});
}

TYPED_TEST (List, Sort) {
    using type = typename TestFixture::type;
    (void) sort (type {});
    EXPECT_TRUE (sorted (type {}));
}

TYPED_TEST (List, ListRemove) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_select = decltype (remove (type {}, size_t {0}));
}

TYPED_TEST (List, ListErase) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_select = decltype (erase (type {}, std::declval<element> ()));
}
