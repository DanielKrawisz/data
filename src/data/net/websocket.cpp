// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/websocket.hpp>
#include <data/net/async/message_queue.hpp>
#include <data/net/async/wait_for_message.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace data::net::websocket {

    namespace beast = boost::beast;             // from <boost/beast.hpp>
    namespace http = beast::http;               // from <boost/beast/http.hpp>
    namespace io = boost::asio;                 // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;           // from <boost/asio/ip/tcp.hpp>
    using insecure_stream = beast::websocket::stream<tcp::socket>;
    using secure_stream = beast::websocket::stream<beast::ssl_stream<beast::tcp_stream>>;

    template <typename stream>
    struct socket final : async::write_stream<const string &, asio::error_code>, async::read_stream<string_view, asio::error_code> {

        ptr<stream> Socket;

        close_handler OnClose;

        ptr<string> Buffer;

        bool Closed;

        socket (ptr<stream> socket, close_handler on_close) : Socket {socket}, OnClose {on_close}, Buffer {new string ()}, Closed {false} {
            Buffer->resize (65025);
        }

        void write (const string &x, handler<asio::error_code> errors) final override {
            Socket->async_write (asio::buffer (x.data (), x.size ()),
                [errors] (const asio::error_code& error, size_t bytes_transferred) -> void {
                    if (error) errors (error);
                });
        }

        void close () final override {
            if (Closed) return;
            Closed = true;
            // TODO put the right funnction in here
            //Socket->close ();
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

        void read (function<void (string_view, asio::error_code)> handle) final override {
            auto buff = Buffer;
            // TODO using the wrong buffer type here.
            /*
            Socket->async_read (asio::buffer (buff->data(), buff->size ()),
                [buff, handle] (const asio::error_code& err, size_t bytes_transferred) -> void {
                    handle (std::basic_string_view {buff->data (), bytes_transferred}, err);
                });
                */
        }
    };

    void open (
        HTTP::caller &caller,
        const URL &url,
        asio::error_handler error_handler,
        interaction<string_view, const string &> interact,
        close_handler closed) {

        // TODO fill this in correctly

        if (url.Protocol != protocol::WS && url.Protocol != protocol::WSS)
            throw exception{} << "protocol " << url.Protocol << " is not websockets";
        if (!caller.SSLContext && url.Protocol == protocol::WSS)
            throw exception{} << "Secure websocket requested when SSL Context not supplied";

        // These objects perform our I/O
        ptr<insecure_stream> ws = std::make_shared<insecure_stream> (*caller.IOContext);

        // Look up the domain name
        auto const results = caller.Resolver.resolve (url.Host, url.Port);

        // Make the connection on the IP address we get from a lookup
        auto ep = io::connect (ws->next_layer (), results);

        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        string host = url.Host + ':' + std::to_string (ep.port ());

        // Set a decorator to change the User-Agent of the handshake
        ws->set_option (beast::websocket::stream_base::decorator (
                [] (beast::websocket::request_type &req) {
                    req.set (http::field::user_agent,
                            std::string (BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-coro");
                }));

        // Perform the websocket handshake
        ws->handshake (host, "/");

        ptr<socket<insecure_stream>> z {new socket<insecure_stream> {ws, closed}};

    }

}

