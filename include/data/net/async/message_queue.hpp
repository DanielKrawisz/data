// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASYNC_MESSAGE_QUEUE
#define DATA_NET_ASYNC_MESSAGE_QUEUE

#include <data/net/session.hpp>
#include <data/tools.hpp>

namespace data::net::async {

    template <typename message, typename error>
    struct write_stream {
        virtual void write (message, handler<error>) = 0;
        virtual void close () = 0;
    };

    template <typename message, typename error>
    class message_queue : public session<message>, protected std::enable_shared_from_this<message_queue<message, error>> {

        ptr<write_stream<message, error>> Stream;
        handler<error> OnError;

        // whether we are writing from the queue.
        bool Writing;

        // whether the queue has been closed. If it is, then it cannot be written to
        // and any attempt to do so will result in an error.
        bool Closed;

        list<message> Queue;

        void handle_error (const error &err) {
            Closed = true;
            OnError (err);
        }

        void write_next_message () {

            auto first = Queue.first ();
            Queue = Queue.rest ();

            Stream->write (first, [self = this->shared_from_this ()] (const error& err) -> void {
                if (err) self->handle_error (err);

                if (self->Closed) self->Stream->close ();
                else if (data::empty (self->Queue)) self->Writing = false;
                else self->write_next_message ();
            });

        }

    public:
        message_queue (ptr<write_stream<message, error>> stream, handler<error> errors):
            Stream {stream}, OnError {errors}, Writing {false}, Closed {false}, Queue {} {}

        void send (const message &e) final override {
            if (Closed) return;

            Queue = Queue << e;

            if (!Writing) {
                Writing = true;
                write_next_message ();
            }
        }

        void close () final override {
            Closed = true;
        }

        bool closed () final override {
            return Closed;
        }
    };

}

#endif


