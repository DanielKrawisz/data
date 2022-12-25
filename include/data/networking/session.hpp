// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_SESSION
#define DATA_SESSION

#include <data/networking/connection.hpp>

namespace data::networking {
    
    // a session is a connection that can be closed.
    template <typename from, typename to = from>
    struct session : virtual connection<from, to> {
        virtual ~session() {}
        virtual bool closed() = 0;
        virtual void close() = 0;
    };
    
}

#endif

