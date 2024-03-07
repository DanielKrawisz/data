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

namespace data::net::HTTP {

    namespace beast {
        using request = boost::beast::http::request<boost::beast::http::string_body>;

        request from (const HTTP::request &r) {
            boost::beast::http::request<boost::beast::http::string_body> req (r.Method, r.target ().c_str (), 11);

            req.set (HTTP::header::host, r.URL.domain_name ()->c_str ());
            req.set (HTTP::header::user_agent, r.UserAgent);

            for (const auto &header: r.Headers) req.set (header.Key, header.Value);

            req.body () = r.Body;
            req.prepare_payload ();
            return req;
        }
    };

    std::ostream &operator << (std::ostream &o, const request &r) {
        return o << beast::from (r);
    }

    namespace {

        template<class SyncReadStream>
        boost::beast::http::response<boost::beast::http::dynamic_body> http_request (
            SyncReadStream& stream, 
            const request &req) {
            
            boost::beast::http::write (stream, beast::from (req));

            boost::beast::flat_buffer buffer;
            boost::beast::http::response<boost::beast::http::dynamic_body> res;
            boost::beast::error_code ec;

            try {
                boost::beast::http::read (stream, buffer, res, ec);
            } catch (boost::exception &ex) {}

            return res;
            
        }
        
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

        boost::beast::http::response<boost::beast::http::dynamic_body> res;

        asio::io_context io {};

        asio::ip::tcp::resolver resolver (io);

        asio::error_code connect_error {};

        if (https) {
            boost::beast::ssl_stream<boost::beast::tcp_stream> stream (io, *ssl);

            // Set SNI Hostname (many hosts need this to handshake successfully)
            if (!SSL_set_tlsext_host_name (stream.native_handle (), hostname)) {
                boost::beast::error_code ec {static_cast<int> (::ERR_get_error ()),
                    asio::error::get_ssl_category ()};
                throw boost::beast::system_error {ec};
            }

            auto const results = resolver.resolve (hostname, port);

            boost::beast::get_lowest_layer (stream).connect (results, connect_error);
            if (connect_error) throw data::exception {} << "Failed to connect to " << req.URL << "; error " << connect_error;

            stream.handshake (asio::ssl::stream_base::client);

            res = http_request (stream, req);
        } else {
            boost::beast::tcp_stream stream (io);

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
        map<header, ASCII> response_headers {};
        for (const auto &field : res)
            if (field.name () != header::unknown) response_headers =
                data::insert (response_headers, data::entry<header, ASCII> {field.name (), ASCII {std::string {field.value ()}}});

        return response {res.base ().result (), response_headers, boost::beast::buffers_to_string (res.body ().data ())};

    }

    // async HTTP call
    void call (asio::io_context &, handler<const response &>, const request &, SSL *) {
        throw data::method::unimplemented {"HTTP::call async"};
    }

}
