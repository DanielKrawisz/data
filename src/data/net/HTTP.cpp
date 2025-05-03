// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/beast/beast.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>

namespace data::net::HTTP {

    std::ostream &operator << (std::ostream &o, const request &r) {
        return o << beast::from (r);
    }

    std::ostream &operator << (std::ostream &o, const response &r) {
        return o << "HTTP response {status: " << r.Status << ", headers: " << r.Headers << ", body: " << r.Body << "}";
    }

}

namespace data::net::HTTP::beast {
    template <typename stream, typename base>
    struct session : public std::enable_shared_from_this<base> {
        asio::ip::tcp::resolver *Resolver;
        stream *Stream;
        asio::error_handler OnError;
        handler<const HTTP::response &> OnResponse;
        HTTP::request Request;

        http::response<http::string_body> res_;
        flat_buffer buffer_; // (Must persist between reads)

        session (
            asio::ip::tcp::resolver *rr, stream *ss,
            asio::error_handler error_handler,
            handler<const HTTP::response &> response_handler,
            const HTTP::request &req): Resolver {rr}, Stream {ss}, OnError {error_handler}, OnResponse {response_handler}, Request {req} {}

        // Start the asynchronous operation
        void run ();

        void on_resolve (beast::error_code ec, tcp::resolver::results_type results);

        virtual void on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type) = 0;

        void on_write (beast::error_code ec, std::size_t bytes_transferred);

        virtual void on_read (beast::error_code ec, std::size_t bytes_transferred) = 0;

        virtual ~session () {
            delete Resolver;
            delete Stream;
        }

    };

    // Objects are constructed with a strand to
    // ensure that handlers do not execute concurrently.
    struct http_session final : session<tcp_stream, http_session> {
        http_session (
            exec ex,
            asio::error_handler error_handler,
            handler<const HTTP::response &> response_handler,
            const HTTP::request &req) :
            session<tcp_stream, http_session> {
                new asio::ip::tcp::resolver {net::make_strand (ex)},
                new tcp_stream {net::make_strand (ex)},
                error_handler, response_handler, req} {}

        void on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type) final override;
        void on_read (beast::error_code ec, std::size_t bytes_transferred) final override;
    };

    struct https_session final : session<ssl_stream<tcp_stream>, https_session> {
        https_session (
            exec ex, SSL &ssl,
            asio::error_handler error_handler,
            handler<const HTTP::response &> response_handler,
            const HTTP::request &req) :
            session<ssl_stream<tcp_stream>, https_session> {
                new asio::ip::tcp::resolver {net::make_strand (ex)},
                new ssl_stream<tcp_stream> {net::make_strand (ex), ssl},
                error_handler, response_handler, req} {

            auto host = Request.URL.domain_name ();
            if (!bool (host)) return;

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name (Stream->native_handle (), host->c_str ())) {
                beast::error_code ec {static_cast<int> (::ERR_get_error ()), net::error::get_ssl_category ()};
                std::cerr << ec.message () << "\n";
                return;
            }
        }

        void on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type) final override;
        void on_handshake (beast::error_code ec);
        void on_read (beast::error_code ec, std::size_t bytes_transferred) final override;
        void on_shutdown (beast::error_code ec);
    };

    // Start the asynchronous operation
    template <typename stream, typename base>
    void session<stream, base>::run () {
        auto host = Request.URL.domain_name ();
        auto endpoint = Request.URL.endpoint ();

        if (bool (host)) { // if we have a host, use the resolver
            // Look up the domain name
            Resolver->async_resolve (host->c_str (), Request.URL.port_DNS ().c_str (),
                beast::bind_front_handler (&session::on_resolve, this->shared_from_this ()));
        } else {           // otherwise just call directly.
            // Set a timeout on the operation
            get_lowest_layer (*Stream).expires_after (std::chrono::seconds (30));

            // Make the connection on the IP address we get from a lookup
            get_lowest_layer (*Stream).async_connect (cross<asio::ip::tcp::endpoint> {{asio::ip::tcp::endpoint (*endpoint)}},
                beast::bind_front_handler (&session::on_connect, this->shared_from_this ()));
        }
    }

    template <typename stream, typename base>
    void session<stream, base>::on_resolve (beast::error_code ec, tcp::resolver::results_type results) {
        if (ec) return OnError (ec);

        // Set a timeout on the operation
        get_lowest_layer (*Stream).expires_after (std::chrono::seconds (30));

        // Make the connection on the IP address we get from a lookup
        get_lowest_layer (*Stream).async_connect (results, beast::bind_front_handler (&session::on_connect, this->shared_from_this ()));
    }

    template <typename stream, typename base>
    void session<stream, base>::on_write (beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused (bytes_transferred);

        if (ec) return this->OnError (ec);

        // Receive the HTTP response
        http::async_read (*Stream, buffer_, res_, beast::bind_front_handler (&session::on_read, this->shared_from_this ()));
    }

    void http_session::on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
        if (ec) return this->OnError (ec);

        // Set a timeout on the operation
        get_lowest_layer (*Stream).expires_after (std::chrono::seconds (30));

        // Send the HTTP request to the remote host
        http::async_write (*Stream, from (Request), beast::bind_front_handler (&session::on_write, this->shared_from_this ()));
    }

    void https_session::on_connect (beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
        if (ec) return this->OnError (ec);

        // Perform the SSL handshake
        Stream->async_handshake (ssl::stream_base::client, beast::bind_front_handler (&https_session::on_handshake, this->shared_from_this ()));
    }

    void https_session::on_handshake (beast::error_code ec) {
        if (ec) return this->OnError (ec);

        // Set a timeout on the operation
        get_lowest_layer (*Stream).expires_after (std::chrono::seconds (30));

        // Send the HTTP request to the remote host
        http::async_write (*Stream, from (Request), beast::bind_front_handler (&session::on_write, this->shared_from_this ()));
    }

    void http_session::on_read (beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused (bytes_transferred);

        if (ec) return this->OnError (ec);

        // return the message
        OnResponse (to (res_));

        // Gracefully close the socket
        get_lowest_layer (*Stream).socket ().shutdown (tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes so don't bother reporting it.
        if (ec && ec != beast::errc::not_connected)
            return OnError (ec);

        // If we get here then the connection is closed gracefully
    }

    void https_session::on_read (beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused (bytes_transferred);

        if (ec) return this->OnError (ec);

        // return the message
        OnResponse (to (res_));

        // Set a timeout on the operation
        get_lowest_layer (*Stream).expires_after (std::chrono::seconds (30));

        // Gracefully close the stream
        Stream->async_shutdown (beast::bind_front_handler (&https_session::on_shutdown, this->shared_from_this ()));

    }

    void https_session::on_shutdown (beast::error_code ec) {
        if (ec == net::error::eof) {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec = {};
        }

        if (ec) return this->OnError (ec);

        // If we get here then the connection is closed gracefully
    }
}

namespace data::net::HTTP {

    // async HTTP call
    void call (exec ex, asio::error_handler err, handler<const response &> on_response, const request &req, SSL *ssl) {

        // first we check that the protocol is valid.
        auto proto = req.URL.protocol ();

        if (!req.valid ()) throw data::exception {} << "Invalid protocol " << proto << "; expected HTTP or HTTPS";

        bool https = proto == protocol::HTTPS;

        if (https && ssl == nullptr) throw data::exception {"https call with no ssl context provided"};

        // next we check on whether we need to use DNS.
        if (!bool (req.URL.domain_name ()) && ! bool (req.URL.endpoint ()))
            throw data::exception {"Neither tcp endpoint nor host can be reconstructed from the URL"};

        if (https) std::make_shared<beast::https_session> (ex, *ssl, err, on_response, req)->run ();
        else std::make_shared<beast::http_session> (ex, err, on_response, req)->run ();
    }

    awaitable<response> call_once (const request &req, SSL *ssl) {
        // Get the executor from this coroutine (needed for handler to resume)
        exec ex = co_await asio::this_coro::executor;

        co_return co_await asio::async_initiate<decltype (asio::use_awaitable), void (response)> (
            [&req, ssl, ex] (auto &&handler) {
                call (ex, [&handler] (asio::error err) {
                    throw data::exception {} << "network error " << err;
                }, [&handler] (const response &r) {
                    handler (r);
                }, req, ssl);
            },
            // this tells Boost we're using coroutines
            asio::use_awaitable);
    }

    awaitable<response> call (const request &r, SSL *ssl, uint32 redirects) {
        request req = r;
        response res;
        // keep trying with more redirects
        for (uint32 redirect = 0; redirect < redirects; redirect++) {
            res = co_await call_once (req, ssl);

            if (static_cast<unsigned int> (res.Status) < 300 || static_cast<unsigned int> (res.Status) >= 400) co_return res;

            URL u (res.Headers [boost::beast::http::field::location]);
            if (!u.valid ()) throw exception {req, res, "could not read redirect URL"};
            req.URL = u;
        }

        throw exception {req, res, "too many redirects"};

    }

}
