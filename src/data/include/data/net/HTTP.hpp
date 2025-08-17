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
#include <data/async.hpp>
#include <data/tools.hpp>
#include <data/cross.hpp>
#include <data/net/asio/stream.hpp>
#include <data/net/JSON.hpp>
#include <data/net/URL.hpp>

namespace data::net::HTTP {

    struct request;
    struct response;

    std::ostream &operator << (std::ostream &, const request &);
    std::istream &operator >> (std::istream &, request &);

    std::ostream &operator << (std::ostream &, const response &);
    std::istream &operator >> (std::istream &, response &);

    using SSL = asio::ssl::context;

    struct header : ASCII {
        using enum boost::beast::http::field;
        using ASCII::ASCII;
        header (boost::beast::http::field);
        bool operator == (boost::beast::http::field x) const {
            return *this == header {x};
        }
    };

    using method = boost::beast::http::verb;

    struct status {
        using enum boost::beast::http::status;
        boost::beast::http::status Status;

        status (boost::beast::http::status x);
        status (unsigned u);
        operator unsigned () const;

        bool operator == (boost::beast::http::status) const;
    };

    std::ostream &operator << (std::ostream &, status);

    // the body of an HTTP request must correspond to a
    // content-type header field that explains how to
    // interpret it. This is a general type for content
    // that includes common use cases as an enum.
    struct content : ASCII {
        enum type {
            // Plain UTF-8 text.
            text_plain,

            // HTML markup.
            text_html,

            // JSON-encoded data (e.g., {"name": "Alice"}).
            application_json,

            // URL-encoded key-value pairs like key1=value1&key2=value2.
            application_x_www_form_urlencoded,

            // Used for file uploads; body is split by boundary strings.
            multipart_form_data,

            // Raw binary data (e.g., file content).
            application_octet_stream,

            // XML content.
            application_xml,

            // Raw JavaScript code (rare in requests, common in responses).
            application_javascript,

            image_png,

            image_jpeg,

            something_else
        };

        using ASCII::ASCII;
        content (type);

        bool operator == (type) const;
    };

    struct request {
        method Method;

        // the remaining part of the url starting with the path.
        net::target Target;

        // additional headers.
        dispatch<header, ASCII> Headers;

        bytes Body;

        struct make {
            operator request () const;

            make () {}

            make method (const HTTP::method &) const;
            make target (const target &) const;
            make path (const net::path &) const;
            make query (const ASCII &) const;
            make fragment (const UTF8 &) const;
            make host (const UTF8 &) const;
            make user_agent (const ASCII &) const;
            make authorization (const ASCII &) const;

            make body (const bytes &, const content &content_type = "application/octet-stream") const;
            make body (const JSON &j) const;
            make body (const std::string &u) const;

            make add_headers (dispatch<HTTP::header, ASCII>) const;

        private:
            using pctstr = encoding::percent::string;

            maybe<HTTP::method> Method;

            maybe<pctstr> Path {};
            maybe<pctstr> Query {};
            maybe<pctstr> Fragment {};

            dispatch<HTTP::header, ASCII> Headers;

            maybe<bytes> Body;
        };

        // host is required.
        bool valid () const;

        domain_name host () const;

        maybe<content> content_type () const;

    };

    struct response {
        status Status {0};
        dispatch<header, ASCII> Headers {};
        bytes Body {};

        response () {}
        response (status x, dispatch<header, ASCII> headers = {}, bytes body = {}):
            Status {x}, Headers {headers}, Body {body} {}

        maybe<content> content_type () const;
    };

    struct session {
        virtual bool is_open () = 0;
        virtual void close () = 0;
        virtual awaitable<response> request (const HTTP::request &) = 0;
        virtual ~session () {}
    };

    enum version {
        version_1_1,
        version_2,
        version_3
    };

    // we only support 1.1 now.
    awaitable<ptr<session>> connect (version, const authority &host_or_endpoint, SSL * = nullptr);

    struct exception : std::exception {
        request Request;
        maybe<response> Response;
        string Error;

        exception (const request &req, const response &res, const string &w) :
            Request {req}, Response {res}, Error {w} {}

        exception (const request &req, const string &w) :
            Request {req}, Response {}, Error {w} {}

        const char *what () const noexcept override {
            return Error.c_str ();
        }
    };

    inline status::status (boost::beast::http::status x) : Status {x} {}

    inline status::status (unsigned u) : Status {u} {}

    inline status::operator unsigned () const {
        return static_cast<unsigned> (Status);
    }

    inline header::header (boost::beast::http::field x) : ASCII {std::string {boost::beast::http::to_string (x)}} {}

    bool inline status::operator == (boost::beast::http::status x) const {
        return Status == x;
    }

    std::ostream inline &operator << (std::ostream &o, status x) {
        return o << x.Status;
    }

    bool inline content::operator == (type ttt) const {
        return static_cast<ASCII> (*this) == static_cast<ASCII> (content (ttt));
    }

    request::make inline request::make::body (const JSON &j) const {
        return body (bytes (string {j.dump ()}), "application/json");
    }

    request::make inline request::make::body (const std::string &u) const {
        return body (bytes (string (u)), "text/plain");
    }

}

#endif 
