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
    struct out {
        virtual ~out () {}
        virtual void send (message_out) = 0;
        virtual bool closed () = 0;
        virtual void close () = 0;
    };

    // type encompassing events that can occur during a session.
    // the implementation of these functions tell how to react to them.
    template <typename message_in>
    struct in {
        virtual ~in () {}
        virtual void receive (message_in) = 0;
        virtual void close () = 0;
    };

    // interaction describes an interaction with a remote peer.
    // it takes a session type called session_out (which could be the type above)
    // and returns a function that takes the message type that will be received
    // by the remote peer. This method will be called when a message is received.
    template <typename in, typename out>
    using interaction = function<ptr<in> (ptr<out>)>;

    // a function type that would open a new session.
    template <typename in, typename out>
    using open = handler<interaction<in, out>>;

    
}

#endif


