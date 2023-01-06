// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONNECTION
#define DATA_CONNECTION

#include <data/types.hpp>

namespace data::networking {

    template <typename message_out>
    struct session {
        virtual ~session () {}
        virtual void send (message_out) = 0;
        virtual bool closed () = 0;
        virtual void close () = 0;
    };

    template <typename message_out, typename message_in = message_out>
    using receive_handler = function<function<void (message_in)> (ptr<session<message_out>>)>;

    template <typename message_out, typename message_in = message_out>
    using open = function<ptr<session<message_out>> (receive_handler<message_out, message_in>)>;

    
}

#endif


