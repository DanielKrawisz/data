// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/HTTP.hpp>
#include <data/net/REST.hpp>

namespace data::net::HTTP {

    namespace {
        
        std::string fromRange (const UriTextRangeA & rng)
        {
            return std::string (rng.first, rng.afterLast);
        }

        std::string fromList (UriPathSegmentA * xs, const std::string & delim)
        {
            UriPathSegmentStructA * head (xs);
            std::string accum;

            while (head)
            {
                accum += delim + fromRange (head->text);
                head = head->next;
            }

            return accum;
        }

        template<class SyncReadStream>
        boost::beast::http::response<boost::beast::http::dynamic_body> http_request (
            SyncReadStream& stream, 
            std::string hostname, 
            HTTP::method verb, 
            string path,
            map<HTTP::header, string> headers, string body, int redirects) {
            
            boost::beast::http::request<boost::beast::http::string_body> req (verb, path.c_str (), 11);

            req.set (HTTP::header::host, hostname.c_str ());
            req.set (HTTP::header::user_agent, BOOST_BEAST_VERSION_STRING);

            for (const auto &header: headers) req.set (header.Key, header.Value);
            
            req.body () = body;
            req.prepare_payload ();
            
            boost::beast::http::write (stream, req);

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

        if (req.URL.Protocol != protocol::HTTP && req.URL.Protocol != protocol::HTTPS)
            throw data::exception {} << "Invalid protocol " << req.URL.Protocol << "; expected HTTP or HTTPS";

        bool https = req.URL.Protocol == protocol::HTTPS;

        if (https && ssl == nullptr) throw data::exception {"https call with no ssl context provided"};

        auto hostname = req.URL.Host.c_str ();
        auto tempPort = string (req.URL.Port);
        auto port = tempPort!="" ? tempPort.c_str () : req.URL.Protocol;

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
            if (connect_error) throw data::exception {} << "Failed to connect to " << req.URL << connect_error;

            stream.handshake (asio::ssl::stream_base::client);

            res = http_request (stream, req.URL.Host, req.Method, req.URL.Path, req.Headers, req.Body, redirects);
        } else {
            boost::beast::tcp_stream stream (io);

            auto const results = resolver.resolve (hostname, port);

            stream.connect (results, connect_error);
            if (connect_error) throw data::exception {} << "Failed to connect to " << req.URL << connect_error;

            res = http_request (stream, req.URL.Host, req.Method, req.URL.Path, req.Headers, req.Body, redirects);
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

        map<header, string> response_headers {};
        for (const auto &field : res) response_headers =
            data::insert (response_headers, data::entry<header, string> {field.name (), std::string {field.value ()}});
        return response {res.base ().result (), response_headers, boost::beast::buffers_to_string (res.body ().data ())};

    }

}
