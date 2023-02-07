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
    struct socket final : async::write_stream<const string &, error_code>, async::read_stream<string_view, error_code> {

        ptr<stream> Socket;

        close_handler OnClose;

        ptr<string> Buffer;

        bool Closed;

        socket (ptr<stream> socket, close_handler on_close) : Socket {socket}, OnClose {on_close}, Buffer {new string ()}, Closed {false} {
            Buffer->resize (65025);
        }

        void write (const string &x, handler<error_code> errors) final override {
            boost::asio::async_write (*Socket, boost::asio::buffer (x.data (), x.size ()),
                [errors] (const error_code& error, size_t bytes_transferred) -> void {
                    if (error) errors (error);
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

        void read (function<void (string_view, error_code)> handle) final override {
            auto buff = Buffer;
            Socket->async_read_some (buffer (buff->data(), buff->size ()),
                [buff, handle] (const error_code& err, size_t bytes_transferred) -> void {
                    handle (std::basic_string_view {buff->data (), bytes_transferred}, err);
                });
        }
    };

}

#endif
