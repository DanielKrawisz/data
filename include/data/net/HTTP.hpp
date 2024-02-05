// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP
#define DATA_NET_HTTP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <data/tools.hpp>
#include <data/cross.hpp>
#include <data/net/asio/session.hpp>
#include <data/net/URL.hpp>
#include <map>

namespace data::net::HTTP {

    struct request;
    struct response;

    std::ostream &operator << (std::ostream &, const request &);
    std::istream &operator >> (std::istream &, request &);

    std::ostream &operator << (std::ostream &, const response &);
    std::istream &operator >> (std::istream &, response &);

    using SSL = asio::ssl::context;

    // make an HTTP call (blocking).
    // error is thrown.
    response call (const request &, SSL * = nullptr, uint32 redirects = 10);

    // async HTTP call
    // Once this is done, call run () on the io_context to actually make the HTTP call and handle the response.
    void call (asio::io_context &, asio::error_handler, handler<const response &>, const request &, SSL * = nullptr);

    using header = boost::beast::http::field;
    using method = boost::beast::http::verb;
    using status = boost::beast::http::status;

    struct request {
        method Method;
        net::URL URL;
        map<header, ASCII> Headers;
        string Body;
        string UserAgent;

        request (
            method method,
            net::URL url,
            map<header, ASCII> headers = {},
            string body = {},
            string user_agent = BOOST_BEAST_VERSION_STRING) :
            Method {method}, URL {url}, Headers {headers}, Body {body}, UserAgent {user_agent} {}

        bool valid () const;
        ASCII target () const;

    };

    struct response {
        status Status;
        map<header, ASCII> Headers;
        string Body;
    };

    struct exception : std::exception {
        request Request;
        response Response;
        string Error;

        exception (const request &req, const response &res, const string &w) :
            Request {req}, Response {res}, Error {w} {}

        const char *what () const noexcept override {
            return Error.c_str ();
        }
    };

    bool inline request::valid () const {
        auto proto = URL.protocol ();
        return proto == protocol::HTTP || proto == protocol::HTTPS;
    }

    ASCII inline request::target () const {
        return ASCII {encoding::percent::URI::target (URL)};
    }

}

#endif 
