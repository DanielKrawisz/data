// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ASIO_SOCKET
#define DATA_NET_ASIO_SOCKET

#include <data/net/asio/session.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <data/net/async/message_queue.hpp>
#include <data/net/async/wait_for_message.hpp>

namespace data::net::asio {

    template <async_write_stream stream>
    struct socket final :
        async::write_stream<const string &>,
        async::read_stream<string_view>,
        std::enable_shared_from_this<socket<stream>> {

        ptr<stream> Socket;

        handler<error> OnError;
        close_handler OnClose;

        string Buffer;

        bool Closed;

        socket (ptr<stream> socket, handler<error> errors, close_handler on_close) :
            Socket {socket}, OnError {errors}, OnClose {on_close}, Buffer {}, Closed {false} {
            Buffer.resize (65025);
        }

        void write (const string &x, function<void ()> on_complete) final override {
            async_write (*Socket, buffer (x.data (), x.size ()),
                [self = this->shared_from_this (), on_complete] (const error &error, size_t bytes_transferred) -> void {
                    if (error == boost::asio::error::eof) self->close ();
                    else if (error) self->OnError (error);
                    else on_complete ();
                });
        }

        void close () final override {
            if (Closed) return;
            Closed = true;
            Socket->close ();
            OnClose ();
        }

        bool closed () final override {
            bool closed = !Socket->is_open ();
            if (closed && !Closed) {
                Closed = true;
                OnClose ();
            }
            return closed;
        }

        void read (function<void (string_view)> handle) final override {
            Socket->async_read_some (buffer (Buffer.data(), Buffer.size ()),
                [self = this->shared_from_this (), handle] (const error& error, size_t bytes_transferred) -> void {
                    if (error == boost::asio::error::eof) self->close ();
                    if (bytes_transferred > 0) handle (std::basic_string_view {self->Buffer.data (), bytes_transferred});
                });
        }
    };

}

#endif
