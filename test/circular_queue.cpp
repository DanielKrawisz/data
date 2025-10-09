// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/tools/circular_queue.h>
#include "gtest/gtest.h"
namespace data::tools {

    TEST (CircularQueue, Initialize) {
        circular_queue<int> queue (5);
        queue.set (5);
        auto n = queue.get ();
        ASSERT_EQ (n, 0) << "Expected to retrieve 5 from the queue, but got " << n;
        queue.set (4);
        queue.set (6);
        queue.set (8);
        queue.set (10);
        auto m = queue.get ();
        ASSERT_EQ (m, 5) << "Expected to retrieve 10 from the queue, but got " << m;
    }
}
