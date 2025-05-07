// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/HTTP_client.hpp>

namespace data::net::HTTP {

    awaitable<response> client::operator () (const request &r) {
        if (Session.get () == nullptr || !Session->is_open ())
            Session = co_await connect (version_1_1, REST.Host, SSL.get ());

        std::chrono::milliseconds wait = Rate.get_time ();
        if (wait != std::chrono::milliseconds {0}) co_await sleep (wait);
        co_return co_await Session->request (r);
    }

}

