// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASYNC_WAIT_FOR_MESSAGE
#define DATA_NET_ASYNC_WAIT_FOR_MESSAGE

#include <data/net/session.hpp>

namespace data::net::async {

    template <typename message, typename error>
    struct read_stream {
        virtual void read (function<void (message, error)>) = 0;
    };

    template <typename message, typename error>
    void wait_for_message (
        ptr<read_stream<message, error>> stream,
        handler<message> receive,
        handler<error> errors) {

        struct wait {
            void operator () (
                ptr<read_stream<message, error>> stream,
                handler<message> receive,
                handler<error> errors) {

                stream->read (
                    [stream, receive, errors] (message m, const error& err) -> void {

                        if (err) return errors (err);

                        receive (m);
                        return wait {} (stream, receive, errors);

                    });
            }
        };

        wait {} (stream, receive, errors);

    };

}

#endif


