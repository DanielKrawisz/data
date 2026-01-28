// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/io/arg_parser.hpp"
#include "data/io/exception.hpp"
#include "gtest/gtest.h"

namespace data::io::args {

    parsed parse (const std::vector<const char*> &v) {
        return parsed {static_cast<int> (v.size ()), v.data ()};
    }

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

    // not a key value, but a flag and positional.
    TEST (Args, NotKeyValue) {
        auto parsed = parse ({"/prog", "--output", "file.txt"});

        maybe<std::string> file;
        parsed.get ("output", file);
        EXPECT_FALSE (file.has_value ());

        parsed.get (1, file);
        EXPECT_TRUE (file.has_value ());
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

    TEST (Args, FlagOptionPos) {
        auto parsed1 = parse ({"/prog", "123", "--zoob=noob", "--bervose"});
        auto parsed2 = parse ({"/prog", "--zoob=noob", "123", "--bervose"});
        auto parsed3 = parse ({"/prog", "--zoob=noob", "--bervose", "123"});

        maybe<int> v1;
        parsed1.get<int> (1, v1);
        EXPECT_TRUE (v1.has_value ());

        maybe<int> v2;
        parsed2.get<int> (1, v2);
        EXPECT_TRUE (v2.has_value ());

        maybe<int> v3;
        parsed3.get<int> (1, v3);
        EXPECT_TRUE (v3.has_value ());

        EXPECT_EQ (*v1, *v2);
        EXPECT_EQ (*v2, *v3);
    }

    // tests with abbeviated flags
    TEST (Args, ShortFlag1) {
        auto parsed = parse ({"/prog", "-abc"});

        EXPECT_TRUE (parsed.has ("a"));
        EXPECT_TRUE (parsed.has ("b"));
        EXPECT_FALSE (parsed.has ("d"));
    }

    TEST (Args, ShortFlag2) {
        auto parsed = parse ({"/prog", "-ab", "-cd"});

        EXPECT_TRUE (parsed.has ("a"));
        EXPECT_TRUE (parsed.has ("b"));
        EXPECT_TRUE (parsed.has ("d"));
    }

    // this is really a positional argument, not a key value.
    TEST (Args, ShortWithValue) {
        auto parsed = parse ({"/prog", "-o=file.txt"});

        maybe<std::string> what;

        parsed.get ("o", what);
        EXPECT_FALSE (what.has_value ());

        parsed.get (1, what);
        EXPECT_TRUE (what.has_value ());
    }

    TEST (Args, Repetitions) {
        EXPECT_THROW ((parse ({"/prog", "-ab", "--a"})), exception);
        EXPECT_THROW ((parse ({"/prog", "--option=value", "--option=value"})), exception);
    }

}
