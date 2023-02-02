// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_SESSION
#define DATA_NET_SESSION

#include <data/types.hpp>

namespace data::net {

    // A basic session type. Not every session
    // works exactly the same way, so
    // you are not required to use this.
    template <typename message_out>
    struct session {
        virtual ~session () {}
        virtual void send (message_out) = 0;
        virtual bool closed () = 0;
        virtual void close () = 0;
    };

    // interaction describes an interaction with a remote peer.
    // it takes a session type called session_out (which could be the type above)
    // and returns a function that takes the message type that will be received
    // by the remote peer. This method will be called when a message is received.
    template <typename message_in, typename session_out = session<message_in>>
    using interaction = function<handler<message_in> (ptr<session_out>)>;

    // a function type that would open a new session.
    template <typename message_in, typename session_out = session<message_in>>
    using open = handler<interaction<session_out, message_in>>;

    
}

#endif


