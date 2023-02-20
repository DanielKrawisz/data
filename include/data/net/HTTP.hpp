// Copyright (c) 2021 Katrina Knight
// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP
#define DATA_NET_HTTP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
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

    // make an HTTP call (blocking)
    response call (const request &, SSL * = nullptr, uint32 redirects = 10);

    // async HTTP call
    void call (asio::io_context &, handler<const response &>, const request &, SSL * = nullptr);

    using header = boost::beast::http::field;
    using method = boost::beast::http::verb;
    using status = boost::beast::http::status;

    struct request {
        method Method;
        net::URL URL;
        map<header, string> Headers;
        string Body;

        request (
            method method,
            net::URL url,
            map<header, string> headers = {},
            string body = {}) : Method {method}, URL {url}, Headers {headers}, Body {body} {}

    };

    struct response {
        status Status;
        map<header, string> Headers;
        string Body;
    };

    struct exception : std::exception {
        request Request;
        response Response;
        string Problem;
        exception (const request &req, const response &res, const string &p) :
            Request {req}, Response {res}, Problem {p} {}

        const char *what () const noexcept override {
            return Problem.c_str ();
        }
    };

}

#endif 
