// Copyright (c) 2022-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_SERVER
#define DATA_NET_HTTP_SERVER

#include <stdlib.h>
#include <data/net/HTTP.hpp>
#include <data/net/asio/session.hpp>
#include <data/net/beast/beast.hpp>

namespace data::net::HTTP {

    using request_handler = function<awaitable<response> (const request &)>;

    // Accepts incoming connections and creates new session objects to handle them
    class server {

        // Handles an HTTP request and sends back an HTTP response
        class session {
        public:
            // Constructor
            session (asio::ip::tcp::socket &&socket, request_handler handler) :
                Socket (std::move (socket)), Handler (std::move (handler)) {}

            // Read an HTTP request from the socket
            awaitable<void> read () {
                beast::request Request;
                co_await beast::http::async_read (Socket, Buffer, Request, asio::use_awaitable);
                co_await handle_request (Request);
            }

            ~session () {
                asio::error ec;
                Socket.shutdown (asio::ip::tcp::socket::shutdown_send, ec);
            }

        private:

            // Apply the request handler to the request and send the response back
            awaitable<void> handle_request (const beast::request req) {
                // Write the response
                co_await beast::http::async_write (Socket, beast::to (co_await Handler (beast::from (req))), asio::use_awaitable);
            }

            asio::ip::tcp::socket Socket;
            boost::beast::flat_buffer Buffer {8192};
            request_handler Handler;
        };

        // Accept a new connection and start a new session to handle it
        awaitable<void> accept () {

            asio::ip::tcp::socket socket = co_await Acceptor.async_accept (asio::use_awaitable);
            // Spawn a new session to handle the connection
            co_spawn (
                Acceptor.get_executor (),
                     [sock = std::move (socket), Handler = this->Handler] () mutable -> awaitable<void> {
                         co_await session {std::move (sock), Handler}.read ();
                         co_return;
                     },
                     asio::detached
            );
        }

        asio::ip::tcp::acceptor Acceptor;
        request_handler Handler;

    public:
        // Constructor
        server (exec ex, asio::ip::tcp::endpoint endpoint, request_handler handler) :
            Acceptor {ex}, Handler {std::move (handler)} {
            asio::error ec;

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
        awaitable<void> run () {
            while (true) {
                co_await accept ();
            }
        }

    };
}

#endif
