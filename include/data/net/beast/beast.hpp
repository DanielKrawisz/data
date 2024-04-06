#ifndef DATA_NET_HTTP_BEAST
#define DATA_NET_HTTP_BEAST

// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>

#include <data/net/HTTP.hpp>
#include <data/net/REST.hpp>
#include <data/io/unimplemented.hpp>

namespace data::net::HTTP::beast {
    using namespace boost::beast;            // from <boost/beast.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>

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

    response from (const HTTP::response &r);

    struct header_already_exists {
        response Response;
        map<header, ASCII> operator () (map<header, ASCII> m, const header &h, const ASCII &o, const ASCII &n) const {
            throw data::exception {} << "HTTP response " << Response << " contains duplicate header " << h;
        }
    };

    // note: it is possible for a header to be known by boost::beast. In that case it gets deleted. Kind of dumb.
    HTTP::response to (const response &res) {

        map<header, ASCII> response_headers {};

        for (const auto &field : res) if (field.name () != header::unknown) response_headers = response_headers.insert
            (field.name (), ASCII {std::string {field.value ()}}, header_already_exists {res});

        return HTTP::response {res.base ().result (), response_headers, res.body ()};
    }

    HTTP::request to (const request &);
}

#endif
