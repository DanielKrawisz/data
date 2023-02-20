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
    using secure_stream = beast::websocket::stream<beast::ssl_stream<tcp::socket>>;

    template <typename stream>
    struct socket final :
        async::write_stream<const string &>,
        async::read_stream<string_view>,
        std::enable_shared_from_this<socket<stream>> {

        ptr<stream> Socket;

        handler<asio::error_code> OnError;
        close_handler OnClose;

        beast::flat_buffer Buffer;

        bool Closed;

        socket (ptr<stream> socket, handler<asio::error_code> errors, close_handler on_close) :
            Socket {socket}, OnError {errors}, OnClose {on_close}, Buffer {}, Closed {false} {}

        void write (const string &x, function<void ()> on_complete) final override {
            Socket->async_write (asio::buffer (x.data (), x.size ()),
                [self = this->shared_from_this (), on_complete] (const asio::error_code& error, size_t bytes_transferred) -> void {
                    if (error == beast::websocket::error::closed) self->close ();
                    else if (error) self->OnError (error);
                    else on_complete ();
                });
        }

        void close () final override {
            if (Closed) return;
            Closed = true;
            Socket->async_close (
                beast::websocket::close_code::normal,
                [on_close = OnClose, on_error = OnError] (beast::error_code e) -> void {
                    if (e) on_error (e);
                    on_close ();
                });
        }

        bool closed () final override {
            bool closed = !Socket->is_open ();
            if (closed && !Closed) close ();
            return closed;
        }

        void read (function<void (string_view)> handle) final override {
            Socket->async_read (Buffer,
                [self = this->shared_from_this (), handle] (const asio::error_code& error, size_t bytes_transferred) -> void {
                    if (error == beast::websocket::error::closed) self->close ();
                    else if (error) self->OnError (error);

                    auto seq = self->Buffer.data ();

                    string x;
                    x.resize (bytes_transferred);
                    std::copy (boost::asio::buffer_sequence_begin(x), boost::asio::buffer_sequence_end(x), x.begin());

                    handle (std::basic_string_view {x.data (), bytes_transferred});
                });
        }
    };

    void insecure_open (
        asio::io_context &io,
        const URL &url,
        asio::error_handler error_handler,
        interaction<string_view, const string &> interact,
        close_handler closed) {
        // These objects perform our I/O
        ptr<insecure_stream> ws = std::make_shared<insecure_stream> (io);

        asio::ip::tcp::resolver resolver (io);
        // Look up the domain name
        auto const results = resolver.resolve (url.Host, url.Port);

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

        ptr<socket<insecure_stream>> ss {new socket<insecure_stream> {ws, error_handler, closed}};

        ptr<session<const string &>> zz {static_cast<session<const string &> *>
                                        (new async::message_queue<const string &>{
                        std::static_pointer_cast<async::write_stream<const string &>> (ss)})};

        async::wait_for_message<string_view> (std::static_pointer_cast<async::read_stream<string_view>> (ss),
                                             [xx = interact (zz)] (string_view z) -> void {
                                                 return xx (z);
                                             });
    }

    void secure_open (
        asio::io_context &io,
        const URL &url,
        HTTP::SSL &ssl,
        asio::error_handler error_handler,
        interaction<string_view, const string &> interact,
        close_handler closed) {
// These objects perform our I/O
        ptr<secure_stream> ws = std::make_shared<secure_stream> (io, ssl);

        asio::ip::tcp::resolver resolver (io);
        // Look up the domain name
        auto const results = resolver.resolve (url.Host, url.Port);

        // Make the connection on the IP address we get from a lookup
        auto ep = io::connect (get_lowest_layer (*ws), results);
        string host = url.Host;

        if (!SSL_set_tlsext_host_name (ws->next_layer ().native_handle (), host.c_str ()))
            throw beast::system_error (
                    beast::error_code (
                            static_cast<int>(::ERR_get_error ()),
                            io::error::get_ssl_category ()),
                    "Failed to set SNI Hostname");
        // Update the host_ string. This will provide the value of the
        // Host HTTP header during the WebSocket handshake.
        // See https://tools.ietf.org/html/rfc7230#section-5.4
        host += ':' + std::to_string (ep.port ());

        // Preform the SSL Handshke
        ws->next_layer ().handshake (io::ssl::stream_base::client);

        // Set a decorator to change the User-Agent of the handshake
        ws->set_option (beast::websocket::stream_base::decorator (
                [] (beast::websocket::request_type &req) {
                    req.set (http::field::user_agent,
                            std::string (BOOST_BEAST_VERSION_STRING) +
                            " websocket-client-coro");
                }));

        // Perform the websocket handshake
        ws->handshake (host, "/");

        ptr<socket<secure_stream>> ss {new socket<secure_stream> {ws, error_handler, closed}};

        ptr<session<const string &>> zz {static_cast<session<const string &> *>
                                         (new async::message_queue<const string &> {
                        std::static_pointer_cast<async::write_stream<const string &>> (ss)})};

        async::wait_for_message<string_view> (std::static_pointer_cast<async::read_stream<string_view>> (ss),
                                             [xx = interact (zz)] (string_view z) -> void {
                                                 return xx (z);
                                             });
    }

    void open (
        asio::io_context &io,
        const URL &url,
        HTTP::SSL *ssl,
        asio::error_handler error_handler,
        close_handler closed,
        interaction<string_view, const string &> interact) {

        if (url.Protocol != protocol::WS && url.Protocol != protocol::WSS)
            throw exception {} << "protocol " << url.Protocol << " is not websockets";
        if (!ssl && url.Protocol == protocol::WSS)
            throw exception {} << "Secure websocket requested when SSL Context not supplied";

        try {
            if (url.Protocol == protocol::WS) insecure_open (io, url, error_handler, interact, closed);
            else secure_open (io, url, *ssl, error_handler, interact, closed);
        } catch (boost::system::system_error err) {
            error_handler (err.code ());
        }
    }

    void open_secure (
        asio::io_context &io,
        const URL &url,
        HTTP::SSL &ssl,
        asio::error_handler error_handler,
        close_handler closed,
        interaction<string_view, const string &> interact) {

        if (url.Protocol != protocol::WSS) throw exception {} << "expected protocol WSS, but got " << url.Protocol;
        try {
            secure_open (io, url, ssl, error_handler, interact, closed);
        } catch (boost::system::system_error err) {
            error_handler (err.code ());
        }
    }

    void open_insecure (
        asio::io_context &io,
        const URL &url,
        asio::error_handler error_handler,
        close_handler closed,
        interaction<string_view, const string &> interact) {

        if (url.Protocol != protocol::WS) throw exception {} << "expected protocol WS, but got " << url.Protocol;
        try {
            insecure_open (io, url, error_handler, interact, closed);
        } catch (boost::system::system_error err) {
            error_handler (err.code ());
        }
    }

}

