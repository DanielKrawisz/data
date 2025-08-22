// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_STREAM
#define DATA_NET_STREAM

#include <data/maybe.hpp>
#include <data/async.hpp>

namespace data::net {

    // A basic session type. Not every session
    // works exactly the same way, so
    // you are not required to use this.

    template <typename message>
    struct out_stream {
        // throw data::exception on failure.
        virtual awaitable<void> send (message) = 0;
        virtual void close () = 0;
        virtual ~out_stream () {}
    };

    template <typename message>
    struct in_stream {
        // throw data::exception on failure.
        virtual awaitable<message> receive () = 0;
        virtual bool closed () = 0;
        virtual ~in_stream () {}
    };

    template <typename in, typename out = const in &>
    struct stream : in_stream<in>, out_stream<out> {
        virtual ~stream () {}
    };

    using close_handler = function<void ()>;

    // interaction with a remote peer.
    template <typename in, typename out = const in &>
    using interaction = function<handler<inserted<in>> (ptr<out_stream<out>>)>;

    // a function to open a new session.
    // you don't get an in session because you already
    // gave the function a way to respond to incoming messages.
    template <typename in, typename out = const in &>
    using open = function<awaitable<ptr<out_stream<out>>> (close_handler, interaction<in, out>)>;
    
}

#endif


