// Copyright (c) 2023 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_ASIO_NETWORKING_MESSAGE_QUEUE
#define DATA_ASIO_NETWORKING_MESSAGE_QUEUE

#include <data/networking/asio/error_session.hpp>
#include <data/tools.hpp>

namespace data::networking::asio {

    template <typename child, typename AsyncWriteStream, typename from, typename Buffer>
    struct message_queue : error_session<from, const Buffer &>, protected std::enable_shared_from_this<child> {
        AsyncWriteStream Stream;
        bool Writing;
        list<Buffer> Messages;

        message_queue (AsyncWriteStream &&stream): Stream {std::move (stream)}, Writing {false}, Messages {} {}

        void send_next_message () {

            auto first = Messages.first ();
            Messages = Messages.rest ();

            boost::asio::async_write(Stream, first, [self = this->shared_from_this ()] (const io_error& error, size_t bytes_transferred) -> void {
                if (error) self->handle_error (error);

                if (data::empty (self->Messages)) self->Writing = false;
                else self->send_next_message ();
            });

        }

        void send (const Buffer &b) final override {
            Messages = Messages << b;

            if (!Writing) {
                Writing = true;
                send_next_message ();
            }
        }


    };

}

#endif

