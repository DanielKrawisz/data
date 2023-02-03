// Copyright (c) 2022-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_WEBSOCKET
#define DATA_NET_WEBSOCKET

#include <data/net/URL.hpp>
#include <data/net/asio/async_stream_session.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace data::net::websocket {

    namespace io = boost::asio;

    // open a websocket connection.
    void open (
        io::io_context &,
        const URL &,
        asio::error_handler error_handler,
        interaction<in<string_view>, out<const string &>>);

    namespace beast = boost::beast;             // from <boost/beast.hpp>
    namespace http = beast::http;               // from <boost/beast/http.hpp>
    namespace io = boost::asio;                 // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
    using insecure_stream = beast::websocket::stream<tcp::socket>;
    
    struct insecure : public session, public asio::async_stream_session<insecure, insecure_stream, char> {
        using asio::async_stream_session<insecure, insecure_stream, char>::async_stream_session;

        void close () final override {
            this->Queue.Stream->close (beast::websocket::close_code::normal);
        }

        bool closed () final override {
            throw method::unimplemented {"websocket::insecure::closed"};
        }

        ~insecure () {
            close ();
        }

    };
/*
    struct secure : asio::async_stream_session<secure, beast::websocket::stream<beast::ssl_stream<tcp::socket>>, char> {
        using asio::async_stream_session<secure, beast::websocket::stream<beast::ssl_stream<tcp::socket>>, char>::async_stream_session;

        void close () final override {
            this->Queue.Stream->close (beast::websocket::close_code::normal);
        }

        bool closed () final override {
            throw method::unimplemented {"websocket::secure::closed"};
        }

        ~secure () {
            close ();
        }
    };*/
}

#endif
