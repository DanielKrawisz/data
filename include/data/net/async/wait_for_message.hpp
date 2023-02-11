// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASYNC_WAIT_FOR_MESSAGE
#define DATA_NET_ASYNC_WAIT_FOR_MESSAGE

#include <data/net/session.hpp>

namespace data::net::async {

    template <typename message>
    struct read_stream {
        virtual void read (handler<message>) = 0;
        virtual bool closed () = 0;
    };

    template <typename message>
    void wait_for_message (
        ptr<read_stream<message>> stream,
        handler<message> receive) {

        struct wait {
            void operator () (
                ptr<read_stream<message>> stream,
                handler<message> receive) {

                stream->read (
                    [stream, receive] (message m) -> void {

                        receive (m);

                        if (!stream->closed ()) wait {} (stream, receive);

                    });
            }
        };

        wait {} (stream, receive);

    };

}

#endif


