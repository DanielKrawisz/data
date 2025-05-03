// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/HTTP_client.hpp>

namespace data::net::HTTP {

    awaitable<response> client::operator () (const request &r) {
        std::chrono::milliseconds wait = Rate.get_time ();
        if (wait != std::chrono::milliseconds {0}) co_await sleep (wait);
        co_return co_await HTTP::call (r, SSL.get ());
    }

    void client::operator () (exec ex, asio::error_handler err, handler<const response &> hr, request r) {
        std::chrono::milliseconds wait = Rate.get_time ();
        if (wait == std::chrono::milliseconds {0}) HTTP::call (ex, err, hr, r, SSL.get ());
        auto timer = std::make_shared<boost::asio::steady_timer> (ex, wait);
        timer->async_wait ([timer, ex, err, hr, r, ssl = SSL](const boost::system::error_code &ec) {
            if (!ec) call (ex, err, hr, r, ssl.get ());
            err (ec);
        });
    }


}

