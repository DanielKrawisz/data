//
// Created by nekosune on 05/07/19.
//
#include "data/encoding/ascii.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"
namespace {

    TEST(AsciiTests,AsciiInvalidAscii) {
        char temp[]="This is a test";
        temp[3]=-20;
        std::string temp2=temp;
        data::encoding::ascii::string stringTest(&temp2);
        ASSERT_FALSE(stringTest.isValid());
    }

    TEST(AsciiTests,AsciiValidAscii) {
        char temp[]="This is a test";
        std::string temp2=temp;
        data::encoding::ascii::string stringTest(&temp2);
        ASSERT_TRUE(stringTest.isValid());
    }


    TEST(AsciiTests,AsciiCastToBytes) {
        char temp[]="This is a test";
        std::string temp2=temp;
        data::encoding::ascii::string stringTest(&temp2);
        ASSERT_THAT(static_cast<data::bytes>(stringTest),::testing::ElementsAre(84,104,105,115,32,105,115,32,97,32,116,101,115,116));
    }

}
