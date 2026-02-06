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
#include <data/dispatch.hpp>
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

    writer<byte> &operator << (writer<byte> &, const request &);
    writer<byte> &operator << (writer<byte> &, const response &);

    using SSL = asio::ssl::context;

    struct header : ASCII {
        using enum boost::beast::http::field;
        using ASCII::ASCII;
        header (boost::beast::http::field);

        bool operator == (const header &h) const {
            return case_insensitive_equal (*this, h);
        }

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

            // Raw JavaScript code.
            application_javascript,

            image_png,

            image_jpeg,

            something_else
        };

        using ASCII::ASCII;
        content (type);

        bool operator == (type) const;
    };

    // NOTE: we only support version 1.1
    enum version {
        version_1,
        version_1_1,
        version_2,
        version_3
    };

    struct message {
        dispatch<header, ASCII> Headers {};
        bytes Body;

        maybe<content> content_type () const;

        message (dispatch<header, ASCII> headers = {}, bytes body = {}):
            Headers {headers}, Body {body} {}
    };

    struct request : message {
        method Method;

        // the remaining part of the url starting with the path.
        net::target Target;

        request (method m, const net::target &targ, dispatch<header, ASCII> headers = {}, bytes body = {}):
            message {headers, body}, Method {m}, Target {targ} {}

        struct make {
            operator request () const;

            make () {}

            make method (const HTTP::method &) const;
            make target (const target &) const;
            make path (const net::path &) const;
            make query (const ASCII &) const;
            make query_map (dispatch<UTF8, UTF8>) const;
            make fragment (const UTF8 &) const;
            make host (const UTF8 &) const;
            make user_agent (const ASCII &) const;
            make authorization (const ASCII &) const;

            make body (const bytes &, const content &content_type = "application/octet-stream") const;
            make body (const JSON &j, const content &content_type = "application/json") const;
            make body (const std::string &u, const content &content_type = "text/plain") const;

            make add_headers (dispatch<HTTP::header, ASCII>) const;

        private:
            version Version {version_1_1};
            using pctstr = encoding::percent::string;

            maybe<HTTP::method> Method;

            target::make Target;

            dispatch<HTTP::header, ASCII> Headers;

            maybe<bytes> Body;
        };

        // host is required.
        bool valid () const;

        domain_name host () const;

        operator bytes () const {
            std::stringstream ss;
            ss << *this;
            return bytes (string (ss.str ()));
        }
    };

    struct response : message {
        status Status {0};

        response () {}
        response (status x, dispatch<header, ASCII> headers = {}, bytes body = {}):
            message {headers, body}, Status {x} {}

        operator bytes () const {
            std::stringstream ss;
            ss << *this;
            return bytes (string (ss.str ()));
        }
    };

    struct stream : net::stream<response, const request &> {
        awaitable<response> request (const HTTP::request &req) {
            co_await this->send (req);
            co_return co_await this->receive ();
        }

        virtual ~stream () {}
    };

    // thread safe.
    // we only support 1.1 now.
    awaitable<ptr<stream>> connect (version, const authority &host_or_endpoint, SSL * = nullptr);

    // an exception class provided for the user, not thrown by anything here.
    struct exception : data::exception {
        request Request;
        maybe<response> Response;

        exception (const request &req, const response &res, const string &w) :
        Request {req}, Response {res} {
            *this << w;
        }

        exception (const request &req, const string &w) :
        Request {req}, Response {} {
            *this << w;
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

    request::make inline request::make::body (const JSON &j, const content &content_type) const {
        return body (bytes (string {j.dump ()}), content_type);
    }

    request::make inline request::make::body (const std::string &u, const content &content_type) const {
        return body (bytes (string (u)), content_type);
    }

    writer<byte> inline &operator << (writer<byte> &w, const request &r) {
        return w << bytes (r);
    }

    writer<byte> inline &operator << (writer<byte> &w, const response &r) {
        return w << bytes (r);
    }

}

#endif 
