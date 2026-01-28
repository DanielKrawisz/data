// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/io/arg_parser.hpp"
#include "gtest/gtest.h"

namespace data::io {

    arg_parser parse (const std::vector<const char*> &v) {
        return arg_parser {static_cast<int> (v.size ()), v.data ()};
    }

    // we are working on a new version of the arg parser.
    // the following are tests that must work in the new version.

    TEST (Args, LongFlag1) {
        auto parsed = parse ({"/prog", "--verbose"});

        EXPECT_TRUE (parsed.has ("verbose"));
        EXPECT_FALSE (parsed.has ("help"));
    }

    TEST (Args, LongFlag2) {
        auto parsed = parse ({"/prog", "--verbose", "--help"});

        EXPECT_TRUE (parsed.has ("verbose"));
        EXPECT_TRUE (parsed.has ("help"));
    }

    TEST (Args, LongOptionEquals) {
        auto parsed = parse ({"/prog", "--output=file.txt"});

        maybe<std::string> file;
        parsed.get ("output", file);
        EXPECT_TRUE (file.has_value ());
        EXPECT_EQ (*file, "file.txt");
    }

    TEST (Args, GetByKeySuccessInt) {
        auto parsed = parse ({"/prog", "--count=42"});

        maybe<int> v;
        parsed.get<int> ("count", v);
        EXPECT_TRUE (v.has_value ());
        EXPECT_EQ (*v, 42);
    }

    TEST (Args, GetByKeyMissing) {
        auto parsed = parse ({"/prog", "--verbose"});

        maybe<int> v;
        parsed.get<int> ("count", v);
        EXPECT_FALSE (v.has_value ());

        maybe<std::string> file;
        parsed.get ("verbose", file);
        EXPECT_FALSE (file.has_value ());
    }

    TEST (Args, GetByKeyInvalidType) {
        auto parsed = parse ({"/prog", "--count=abc"});

        maybe<int> v;
        parsed.get<int> ("count", v);
        EXPECT_FALSE (v.has_value ());
    }

    TEST (Args, GetByKeyBoolFlag) {
        auto parsed = parse ({"/prog", "--verbose=true"});

        maybe<bool> v;
        parsed.get<bool> ("verbose", v);
        EXPECT_TRUE (v.has_value ());
        EXPECT_TRUE (*v);
    }

    TEST (Args, GetByPositionSuccess) {
        auto parsed = parse ({"/prog", "123"});

        maybe<int> v;
        parsed.get<int> (1, v);
        EXPECT_TRUE (v.has_value ());
        EXPECT_EQ (*v, 123);
    }

    TEST (Args, GetByPositionOutOfRange) {
        auto parsed = parse ({"/prog", "123"});

        maybe<int> v;
        parsed.get<int> (2, v);
        EXPECT_FALSE (v.has_value ());
    }

    TEST (Args, GetByPositionInvalidType) {
        auto parsed = parse ({"/prog", "abc"});

        maybe<int> v;
        parsed.get<int> (1, v);
        EXPECT_FALSE (v.has_value ());
    }

    TEST (Args, PositionalPreferredOverKey) {
        auto parsed = parse ({"/prog", "7", "--count=42"});

        maybe<int> v;
        parsed.get<int> (1, "count", v);
        EXPECT_TRUE (v.has_value ());
        EXPECT_EQ (*v, 7);
    }

    TEST (Args, KeyUsedIfPositionMissing) {
        auto parsed = parse ({"/prog", "--count=42"});

        maybe<int> v;
        parsed.get<int> (1, "count", v);
        EXPECT_TRUE (v.has_value ());
        EXPECT_EQ (*v, 42);
    }

    TEST (Args, InvalidPositionalBlocksKeyFallback) {
        auto parsed = parse ({"/prog", "abc", "--count=42"});

        maybe<int> v;
        parsed.get<int> (1, "count", v);
        EXPECT_FALSE (v.has_value ());
    }

}
