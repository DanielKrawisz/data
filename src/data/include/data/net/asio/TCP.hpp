// Copyright (c) 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_TCP
#define DATA_NET_ASIO_TCP

#include <data/net/asio/stream.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace data::net::asio {

    using tcp_stream = stream<ip::tcp::socket>;

}

#endif

