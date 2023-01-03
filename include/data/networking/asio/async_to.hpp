// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_ASIO_ASYNC_MESSAGE_QUEUE
#define DATA_NETWORKING_ASIO_ASYNC_MESSAGE_QUEUE

#include <data/networking/asio/session.hpp>
#include <data/tools.hpp>

namespace data::networking::asio {

    template <typename AsyncStream, typename word>
    struct async_to :
        virtual to<const std::basic_string<word> &>,
        virtual error_handler,
        protected std::enable_shared_from_this<async_to<AsyncStream, word>> {

        AsyncStream *Stream;

        // whether we are writing from the queue.
        bool Writing;
        list<std::basic_string<word>> Queue;

        async_to (AsyncStream *stream): Stream {std::move (stream)}, Writing {false}, Queue {} {}

        void send_next_message () {

            auto first = Queue.first ();
            Queue = Queue.rest ();

            boost::asio::async_write(*Stream, first, [self = this->shared_from_this ()] (const io_error& error, size_t bytes_transferred) -> void {
                if (error) self->handle_error (error);

                if (data::empty (self->Queue)) self->Writing = false;
                else self->send_next_message ();
            });

        }

        void send (const std::basic_string<word> &b) final override {
            Queue = Queue << b;

            if (!Writing) {
                Writing = true;
                send_next_message ();
            }
        }
    };

}

#endif

