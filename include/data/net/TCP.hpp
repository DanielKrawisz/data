// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_TCP
#define DATA_NET_TCP

#include <data/net/URL.hpp>
#include <data/net/asio/TCP.hpp>

namespace data::net::IP {
    
    struct exception : data::exception {
        exception (asio::error err) : data::exception {} {
            this->write ("IO error: ", err.message ());
        }
    };
}

namespace data::net::IP::TCP {

    // open a TCP connection.
    struct open {
        endpoint Endpoint;
        bool ThreadSafe;

        open (const endpoint e, bool thread_safe = true): Endpoint {e}, ThreadSafe {thread_safe} {}

        // we provide the option of bytes or string to represent the data along the stream.
        awaitable<ptr<out_stream<byte_slice>>> operator () (close_handler, interaction<bytes, byte_slice>);
        awaitable<ptr<out_stream<string_view>>> operator () (close_handler, interaction<string, string_view>);
    };

    // note: to make this thread safe, use make_strand on the executor.
    ptr<stream> connect (exec ec, const endpoint &p, close_handler on_close);
    
}

#endif

