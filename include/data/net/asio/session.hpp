// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_ERROR_SESSION
#define DATA_NET_ASIO_ERROR_SESSION

#include <data/net/session.hpp>
#include <boost/asio.hpp>

namespace data::net::asio {
    using namespace boost::asio;
    using io_error = boost::system::error_code;

}

#endif


