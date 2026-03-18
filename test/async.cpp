// Copyright (c) 2026 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/async.hpp"
#include "gtest/gtest.h"

namespace data {

    awaitable<void> async_return_void () {
        co_return;
    }

    awaitable<int> async_return_int (int i) {
        co_return i;
    }

    // we had a problem earlier with synced not
    // working on types that are not default
    // initializable, so we have a test for it.
    struct poop {
        poop (int) {}
    };

    awaitable<poop> async_return_poop (int i) {
        co_return poop {i};
    }

    TEST (Async, Construct) {
        synced (async_return_void);

        synced ([] { return async_return_int (1); });

        synced (async_return_int, 1);
        synced (async_return_poop, 1);
    }

}

