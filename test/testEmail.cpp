
#include <data/net/email.hpp>
#include "gtest/gtest.h"

namespace data::net::email {

    TEST (EmailTest, TestAddress) {
        for (const ASCII &addr : list<ASCII> {
            "example@email.com",
            "example.first.middle.lastname@email.com",
            "example@subdomain.email.com",
            "example+firstname+lastname@email.com",
            "example@234.234.234.234",
            "example@[234.234.234.234]",
            R"("example"@email.com)",
            "0987654321@example.com",
            "example@email-one.com",
            "_______@email.com",
            "example@email.name",
            "example@email.museum",
            "example@email.co.jp",
            "example.firstname-lastname@email.com",
            "jon.o'conner@example.com",
            R"("bob" <bob@zoob.com>)",
            R"(Set Na <sed@na.co>)",
            "Joomba loomba <jl @ (hi, this is a comment) na.co>"
        }) EXPECT_TRUE (address::valid (addr));

        for (const ASCII &addr : list<ASCII> {
            R"(“example”@email.com)",
            R"(very."(),:;<>[]".VERY."very@\\ "very".unusual@strange.email.example.com)"}) EXPECT_FALSE (address::valid (addr));
    }
}
