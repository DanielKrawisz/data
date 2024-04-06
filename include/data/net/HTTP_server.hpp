// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_SERVER
#define DATA_NET_HTTP_SERVER

#include <stdlib.h>
#include <data/net/asio/session.hpp>

#include <data/net/beast/beast.hpp>

namespace data::net::HTTP {

    using request_handler = function<response (const request &)>;

    // Accepts incoming connections and creates new session objects to handle them
    class server {

        // Handles an HTTP request and sends back an HTTP response
        class session : public std::enable_shared_from_this<session> {
        public:
            // Constructor
            session (asio::ip::tcp::socket &&socket, request_handler handler) : Socket (std::move (socket)), Handler (std::move (handler)) {}

            // Start the session
            void start () {
                do_read ();
            }

        private:
            // Read an HTTP request from the socket
            void do_read () {
                auto self = shared_from_this ();
                beast::http::async_read (Socket, Buffer, Request, [self] (asio::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) self->do_handle_request ();
                });
            }

            // Apply the request handler to the request and send the response back
            void do_handle_request () {
                auto self = shared_from_this ();
                beast::http::async_write
                (Socket, beast::from (Handler (beast::to (Request))), [self] (asio::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) self->Socket.shutdown (asio::ip::tcp::socket::shutdown_send, ec);
                });
            }

            asio::ip::tcp::socket Socket;
            boost::beast::flat_buffer Buffer {8192};
            beast::request Request;
            request_handler Handler;
        };

        // Accept a new connection and start a new session to handle it
        void do_accept () {
            Acceptor.async_accept (asio::make_strand (IO), [this] (asio::error_code ec, asio::ip::tcp::socket socket) {
                if (!ec) std::make_shared<session> (std::move (socket), Handler)->start ();
                do_accept ();
            });
        }

        asio::io_context &IO;
        asio::ip::tcp::acceptor Acceptor;
        request_handler Handler;

    public:
        // Constructor
        server (asio::io_context &ioc, asio::ip::tcp::endpoint endpoint, request_handler handler) :
            IO {ioc}, Acceptor {ioc}, Handler {std::move (handler)} {
            asio::error_code ec;

            // Open the acceptor
            Acceptor.open (endpoint.protocol (), ec);
            if (ec) throw std::runtime_error ("Error opening acceptor: " + ec.message ());

            // Bind to the endpoint
            Acceptor.bind (endpoint, ec);
            if (ec) throw std::runtime_error ("Error binding acceptor: " + ec.message ());

            // Start listening for connections
            Acceptor.listen (asio::socket_base::max_listen_connections, ec);
            if (ec) throw std::runtime_error ("Error listening for connections: " + ec.message ());
        }

        // Start accepting connections
        void start () {
            do_accept ();
        }

    };
}

#endif
