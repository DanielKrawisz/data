// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_ASYNC_RECEIVER
#define DATA_NET_ASIO_ASYNC_RECEIVER

#include <data/net/asio/session.hpp>

namespace data::net::asio {

    template <async_write_stream async_stream, typename word>
    void async_wait_for_message (
        ptr<async_stream> stream,
        handler<std::basic_string_view<word>> receive,
        error_handler error,
        uint32 buffer_size = 65536) {

        auto buff = new std::basic_string<word> {};
        buff->resize (buffer_size);

        struct wait_for_message {
            void operator () (
                ptr<async_stream> stream,
                handler<std::basic_string_view<word>> receive,
                error_handler error,
                std::basic_string<word> *buff) {

                stream->async_read_some (
                    buffer (buff->data (), buff->size ()),
                    [stream, receive, error, buff] (const error_code& err, size_t bytes_transferred) -> void {

                        if (err) goto done;

                        try {

                            receive (std::basic_string_view {buff->data (), bytes_transferred});
                            wait_for_message {} (stream, receive, error, buff);

                            return;

                        } catch (...) {}

                        done:

                        stream->close ();
                        error (err);
                        delete buff;

                    });
            }
        };

        wait_for_message {} (stream, receive, error, buff);

    };

}

#endif


