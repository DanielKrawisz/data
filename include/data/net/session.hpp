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

    using close_handler = function<void ()>;

    // interaction describes an interaction with a remote peer.
    template <typename in, typename out = in>
    using interaction = function<handler<in> (ptr<session<out>>)>;

    // a function type that would open a new session.
    template <typename in, typename out = in>
    using open = handler<close_handler, interaction<in, out>>;

    
}

#endif


