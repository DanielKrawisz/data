// Copyright (c) 2022-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_WEBSOCKET
#define DATA_NET_WEBSOCKET

#include <data/net/URL.hpp>
#include <data/net/asio/session.hpp>

namespace data::net::websocket {

    // open a websocket connection.
    void open (
        asio::io_context &,
        const URL &,
        asio::error_handler error_handler,
        interaction<string_view, const string &>,
        close_handler);
    
}

#endif
