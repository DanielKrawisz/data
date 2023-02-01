// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_ASIO_ASYNC_RECEIVER
#define DATA_NETWORKING_ASIO_ASYNC_RECEIVER

#include <data/net/asio/session.hpp>

namespace data::net::asio {

    template <typename async_stream, typename word>
    void async_wait_for_message (
        ptr<async_stream> stream,
        function<void (std::basic_string_view<word>)> receive,
        function<void (io_error)> error,
        uint32 buffer_size = 65536) {

        auto buffer = new std::basic_string<word> {};
        buffer->resize (buffer_size);

        struct wait_for_message {
            void operator () (
                ptr<async_stream> stream,
                function<void (std::basic_string_view<word>)> receive,
                function<void (io_error)> error,
                std::basic_string<word> *buffer) {

                stream->async_read_some (
                    boost::asio::buffer (buffer->data (), buffer->size ()),
                    [stream, receive, error, buffer] (const io_error& err, size_t bytes_transferred) -> void {

                        if (err) goto done;

                        try {

                            receive (std::basic_string_view {buffer->data (), bytes_transferred});
                            wait_for_message {} (stream, receive, error, buffer);

                            return;

                        } catch (...) {}

                        done:

                        stream->close ();
                        error (err);
                        delete buffer;

                    });
            }
        };

        wait_for_message {} (stream, receive, error, buffer);

    };

}

#endif


