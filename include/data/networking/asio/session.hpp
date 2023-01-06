// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ASIO_ERROR_SESSION
#define DATA_ASIO_ERROR_SESSION

#include <data/networking/session.hpp>
#include <boost/asio.hpp>

namespace data::networking::asio {
    using namespace boost::asio;
    using io_error = boost::system::error_code;

}

#endif


