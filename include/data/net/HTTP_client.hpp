// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_CLIENT
#define DATA_NET_HTTP_CLIENT

#include <data/net/REST.hpp>
#include <data/tools/rate_limiter.hpp>
#include <stdlib.h>

namespace data::net::HTTP {
    struct client {

        ptr<HTTP::SSL> SSL;
        
        HTTP::REST REST;
        
        tools::rate_limiter Rate;
        
        client (const HTTP::REST &rest, tools::rate_limiter rate = {});
        client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate = {});

        // blocking methods
        response operator () (const request &r);
        response GET (string path, list<entry<string, string>> params = {});
        response POST (string path, map<header, string> headers, string body);
        response POST_form_data (string path, map<string, string> form_data = {});

        // async methods
        void operator () (asio::io_context &, handler<const response &>, const request &r);
        void GET (asio::io_context &, handler<const response &>, string path, list<entry<string, string>> params = {});
        void POST (asio::io_context &, handler<const response &>, string path, map<header, string> headers, string body);
        void POST_form_data (asio::io_context &, handler<const response &>, string path, map<string, string> form_data = {});
        
    };
        
    inline client::client (const HTTP::REST &rest, tools::rate_limiter rate) :
        client {std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline client::client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate) :
        SSL {ssl}, REST {rest}, Rate {rate} {}
    
    response inline client::operator () (const request &r) {
        auto wait = Rate.getTime ();
        asio::io_context io {};
        if (wait != 0) asio::steady_timer {io, asio::chrono::seconds (wait)}.wait ();
        return HTTP::call (r, SSL.get ());
    }
    
    response inline client::GET (string path, list<entry<string, string>> params) {
        return (*this) (REST.GET (path, params));
    }

    void inline client::GET (
        asio::io_context &io,
        handler<const response &> on_receive,
        string path,
        list<entry<string, string>> params) {
        return (*this) (io, on_receive, REST.GET (path, params));
    }

    response inline client::POST (string path, map<header, string> headers, string body) {
        return (*this) (REST.POST (path, headers, body));
    }

    void inline client::POST (
        asio::io_context &io,
        handler<const response &> on_receive,
        string path, map<header, string> headers,
        string body) {
        return (*this) (io, on_receive, REST.POST (path, headers, body));
    }
    
    // POST form data
    response inline client::POST_form_data (string path, map<string, string> form_data) {
        return (*this) (REST.POST (path, form_data));
    }

    void inline client::POST_form_data (
        asio::io_context &io,
        handler<const response &> on_receive,
        string path,
        map<string, string> form_data) {
        return (*this) (io, on_receive, REST.POST (path, form_data));
    }
}

#endif
