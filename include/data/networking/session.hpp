// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_CONNECTION
#define DATA_CONNECTION

#include <data/cross.hpp>

namespace data::networking {

    template <typename out>
    struct to {
        virtual ~to () {}
        virtual void send (out) = 0;
    };

    template <typename in>
    struct from {
        virtual ~from () {}
        virtual void receive (in) = 0;
    };

    struct closable {
        virtual ~closable () {}
        virtual bool closed () = 0;
        virtual void close () = 0;
    };

    template <typename in, typename out = in>
    struct connection : virtual to<out>, virtual from<in> {
        ~connection () {}
    };

    template <typename in, typename out = in>
    struct session : connection<in, out>, virtual closable {
        ~session () {}
    };
    
    // accept a lambda to receive incoming messages that disconnects the session on failure.
    template <typename in>
    struct receiver : virtual from<in>, virtual closable {
        virtual ~receiver() {}

        std::function<bool (in)> Receive;

        void receive (in x) final override {
            if (!Receive (x)) this->close ();
        }
        
        receiver (std::function<bool (in)> receive): Receive {receive} {}
    };
    
}

#endif


