// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASYNC_MESSAGE_QUEUE
#define DATA_NET_ASYNC_MESSAGE_QUEUE

#include <data/net/session.hpp>
#include <data/tools.hpp>

namespace data::net::async {

    template <typename message>
    struct write_stream {
        virtual void write (message, function<void ()>) = 0;
        virtual void close () = 0;
    };

    template <typename message>
    class message_queue : public session<message>, protected std::enable_shared_from_this<message_queue<message>> {

        ptr<write_stream<message>> Stream;

        // whether we are writing from the queue.
        bool Writing;

        list<message> Queue;

        bool Closed;

        void write_next_message () {

            auto first = Queue.first ();
            Queue = Queue.rest ();

            Stream->write (first, [self = this->shared_from_this ()] () -> void {
                if (self->Closed) self->Stream->close ();
                else if (data::empty (self->Queue)) self->Writing = false;
                else self->write_next_message ();
            });

        }

    public:
        message_queue (ptr<write_stream<message>> stream):
            Stream {stream}, Writing {false}, Queue {}, Closed {false} {}

        void send (const message &e) final override {
            if (Closed) return;

            Queue = Queue << e;

            if (!Writing) {
                Writing = true;
                write_next_message ();
            }
        }

        void close () final override {
            if (Closed) return;
            Closed = true;
            Stream->close ();
        }

        bool closed () final override {
            return Closed;
        }
    };

}

#endif


