// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_WEBSOCKET
#define DATA_NETWORKING_WEBSOCKET

#include <data/networking/session.hpp>
#include <boost/asio.hpp>

namespace data::networking::websocket {
    namespace io = boost::asio;
    
    static ptr<session<bytes_view>> connect(io::io_context &, string url, std::function<bool (bytes_view)>);
    
    // Notes: destructor must close session. 
}

#endif
