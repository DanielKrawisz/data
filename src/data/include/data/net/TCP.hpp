// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_TCP
#define DATA_NET_TCP

#include <data/net/URL.hpp>

namespace data::net::IP {
    
    struct exception : data::exception {
        exception (asio::error err) : data::exception {} {
            this->write ("IO error: ", err.message ());
        }
    };
}

namespace data::net::IP::TCP {

    // NOTE: these are not yet thread safe.

    // open a TCP connection.
    // curry this with the endpoint and then we have an open method that can be used
    // with serialized and JSON and whatever.
    awaitable<ptr<out_stream<byte_slice>>> open (endpoint, close_handler, interaction<bytes, byte_slice>);

    // the same thing but it works with strings instead of bytes.
    awaitable<ptr<out_stream<string_view>>> open (endpoint, close_handler, interaction<string, string_view>);
    
}

#endif

