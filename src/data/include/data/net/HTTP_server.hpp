// Copyright (c) 2022-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_SERVER
#define DATA_NET_HTTP_SERVER

#include <stdlib.h>
#include <data/async.hpp>
#include <data/net/HTTP.hpp>
#include <data/net/asio/session.hpp>
#include <data/net/beast/beast.hpp>
#include <unordered_set>

namespace data::net::HTTP::beast {
    bool is_websocket_upgrade (const request &);
    awaitable<void> write_websocket_decline (asio::ip::tcp::socket &);
}

namespace data::net::HTTP {

    using request_handler = function<awaitable<response> (const request &)>;

    // Accepts incoming connections and creates new session objects to handle them
    class server {

        class session;

        class sessions {

            std::unordered_set<ptr<session>> Sessions;
            std::mutex Mtx;

        public:
            void add (ptr<session>);
            void remove (ptr<session>);
            void remove_all ();
        };

        // Handles an HTTP request and sends back an HTTP response
        class session {
            // Apply the request handler to the request and send the response back
            awaitable<void> handle_request (const beast::request req) {
                if (beast::is_websocket_upgrade (req))
                    co_await beast::write_websocket_decline (Socket);
                // Write the response
                else co_await beast::http::async_write (Socket, beast::to (co_await Handler (beast::from (req))), asio::use_awaitable);
            }

            asio::ip::tcp::socket Socket;
            request_handler Handler;
            sessions &Sessions;

        public:
            // Constructor
            session (sessions &x, asio::ip::tcp::socket &&socket, request_handler handler) :
                Sessions {x}, Socket (std::move (socket)), Handler (std::move (handler)) {}

            // Read an HTTP request from the socket
            awaitable<void> read ();

            void close () {
                Socket.cancel ();
            }

            ptr<session> Self {};
        };

        asio::ip::tcp::acceptor Acceptor;
        request_handler Handler;

        sessions Sessions;

    public:
        // Constructor
        server (exec ex, net::IP::TCP::endpoint ep, request_handler handler);

        // Accept a new connection and start a new session to handle it
        awaitable<bool> accept ();

        void close () {
            if (Acceptor.is_open ()) {
                Acceptor.cancel ();
                Acceptor.close ();
            }

            Sessions.remove_all ();
        }

        ~server () {
            close ();
        }

    };
}

#endif
