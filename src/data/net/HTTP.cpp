// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/beast/http.hpp>
#include <data/io/exception.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>

namespace data::net::HTTP {

    std::ostream &operator << (std::ostream &o, const request &r) {
        return o << beast::to (r);
    }

    std::ostream &operator << (std::ostream &o, const response &r) {
        return o << "HTTP response {status: " << r.Status << ", headers: " << r.Headers << ", body: " << r.Body << "}";
    }

    list<ASCII> get_headers (dispatch<header, ASCII> x, header n) {
        list<ASCII> z;
        for (const auto &[h, v] : x) if (h == n) z <<= v;
        return z;
    }

    ASCII write_content_type (content::type x) {
        switch (x) {
            case content::text_plain: return "text/plain";
            case content::text_html: return "text/html";
            case content::application_json: return "application/json";
            case content::application_x_www_form_urlencoded: return "application/x-www-form-urlencoded";
            case content::multipart_form_data: return "multipart/form-data";
            case content::application_octet_stream: return "application/octet-stream";
            case content::application_xml: return "application/xml";
            case content::application_javascript: return "application/javascript";
            case content::image_png: return "image/png";
            case content::image_jpeg: return "image/jpeg";
            default: throw data::exception {} << "unknown content type";
        }
    }

    content::content (content::type x): ASCII {write_content_type (x)} {}

    request::make::operator request () const {
        if (Version != version_1_1) throw data::exception {} << "Only version 1.1 is supported.";

        if (!bool (Method)) throw data::exception {} << "Method is not set";
        if (!bool (Target) && !bool (Path)) throw data::exception {} << "Path is not set";

        if (bool (Body)) {
            for (auto &[h, setting] : Headers) if (h == header::content_type) goto content_type_set;
            throw data::exception {} << "there is a body but content-type is not set";
        }
        content_type_set:

        // check for host, required in version 1.1.
        if (Version == version_1_1) {
            for (auto &[h, setting] : Headers) if (h == header::host) goto host_set;
            throw data::exception {} << "Host is not set, required in version 1.1";
        }
        host_set:

        net::target targ;

        if (bool (Target)) {
            targ = *Target;
        } else {
            std::stringstream target_stream;
            target_stream << static_cast<std::string> (*Path);
            if (bool (Query)) target_stream << "?" << static_cast<std::string> (*Query);
            if (bool (Fragment)) target_stream << "#" << static_cast<std::string> (*Fragment);
            targ = net::target {target_stream.str ()};
        }

        if (!targ.valid () || targ == net::target {}) throw data::exception {} << "invalid target " << targ;

        return request {*Method, targ, Headers, bool (Body) ? *Body : bytes {}};

    }

    maybe<content> message::content_type () const {
        for (auto [h, val]: Headers) if (h == header::content_type) return content {val};
        return {};
    }

    request::make request::make::method (const HTTP::method &m) const {
        if (bool (Method)) throw data::exception {"method already set"};
        auto r = *this;
        r.Method = m;
        return r;
    }

    request::make request::make::target (const net::target &p) const {
        if (bool (Target) || bool (Path) || bool (Query) || bool (Fragment)) throw data::exception {"target already set"};
        auto r = *this;
        r.Target = p;
        return r;
    }

    request::make request::make::path (const net::path &p) const {
        if (bool (Path) || bool (Target)) throw data::exception {"path already set"};
        auto r = *this;
        r.Path = p;
        return r;
    }

    request::make request::make::query (const ASCII &p) const {
        if (bool (Query) || bool (Target)) throw data::exception {"query already set"};
        auto r = *this;
        r.Query = encoding::percent::encode (p);
        return r;
    }

    request::make request::make::fragment (const UTF8 &p) const {
        if (bool (Query) || bool (Target)) throw data::exception {"query already set"};
        auto r = *this;
        r.Query = encoding::percent::encode (p);
        return r;
    }

    request::make request::make::add_headers (dispatch<HTTP::header, ASCII> h) const {
        auto r = *this;
        r.Headers = r.Headers + h;
        return r;
    }

    request::make request::make::body (const bytes &b, const content &content_type) const {
        if (bool (Body)) throw data::exception {"body is already set"};

        auto r = add_headers ({entry<header, ASCII> {header::content_type, content_type}});

        r.Body = b;
        return r;
    }

    request::make request::make::host (const UTF8 &u) const {
        return add_headers ({entry<header, ASCII> {header::host, encoding::percent::encode (u)}});
    }

    request::make request::make::authorization (const ASCII &a) const {
        return add_headers ({entry<header, ASCII> {header::authorization, a}});
    }


}

namespace data::net::HTTP::beast {
    template <typename beast_stream>
    awaitable<void> connect (beast_stream &x, const authority &host_or_endpoint, bool use_ssl) {

        get_lowest_layer (x).expires_after (std::chrono::seconds (30));

        maybe<IP::TCP::endpoint> maybe_endpoint = host_or_endpoint.endpoint ();

        if (bool (maybe_endpoint)) {
            cross<asio::ip::tcp::endpoint> results {{asio::ip::tcp::endpoint (*maybe_endpoint)}};

            co_await get_lowest_layer (x).async_connect (results, asio::use_awaitable);
        } else {
            maybe<domain_name> maybe_host = host_or_endpoint.host ();

            if (!bool (maybe_host)) throw data::exception {} << "could not read endpoint or host from " << host_or_endpoint;

            maybe<ASCII> maybe_port = host_or_endpoint.port ();
            ASCII port = bool (maybe_port) ? *maybe_port : bool (use_ssl) ? ASCII {"https"} : ASCII {"HTTP"};

            asio::ip::tcp::resolver resolver {asio::make_strand (co_await asio::this_coro::executor)};
            auto results = co_await resolver.async_resolve (*maybe_host, port, asio::use_awaitable);

            co_await get_lowest_layer (x).async_connect (results, asio::use_awaitable);
        }
    }

    awaitable<void> handshake (ssl_stream<tcp_stream> &x) {
        co_await x.async_handshake (asio::ssl::stream_base::client, asio::use_awaitable);
    }

    template <typename beast_stream>
    struct session final : HTTP::stream {
        beast_stream Stream;
        flat_buffer Buffer; // (Must persist between reads)

        session (beast_stream &&x): Stream {std::move (x)} {}

        awaitable<void> start (const authority &host_or_endpoint);

        virtual bool closed () final override {
            return !get_lowest_layer (Stream).socket ().is_open ();
        }

        virtual void close () final override {
            boost::system::error_code ec;
            get_lowest_layer (Stream).socket ().shutdown (boost::asio::ip::tcp::socket::shutdown_both, ec);
            get_lowest_layer (Stream).socket ().close (ec);
        }

        virtual awaitable<void> send (const HTTP::request &req) final override {
            boost::system::error_code ec;
            co_await http::async_write (Stream, to (req), asio::redirect_error (asio::use_awaitable, ec));
            if (ec) throw asio::exception {ec};
        }

        virtual awaitable<HTTP::response> receive () final override {
            boost::system::error_code ec;
            response res;
            co_await http::async_read (Stream, Buffer, res, asio::redirect_error (asio::use_awaitable, ec));

            if (ec) {
                close ();
                throw asio::exception {ec};
            }

            co_return HTTP::response {from (res)};
        }
    };

    template <> awaitable<void> session<tcp_stream>::start (const authority &host_or_endpoint) {
        co_await connect (Stream, host_or_endpoint, false);
    }

    template <> awaitable<void> session<ssl_stream<tcp_stream>>::start (const authority &host_or_endpoint) {
        co_await connect (Stream, host_or_endpoint, true);
        co_await handshake (Stream);
    }
}

namespace data::net::HTTP {

    // async HTTP call
    awaitable<ptr<stream>> connect (version v, const authority &host_or_endpoint, SSL *ssl) {
        if (v != version_1_1) throw data::exception {} << "Only version 1.1 supported.";

        if (bool (ssl)) {
            auto z = new beast::session<boost::beast::ssl_stream<boost::beast::tcp_stream>>
                {std::move (beast::ssl_stream<boost::beast::tcp_stream> {beast::net::make_strand (co_await asio::this_coro::executor), *ssl})};
            co_await z->start (host_or_endpoint);
            co_return ptr<stream> {static_cast<stream *> (z)};
        } else {
            auto z = new beast::session<beast::tcp_stream>
                {std::move (boost::beast::tcp_stream {beast::net::make_strand (co_await asio::this_coro::executor)})};
            co_await z->start (host_or_endpoint);
            co_return ptr<stream> {static_cast<stream *> (z)};
        }
    }

}
