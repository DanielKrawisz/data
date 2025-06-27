// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/encoding/ascii.hpp"
#include "data/encoding/unicode.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace data {

    TEST (AsciiTests, AsciiConstruct) {
        ASCII {"this should compile"};
        string a_string {"a string"};
        ASCII this_should_compile (a_string);
    }

    TEST (AsciiTests, AsciiInvalidAscii) {
        char temp[] = "This is a test";
        temp[3] = -20;
        string temp2 = temp;
        ASCII stringTest (temp2);
        ASSERT_FALSE (stringTest.valid ());
    }

    TEST (AsciiTests, AsciiValidAscii) {
        char temp[] = "This is a test";
        std::string temp2=temp;
        ASCII stringTest (temp2);
        ASSERT_TRUE (stringTest.valid ());
    }

    TEST (AsciiTests, AsciiCastToBytes) {
        char temp[] = "This is a test";
        std::string temp2 = temp;
        ASCII stringTest (temp2);
        ASSERT_THAT (static_cast<data::bytes> (stringTest), ::testing::ElementsAre (84,104,105,115,32,105,115,32,97,32,116,101,115,116));
    }

    TEST (AsciiTests, UnicodeToUTF8) {
        UTF8 expected {"Hello, World! Привет, мир!"};
        unicode test {U"Hello, World! Привет, мир!"};

        EXPECT_EQ (expected, encoding::UTF8::encode (test));
    }

}
