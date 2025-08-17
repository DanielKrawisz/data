// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/TCP.hpp>
#include <data/net/asio/TCP.hpp>
#include <iostream>

namespace data::net::IP::TCP {

    ptr<asio::tcp_stream> connect (exec ec, const endpoint &p, close_handler on_close) {

        ptr<asio::ip::tcp::socket> x {new asio::ip::tcp::socket (ec)};
        asio::error error;
        x->connect (asio::ip::tcp::endpoint (p), error);

        if (error) throw exception {error};
        return ptr<asio::tcp_stream> {new asio::tcp_stream {x, on_close}};
    }

    ptr<out_stream<byte_slice>> open (
        exec ec,
        endpoint end,
        close_handler on_close,
        interaction<bytes, byte_slice> interact) {

        auto tcp_stream = std::static_pointer_cast<stream<bytes, byte_slice>> (connect (ec, end, on_close));
        auto out = std::static_pointer_cast<out_stream<byte_slice>> (tcp_stream);

        spawn (ec, [
            in = std::static_pointer_cast<in_stream<bytes>> (tcp_stream),
            in_handler = interact (out)
        ]() -> awaitable<void> {
            while (true) {
                maybe<bytes> msg = co_await in->receive ();
                if (!bool (msg)) break;
                in_handler (*msg);
            }
        });

        return out;

    }

}
