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

    template <typename session_out, typename message_in>
    using receive_handler = function<function<void (message_in)> (ptr<session_out>)>;

    template <typename session_out, typename message_in>
    using open = function<ptr<session_out> (receive_handler<session_out, message_in>)>;

    
}

#endif


