// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/net/REST.hpp"
#include "data/net/HTTP_client.hpp"
#include "gtest/gtest.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace data {

    const std::string baseHttpUrl = "echo.jsontest.com";

    TEST (HTTPTest, TestGetHttp) {
        auto rest = data::net::HTTP::REST ("http", baseHttpUrl);
        auto client = data::net::HTTP::client_blocking (rest);
        auto request = rest.GET ("/key/pup/test/poppy");
        auto response = client (request);
        json ex1 = json::parse (response.Body);
        EXPECT_EQ (ex1["test"], "poppy") << "JSON mangled";
        EXPECT_EQ (ex1["key"], "pup") << "JSON mangled";
    }
}

