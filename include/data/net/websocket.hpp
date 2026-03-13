// Copyright (c) 2022-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_WEBSOCKET
#define DATA_NET_WEBSOCKET

#include <data/net/URL.hpp>
#include <data/net/HTTP.hpp>
#include <data/net/asio/stream.hpp>

namespace data::net::websocket {

    // open a websocket connection.
    void open (
        asio::io_context &,
        const URL &,
        HTTP::SSL *,
        asio::error_handler error_handler,
        close_handler,
        interaction<byte_slice, const bytes &>);

    void open_secure (
        asio::io_context &,
        const URL &,
        HTTP::SSL &,
        asio::error_handler error_handler,
        close_handler,
        interaction<byte_slice, const bytes &>);

    void open_insecure (
        asio::io_context &,
        const URL &,
        asio::error_handler error_handler,
        close_handler,
        interaction<byte_slice, const bytes &>);
    
}

#include <boost/beast/websocket.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

namespace data::net::websocket {

    namespace beast = boost::beast;
    namespace websocket = beast::websocket;
    namespace net = boost::asio;
    using tcp = net::ip::tcp;

    class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
    public:
        explicit WebSocketSession (tcp::socket socket) : ws_ (std::move (socket)) {}

        void run () {
            ws_.async_accept (beast::bind_front_handler(&WebSocketSession::on_accept, shared_from_this()));
        }

    private:
        void on_accept (beast::error_code ec) {
            if (ec) {
                std::cerr << "Accept error: " << ec.message () << std::endl;
                return;
            }

            read ();
        }

        void read () {
            ws_.async_read (buffer_, beast::bind_front_handler (&WebSocketSession::on_read, shared_from_this ()));
        }

        void on_read (beast::error_code ec, std::size_t bytes_transferred) {
            if (ec) {
                std::cerr << "Read error: " << ec.message () << std::endl;
                return;
            }

            ws_.text (ws_.got_text ());
            ws_.async_write (buffer_.data (), beast::bind_front_handler (&WebSocketSession::on_write, shared_from_this ()));

            buffer_.consume (bytes_transferred);
        }

        void on_write (beast::error_code ec, std::size_t) {
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
                return;
            }

            read ();
        }

        websocket::stream<tcp::socket> ws_;
        beast::flat_buffer buffer_;
    };
/*
    void accept_connection (tcp::acceptor &acceptor) {
        acceptor.async_accept([&] (beast::error_code ec, tcp::socket socket) {
            if (!ec)  std::make_shared<WebSocketSession> (std::move (socket))->run ();
            accept_connection (acceptor);
        });
    }

    int main () {
        const auto address = net::ip::make_address("127.0.0.1");
        const auto port = static_cast<unsigned short>(8080);

        net::io_context ioc;
        tcp::acceptor acceptor {ioc, {address, port}};

        std::cout << "WebSocket server listening on " << address << ":" << port << std::endl;
        accept_connection (acceptor);
        ioc.run ();

        return 0;
    }*/
}

#endif
