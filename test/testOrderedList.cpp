// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools.hpp>
#include <data/string.hpp>
#include "gtest/gtest.h"

namespace data {

    TEST (OrderedListTest, TestOrderedSequenceInterfaces) {
        // We had some trouble defining SequenceOf properly and these are some 
        // tests that helped to make it work right. 
        static_assert (Same<decltype (std::declval<ordered_sequence<int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<int>> ().first ()), const int &>);
        static_assert (Same<decltype (std::declval<ordered_sequence<const int>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<ordered_sequence<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<int &>> ().first ()), int &>);
        static_assert (Same<decltype (std::declval<ordered_sequence<const int &>> ().first ()), const int &>);

        static_assert (Same<decltype (std::declval<ordered_sequence<int *>> ().first ()), int *const &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<int *>> ().first ()), int *const &>);
        static_assert (Same<decltype (std::declval<ordered_sequence<const int *>> ().first ()), const int *const &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<const int *>> ().first ()), const int *const &>);

        static_assert (Same<decltype (std::declval<ordered_sequence<string>> ().first ()), const string &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<string>> ().first ()), const string &>);
        static_assert (Same<decltype (std::declval<ordered_sequence<const string>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<ordered_sequence<string &>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<string &>> ().first ()), string &>);
        static_assert (Same<decltype (std::declval<ordered_sequence<const string &>> ().first ()), const string &>);

        static_assert (Same<decltype (std::declval<ordered_sequence<string *>> ().first ()), string *const &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<string *>> ().first ()), string *const &>);
        static_assert (Same<decltype (std::declval<ordered_sequence<const string *>> ().first ()), const string *const &>);
        static_assert (Same<decltype (std::declval<const ordered_sequence<const string *>> ().first ()), const string *const &>);
    
        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<int>> ().first ()), int>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<int &>> ().first ()), int>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<int *>> ().first ()), int *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<const int *>> ().first ()), const int *>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<string>> ().first ()), const string &>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<string &>> ().first ()), const string &>);

        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<string *>> ().first ()), string *>);
        static_assert (ImplicitlyConvertible<decltype (std::declval<const ordered_sequence<const string *>> ().first ()), const string *>);

        static_assert (SequenceOf<ordered_sequence<int>, int>);
        static_assert (SequenceOf<ordered_sequence<int *>, int *>);
        static_assert (SequenceOf<ordered_sequence<const int *>, const int *>);
        static_assert (SequenceOf<ordered_sequence<int *const>, int *const>);
        static_assert (SequenceOf<ordered_sequence<const int *const>, const int *const>);
        static_assert (SequenceOf<ordered_sequence<int &>, int &>);
        static_assert (SequenceOf<ordered_sequence<const int &>, const int &>);

        static_assert (Container<ordered_sequence<int>, int>);
        static_assert (Container<ordered_sequence<int *>, int *>);
        static_assert (Container<ordered_sequence<const int *>, const int *>);
        static_assert (Container<ordered_sequence<int *const>, int *const>);
        static_assert (Container<ordered_sequence<const int *const>, const int *const>);
        static_assert (Container<ordered_sequence<int &>, int &>);
        static_assert (Container<ordered_sequence<const int &>, const int &>);

        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<const int>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<const int *>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<const int &>> ().begin ())>);
        static_assert (std::forward_iterator<decltype (std::declval<const ordered_sequence<const int *const >> ().begin ())>);

    }

    TEST (OrderedListTest, TestOrderedList) {

        ordered_sequence<int> a {{1}};
        ordered_sequence<int> b {{1, 2}};
        ordered_sequence<int> c {{1, 2, 3}};
        ordered_sequence<int> ca {{3, 2, 1}};
        ordered_sequence<int> cb {{2, 3, 1}};
        ordered_sequence<int> cc {{1, 3, 2}};
        ordered_sequence<int> cd {{2, 1, 3}};
        ordered_sequence<int> ce {{3, 1, 2}};
        ordered_sequence<int> d {};

        EXPECT_EQ (size (a), 1);
        EXPECT_EQ (size (b), 2);
        EXPECT_EQ (size (c), 3);
        EXPECT_EQ (size (d), 0);

        EXPECT_NE (a, b);

        EXPECT_FALSE ((sequence_equal (a, b)));

        EXPECT_NE (a, d);
        EXPECT_NE (b, d);
        EXPECT_NE (c, d);

        EXPECT_EQ (c, ca);
        EXPECT_EQ (c, cb);
        EXPECT_EQ (c, cc);
        EXPECT_EQ (c, cd);
        EXPECT_EQ (c, ce);

        // we had a problem with this earlier.
        d = c;

        EXPECT_NE (a, d);
        EXPECT_NE (b, d);
        EXPECT_EQ (c, d);

        ordered_sequence<int> e {d};

        EXPECT_EQ (e, d);

        EXPECT_EQ (ordered_sequence<int> {}, stack<int> {});
    }

}

template <typename X> struct ordseq_test : ::testing::Test {
    using type = data::ordered_sequence<X>;
    using element = X;
};

using ordseq_test_cases = ::testing::Types<
    int, const int, int &, const int &, int *, int *const, const int *, const int *const, 
    data::string, const data::string, data::string &, const data::string &, data::string *, 
    data::string *const, const data::string *, const data::string *const>;

TYPED_TEST_SUITE (ordseq_test, ordseq_test_cases);

TYPED_TEST (ordseq_test, TestOrdSeqEmpty) {
    using type = typename TestFixture::type;
    auto is_empty = empty (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_empty), bool>);
    EXPECT_TRUE (is_empty);
}

TYPED_TEST (ordseq_test, TestOrdSeqSize) {
    using type = typename TestFixture::type;
    auto empty_size = size (type {});
    static_assert (data::ImplicitlyConvertible<decltype (empty_size), size_t>); 
    EXPECT_EQ (empty_size, 0);
}

TYPED_TEST (ordseq_test, TestOrdSeqFirst) {
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

TYPED_TEST (ordseq_test, TestOrdSeqRest) {
    using type = typename TestFixture::type;
    using return_type = decltype (rest (type {}));
    static_assert (data::ImplicitlyConvertible<return_type, const type>);
}

TYPED_TEST (ordseq_test, TestOrdSeqValues) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (values (type {}));
    static_assert (data::SequenceOf<return_type, element>);
}

TYPED_TEST (ordseq_test, TestOrdSeqReverse) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (reverse (type {}));
    static_assert (data::SequenceOf<return_type, element>);
}

TYPED_TEST (ordseq_test, TestOrdSeqContains) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_contains = decltype (contains (type {}, std::declval<element> ()));
}
/*
TYPED_TEST (ordseq_test, TestOrdSeqInsert) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_insert = decltype (insert (type {}, std::declval<element> ()));
    using has_rshift = decltype (type {} >> std::declval<element> ());
    type stack {};
    using has_rshift_equals = decltype (stack >>= std::declval<element> ());
}*/
/*
TYPED_TEST (ordseq_test, TestOrdSeqTakeDrop) {
    using type = typename TestFixture::type;
    using has_take = decltype (take (type {}, size_t (0)));
    using has_drop = decltype (drop (type {}, size_t (0)));
}*/

TYPED_TEST (ordseq_test, TestOrdSeqSort) {
    using type = typename TestFixture::type;
    //(void) sort (type {});
    EXPECT_TRUE (sorted (type {}));
}
/*
TYPED_TEST (ordseq_test, TestOrdSeqMerge) {
    using type = typename TestFixture::type;
    (void) merge (type {}, type {});
    (void) (type {} & type {});
}

TYPED_TEST (ordseq_test, TestOrdSeqSelect) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_select = decltype (contains (type {}, std::declval<element> ()));
}*/
