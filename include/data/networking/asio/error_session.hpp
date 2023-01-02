// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_ASIO_ERROR_SESSION
#define DATA_ASIO_ERROR_SESSION

#include <data/networking/session.hpp>
#include <boost/asio.hpp>

namespace data::networking::asio {
    using namespace boost::asio;
    using io_error = boost::system::error_code;

    template <typename from, typename to = from>
    struct error_session : session<from, to> {
        virtual ~error_session() {}

        virtual void handle_error (const io_error &err) = 0;
    };

}

#endif


