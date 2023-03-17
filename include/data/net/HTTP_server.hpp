// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_SERVER
#define DATA_NET_HTTP_SERVER

#include <data/net/HTTP.hpp>
#include <stdlib.h>

namespace data::net::HTTP {
    using request_handler = function<response (const request &)>;

    // Accepts incoming connections and creates new session objects to handle them
    class server {

        // Handles an HTTP request and sends back an HTTP response
        class session : public std::enable_shared_from_this<session> {
        public:
            // Constructor
            session (tcp::socket&& socket, request_handler handler) : socket_( std::move (socket)), handler_( std::move (handler)) {}

            // Start the session
            void start () {
                do_read ();
            }

        private:
            // Read an HTTP request from the socket
            void do_read () {
                auto self = shared_from_this ();
                http::async_read (socket_, buffer_, request_, [self] (boost::system::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) self->do_handle_request ();
                });
            }

            // Apply the request handler to the request and send the response back
            void do_handle_request () {
                auto self = shared_from_this ();
                http::async_write (socket_, handler_(request_), [self] (boost::system::error_code ec, std::size_t bytes_transferred) {
                    if (!ec) self->socket_.shutdown (tcp::socket::shutdown_send, ec);
                });
            }

            tcp::socket socket_;
            boost::beast::flat_buffer buffer_ {8192};
            http::request<http::string_body> request_;
            request_handler handler_;
        };

        // Accept a new connection and start a new session to handle it
        void do_accept () {
            acceptor_.async_accept (boost::asio::make_strand (ioc_), [this] (boost::system::error_code ec, tcp::socket socket) {
                if (!ec) std::make_shared<Session> (std::move (socket), handler_)->start ();
                do_accept();
            });
        }

        boost::asio::io_context& ioc_;
        tcp::acceptor acceptor_;
        RequestHandler handler_;

        public:
        // Constructor
        server (boost::asio::io_context& ioc, tcp::endpoint endpoint, RequestHandler handler) :
            ioc_ (ioc), acceptor_ (ioc), handler_ (std::move (handler)) {
            boost::system::error_code ec;

            // Open the acceptor
            acceptor_.open (endpoint.protocol (), ec);
            if (ec) throw std::runtime_error ("Error opening acceptor: " + ec.message ());

            // Bind to the endpoint
            acceptor_.bind (endpoint, ec);
            if (ec) throw std::runtime_error ("Error binding acceptor: " + ec.message ());

            // Start listening for connections
            acceptor_.listen (boost::asio::socket_base::max_listen_connections, ec);
            if (ec) throw std::runtime_error ("Error listening for connections: " + ec.message ());
        }

        // Start accepting connections
        void start () {
            do_accept ();
        }

    };
}

#endif
