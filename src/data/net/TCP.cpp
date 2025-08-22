// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/TCP.hpp>
#include <data/net/asio/TCP.hpp>
#include <boost/asio/strand.hpp>

namespace data::net::IP::TCP {

    ptr<stream> connect (exec ec, const endpoint &p, close_handler on_close) {

        ptr<asio::ip::tcp::socket> x {new asio::ip::tcp::socket (ec)};
        asio::error error;
        x->connect (asio::ip::tcp::endpoint (p), error);

        if (error) throw exception {error};
        return ptr<stream> {new stream {x, on_close}};
    }

    awaitable<ptr<out_stream<byte_slice>>> open::operator () (
        close_handler on_close,
        interaction<bytes, byte_slice> interact) {

        auto ec = co_await asio::this_coro::executor;

        auto tcp_stream = std::static_pointer_cast<net::stream<bytes, byte_slice>> (
            ThreadSafe ? connect (asio::make_strand (ec), Endpoint, on_close) : connect (ec, Endpoint, on_close));

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

        co_return out;

    }

}
