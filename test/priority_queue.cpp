// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/string.hpp"
#include "data/tools.hpp"
#include "gtest/gtest.h"

TEST (PriorityQueue, Interface) {
    // should be const iterable.
}

TEST (PriorityQueue, PriorityQueue) {
    using namespace data;
    
    using pq = priority_queue<int>;
    
    EXPECT_EQ (pq {}, pq {});
    EXPECT_EQ (first (pq {} << 1), 1);
    EXPECT_NE (pq {} << 1, pq {});
    EXPECT_EQ (pq {} << 1 << 2, pq {} << 2 << 1);
    EXPECT_EQ (1, first (pq {} << 2 << 1));
}

template <typename X> struct PriQue : ::testing::Test {
    using type = data::priority_queue<X>;
    using element = X;
};

using PriQue_cases = ::testing::Types<
int, const int, int &, const int &, int *, int *const, const int *, const int *const,
data::string, const data::string, data::string &, const data::string &, data::string *,
data::string *const, const data::string *, const data::string *const>;

TYPED_TEST_SUITE (PriQue, PriQue_cases);

TYPED_TEST (PriQue, Valid) {
    using type = typename TestFixture::type;
    auto is_valid = valid (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_valid), bool>);
    EXPECT_TRUE (is_valid);
}

TYPED_TEST (PriQue, Empty) {
    using type = typename TestFixture::type;
    auto is_empty = empty (type {});
    static_assert (data::ImplicitlyConvertible<decltype (is_empty), bool>);
    EXPECT_TRUE (is_empty);
}

TYPED_TEST (PriQue, Size) {
    using type = typename TestFixture::type;
    auto empty_size = size (type {});
    static_assert (data::ImplicitlyConvertible<decltype (empty_size), size_t>);
    EXPECT_EQ (empty_size, 0);
}

TYPED_TEST (PriQue, First) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    EXPECT_THROW (first (type {}), data::empty_sequence_exception);

    using return_type = decltype (first (type {}));

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

TYPED_TEST (PriQue, Rest) {
    using type = typename TestFixture::type;
    using return_type = decltype (rest (type {}));
    static_assert (data::ImplicitlyConvertible<return_type, const type>);
}

TYPED_TEST (PriQue, Values) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using return_type = decltype (values (type {}));
    static_assert (data::Sequence<return_type, element>);
}

TYPED_TEST (PriQue, Contains) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_contains = decltype (contains (type {}, std::declval<element> ()));
}
// TODO for beta
/*
TYPED_TEST (PriQue, Insert) {
    using type = typename TestFixture::type;
    using element = typename TestFixture::element;
    using has_insert = decltype (insert (type {}, std::declval<element> ()));
    using has_rshift = decltype (type {} >> std::declval<element> ());
    type stack {};
    using has_rshift_equals = decltype (stack >>= std::declval<element> ());
}*/

