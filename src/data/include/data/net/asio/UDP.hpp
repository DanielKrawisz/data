// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_UDP
#define DATA_NET_ASIO_UDP

#include <data/net/stream.hpp>
#include <boost/asio/ip/udp.hpp>

namespace data::net::asio {

    using udp_stream = stream<ip::tcp::socket>;

}

#endif

