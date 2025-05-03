// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/async.hpp>
#include <boost/asio/steady_timer.hpp>

namespace data {

    // wait for a given amount of time.
    awaitable<void> sleep (std::chrono::milliseconds duration) {
        // Get the current executor associated with the coroutine
        auto executor = co_await boost::asio::this_coro::executor;

        // Create a timer using the same executor
        boost::asio::steady_timer timer (executor);

        // Set the timer to expire after the given duration
        timer.expires_after (duration);

        // Suspend the coroutine until the timer expires
        co_await timer.async_wait (boost::asio::use_awaitable);
    }

}
