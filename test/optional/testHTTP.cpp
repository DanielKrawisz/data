// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/net/REST.hpp"
#include "gtest/gtest.h"

namespace data {

    const std::string baseurl="echo.jsontest.com";

    TEST(HTTPTest, TestGetHttps) {
        auto rest=data::net::HTTP::REST("https",baseurl);
        auto request=rest.GET("key/1/key2/poppy");
        
        
    }
}

