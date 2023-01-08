// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_WEBSOCKET
#define DATA_NETWORKING_WEBSOCKET

#include <data/networking/asio/async_stream_session.hpp>

namespace data::networking::websocket {
    namespace io = boost::asio;

    struct open {
        io::io_context &IO;
        string URL;

        ptr<session<const string &>> operator() (receive_session<session<const string &>, string_view>);
    };
    
    struct insecure : asio::async_stream_session<session<const string &>, websocket::stream<beast::tcp_stream>, char> {
        using asio::async_stream_session<session<const string &>, websocket::stream<beast::tcp_stream>, char>::async_stream_session;

        void close () final override {
            throw method::unimplemented {"websocket::insecure::close"};
        }

        bool closed () final override {
            throw method::unimplemented {"websocket::insecure::closed"};
        }

        function<void (io_error)> HandleError;

        ~insecure () {
            close ();
        }
    };

    struct secure : async_stream_session<session<const string &>, websocket::stream<beast::ssl_stream<beast::tcp_stream>>, char> {
        using asio::async_stream_session<session<const string &>, websocket::stream<beast::ssl_stream<beast::tcp_stream>>, char>::async_stream_session;

        void close () final override {
            throw method::unimplemented {"websocket::secure::close"};
        }

        bool closed () final override {
            throw method::unimplemented {"websocket::secure::closed"};
        }

        function<void (io_error)> HandleError;

        ~secure () {
            close ();
        }
    };
}

#endif
