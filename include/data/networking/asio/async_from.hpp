// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_ASIO_ASYNC_RECEIVER
#define DATA_NETWORKING_ASIO_ASYNC_RECEIVER

#include <data/networking/asio/session.hpp>

namespace data::networking::asio {

    template <typename AsyncStream, typename word>
    struct async_from :
        virtual from<std::basic_string_view<word>>,
        virtual error_handler,
        protected std::enable_shared_from_this<async_from<AsyncStream, word>> {

        ptr<AsyncStream> Stream;

        std::basic_string<word> Buffer;

        async_from (AsyncStream *stream, uint32 buffer_size = 65536):
            Stream {stream}, Buffer {} {
            Buffer.resize(buffer_size);
        }

        // begin waiting for the next message asynchronously.
        void wait_for_message () {
            this->Stream->async_read_some (boost::asio::buffer (Buffer.data (), Buffer.size ()),
                [self = this->shared_from_this ()] (const io_error& error, size_t bytes_transferred) -> void {
                    if (error) return self->handle_error (error);

                    try {
                        self->receive (std::basic_string_view {self->Buffer.data(), bytes_transferred});
                        self->wait_for_message ();
                    } catch (...) {
                        self->Stream->close ();
                    }
                });
        }

    };

}

#endif


