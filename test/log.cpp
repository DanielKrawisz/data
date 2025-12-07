// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/io/log.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "gmock/gmock-matchers.h"

namespace data::log {

    TEST (Log, Log) {
        init ();
        DATA_LOG_CHANNEL ("temp", normal) << "Hello, world!";
        indent x {};
        DATA_LOG_CHANNEL ("temp", normal) << "This line ought to be indented. ";
    }
}
