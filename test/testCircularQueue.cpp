// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools/circular_queue.h>
#include "gtest/gtest.h"
#include <iostream>
namespace data {
    namespace tools {
        TEST(CircularQueueTest, TestInitialize) {
            circular_queue queue(5);
            queue.setValue(5);
            ASSERT_EQ(queue.getValue(),5);
            queue.next();
            queue.setValue(4);
            queue.next();
            queue.setValue(6);
            queue.next();
            queue.setValue(8);
            queue.next();
            queue.setValue(10);
            queue.next();
            ASSERT_EQ(queue.getValue(),5);
        }
    }
}