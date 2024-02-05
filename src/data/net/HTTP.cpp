// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <data/net/HTTP.hpp>
#include <data/net/REST.hpp>
#include <data/io/unimplemented.hpp>

namespace data::net::HTTP::beast {
    using namespace boost::beast;            // from <boost/beast.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

    using request = boost::beast::http::request<boost::beast::http::string_body>;
    using response = boost::beast::http::response<boost::beast::http::string_body>;

    // convert to beast format
    request from (const HTTP::request &r) {
        request req (r.Method, r.target ().c_str (), 11);

        req.set (HTTP::header::host, r.URL.domain_name ()->c_str ());
        req.set (HTTP::header::user_agent, r.UserAgent);

        for (const auto &header: r.Headers) req.set (header.Key, header.Value);

        req.body () = r.Body;
        req.prepare_payload ();
        return req;
    }

    HTTP::response to (const http::response<http::string_body>);

    response from (const HTTP::response &r);

    // note: it is possible for a header to be known by boost::beast. In that case it gets deleted. Kind of dumb.
    HTTP::response to (const http::response<http::string_body> res) {
        map<header, ASCII> response_headers {};
        for (const auto &field : res) response_headers =
            data::insert (response_headers, data::entry<header, ASCII> {field.name (), ASCII {std::string {field.value ()}}});
        return HTTP::response {res.base ().result (), response_headers, res.body ()};
    }

}

namespace data::net::HTTP {

    std::ostream &operator << (std::ostream &o, const request &r) {
        return o << beast::from (r);
    }

    std::ostream &operator << (std::ostream &o, const response &r) {
        return o << "HTTP response {status: " << r.Status << ", headers: " << r.Headers << ", body: " << r.Body << "}";
    }

    template<class SyncReadStream>
    beast::response http_request (
        SyncReadStream& stream,
        const request &req) {

        beast::http::write (stream, beast::from (req));

        beast::flat_buffer buffer;
        beast::response res;
        beast::error_code ec;

        try {
            beast::http::read (stream, buffer, res, ec);
        } catch (boost::exception &ex) {}

        return res;

    }

    response call (const request &req, SSL *ssl, uint32 redirects) {

        auto proto = req.URL.protocol ();

        if (!req.valid ()) throw data::exception {} << "Invalid protocol " << proto << "; expected HTTP or HTTPS";

        bool https = proto == protocol::HTTPS;

        if (https && ssl == nullptr) throw data::exception {"https call with no ssl context provided"};

        auto host = req.URL.domain_name ();
        if (!bool (host)) throw data::exception {"No host provided in the URL."};

        auto hostname = host->c_str ();
        auto port = req.URL.port_DNS ().c_str ();

        beast::response res;

        asio::io_context io {};

        asio::ip::tcp::resolver resolver (io);

        asio::error_code connect_error {};

        if (https) {
            beast::ssl_stream<beast::tcp_stream> stream (io, *ssl);

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name (stream.native_handle (), hostname)) {
                beast::error_code ec {static_cast<int> (::ERR_get_error ()),
                    asio::error::get_ssl_category ()};
                throw beast::system_error {ec};
            }

            auto const results = resolver.resolve (hostname, port);

            beast::get_lowest_layer (stream).connect (results, connect_error);
            if (connect_error) throw data::exception {} << "Failed to connect to " << req.URL << "; error " << connect_error;

            stream.handshake (asio::ssl::stream_base::client);

            res = http_request (stream, req);
        } else {
            beast::tcp_stream stream (io);

            auto const results = resolver.resolve (hostname, port);

            stream.connect (results, connect_error);
            if (connect_error) throw data::exception {} << "Failed to connect to " << req.URL << "; error " << connect_error;

            res = http_request (stream, req);
        }
/*
        if (static_cast<unsigned int> (res.base ().result ()) >= 300 && static_cast<unsigned int> (res.base ().result ()) < 400) {
            std::string loc = res.base ()["Location"].to_string ();
            if (!loc.empty ()) {
                UriUriA uri;
                const char **errorPos;
                if (uriParseSingleUriA (&uri, loc.c_str (), errorPos))
                    throw data::exception {"could not read redirect url"};

                if (redirects == 0) throw data::exception {"too many redirects"};

                return call (request {req.Method, URL {fromRange (uri.portText), fromRange (uri.hostText),
                    fromList (uri.pathHead, "/") + fromRange (uri.fragment)}, req.Headers, req.Body}, ssl, redirects - 1);
            }
        }*/

        // note: it is possible for a header to be known by boost::beast. In that case it gets deleted. Kind of dumb.
        return beast::to (res);

    }
}

namespace data::net::HTTP::beast {
    class session : public std::enable_shared_from_this<session> {
        asio::ip::tcp::resolver Resolver;
        tcp_stream Stream;
        handler<const error &> OnError;
        handler<const HTTP::response &> OnResponse;
        HTTP::request Request;

        response res_;
        flat_buffer buffer_; // (Must persist between reads)

    public:
        session (
            asio::io_context &io,
            handler<const error &> error_handler,
            handler<const HTTP::response &> response_handler,
            const HTTP::request &req):
            Resolver {io}, Stream {io}, OnError {error_handler}, OnResponse {response_handler}, Request {req} {}

        // Start the asynchronous operation
        void run ();

        void on_resolve (beast::error_code ec, tcp::resolver::results_type results);

        void on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type);

        void on_write (beast::error_code ec, std::size_t bytes_transferred);

        void on_read (beast::error_code ec, std::size_t bytes_transferred);

    };

    // Start the asynchronous operation
    void session::run () {
        auto host = Request.URL.domain_name ();
        auto endpoint = Request.URL.endpoint ();

        if (bool (host)) { // if we have a host, use the resolver
            // Look up the domain name
            Resolver.async_resolve (host->c_str (), Request.URL.port_DNS ().c_str (),
                beast::bind_front_handler (&session::on_resolve, shared_from_this ()));
        } else {           // otherwise just call directly.
            // Set a timeout on the operation
            Stream.expires_after (std::chrono::seconds (30));

            // Make the connection on the IP address we get from a lookup
            Stream.async_connect (cross<asio::ip::tcp::endpoint> {{asio::ip::tcp::endpoint (*endpoint)}},
                beast::bind_front_handler (&session::on_connect, shared_from_this ()));
        }
    }

    void session::on_resolve (beast::error_code ec, tcp::resolver::results_type results) {
        if (ec) return OnError (error {ec, "resolve"});

        // Set a timeout on the operation
        Stream.expires_after (std::chrono::seconds (30));

        // Make the connection on the IP address we get from a lookup
        Stream.async_connect (results, beast::bind_front_handler (&session::on_connect, shared_from_this ()));
    }

    void session::on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
        if (ec) return OnError (error {ec, "connect"});

        // Set a timeout on the operation
        Stream.expires_after (std::chrono::seconds (30));

        // Send the HTTP request to the remote host
        http::async_write (Stream, from (Request), beast::bind_front_handler (&session::on_write, shared_from_this ()));
    }

    void session::on_write (beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused (bytes_transferred);

        if (ec) return OnError (error {ec, "write"});

        // Receive the HTTP response
        http::async_read (Stream, buffer_, res_, beast::bind_front_handler (&session::on_read, shared_from_this ()));
    }

    void session::on_read (beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused (bytes_transferred);

        if (ec) return OnError (error {ec, "read"});

        // return the message
        OnResponse (to (res_));

        // Gracefully close the socket
        Stream.socket ().shutdown (tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes so don't bother reporting it.
        if (ec && ec != beast::errc::not_connected)
            return OnError (error {ec, "shutdown"});

        // If we get here then the connection is closed gracefully
    }
}

namespace data::net::HTTP {

    // async HTTP call
    void call (asio::io_context &io, handler<const error &> error_handler, handler<const response &> on_response, const request &req, SSL *ssl) {

        // first we check that the protocol is valid.

        auto proto = req.URL.protocol ();

        if (!req.valid ()) throw data::exception {} << "Invalid protocol " << proto << "; expected HTTP or HTTPS";

        bool https = proto == protocol::HTTPS;

        // TODO: does beast provide an error code for this case? We shouldn't throw exceptions in this function ideally.
        if (https && ssl == nullptr) throw data::exception {"https call with no ssl context provided"};

        if (https) throw data::exception {"cannot do async https yet"};

        // next we check on whether we need to use DNS.
        // TODO: another case where an exception is thrown instead of using the error handler.
        if (!bool (req.URL.domain_name ()) && ! bool (req.URL.endpoint ()))
            throw data::exception {"Neither tcp endpoint nor host can be reconstructed from the URL"};

        std::make_shared<beast::session> (io, error_handler, on_response, req)->run ();

    }

}
