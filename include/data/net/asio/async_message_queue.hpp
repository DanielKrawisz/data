// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_ASIO_ASYNC_MESSAGE_QUEUE
#define DATA_NETWORKING_ASIO_ASYNC_MESSAGE_QUEUE

#include <data/net/asio/session.hpp>
#include <data/tools.hpp>

namespace data::net::asio {

    template <typename async_stream, typename word>
    struct async_message_queue : protected std::enable_shared_from_this<async_message_queue<async_stream, word>> {

        ptr<async_stream> Stream;
        function<void (io_error)> HandleError;

        // whether we are writing from the queue.
        bool Writing;
        list<std::basic_string<word>> Queue;

        async_message_queue (ptr<async_stream> stream, function<void (io_error)> errors):
            Stream {std::move (stream)}, HandleError {errors}, Writing {false}, Queue {} {}

        void write_next_message () {

            auto first = Queue.first ();
            Queue = Queue.rest ();

            boost::asio::async_write (*Stream, boost::asio::buffer (first.data (), first.size ()),
                [self = this->shared_from_this ()] (const io_error& error, size_t bytes_transferred) -> void {
                    if (error) self->HandleError (error);

                    if (data::empty (self->Queue)) self->Writing = false;
                    else self->write_next_message ();
                });

        }

        void write (const std::basic_string<word> &b) {
            Queue = Queue << b;

            if (!Writing) {
                Writing = true;
                write_next_message ();
            }
        }
    };

}

#endif
