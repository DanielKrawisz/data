// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_CLIENT
#define DATA_NET_HTTP_CLIENT

#include <data/net/REST.hpp>
#include <data/tools/rate_limiter.hpp>
#include <stdlib.h>

namespace data::net::HTTP::client {

    struct blocking {

        HTTP::REST REST;
        
        // if no SSL context is provided, it will be created.
        blocking (const HTTP::REST &rest, uint32 redirects = 10, tools::rate_limiter rate = {});
        blocking (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, uint32 redirects = 10, tools::rate_limiter rate = {});

        // blocking methods
        response operator () (const request &r);
        response GET (path path, list<entry<UTF8, UTF8>> params = {});
        response POST (path path, map<header, ASCII> headers, string body);
        response POST_form_data (path path, map<header, ASCII> form_data = {});

        struct make;

    private:
        ptr<HTTP::SSL> SSL;
        uint32 Redirects;
        tools::rate_limiter Rate;
    };

    struct async : std::enable_shared_from_this<async> {

        // if no SSL context is provided, it will be created.
        async (asio::io_context &io, const HTTP::REST &rest, uint32 redirects = 10, tools::rate_limiter rate = {});
        async (asio::io_context &io, ptr<HTTP::SSL> ssl, const HTTP::REST &rest, uint32 redirects = 10, tools::rate_limiter rate = {});

        // async methods
        void operator () (handler<const response &>, const request &r);
        void GET (handler<const response &>, path path, list<entry<UTF8, UTF8>> params = {});
        void POST (handler<const response &>, path path, map<header, ASCII> headers, string body);
        void POST_form_data (handler<const response &>, path path, map<header, ASCII> form_data = {});

        struct make;

    private:
        asio::io_context &IO;
        ptr<HTTP::SSL> SSL;

        HTTP::REST REST;
        uint32 Redirects;
        tools::rate_limiter Rate;

        list<std::pair<request, handler<const response &>>> Queue;
        bool Writing;

        void send_next_request ();
        void send_next_request_now ();
    };
        
    inline blocking::blocking (const HTTP::REST &rest, uint32 redirects, tools::rate_limiter rate) :
        blocking {std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, redirects, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline blocking::blocking (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, uint32 redirects, tools::rate_limiter rate) :
        REST {rest}, SSL {ssl}, Redirects {redirects}, Rate {rate} {}


    inline async::async (asio::io_context &io, const HTTP::REST &rest, uint32 redirects, tools::rate_limiter rate) :
        async {io, std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, redirects, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline async::async (asio::io_context &io, ptr<HTTP::SSL> ssl, const HTTP::REST &rest, uint32 redirects, tools::rate_limiter rate) :
        IO {io}, SSL {ssl}, REST {rest}, Redirects {redirects}, Rate {rate}, Queue {}, Writing {false} {}
    
    response inline blocking::GET (path path, list<entry<UTF8, UTF8>> params) {
        return (*this) (REST.GET (path, params));
    }

    void inline async::GET (
        handler<const response &> on_receive,
        path path,
        list<entry<UTF8, UTF8>> params) {
        return (*this) (on_receive, REST.GET (path, params));
    }

    response inline blocking::POST (path path, map<header, ASCII> headers, string body) {
        return (*this) (REST.POST (path, headers, body));
    }

    void inline async::POST (
        handler<const response &> on_receive,
        path path, map<header, ASCII> headers,
        string body) {
        return (*this) (on_receive, REST.POST (path, headers, body));
    }
    
    // POST form data
    response inline blocking::POST_form_data (path path, map<header, ASCII> form_data) {
        return (*this) (REST.POST (path, form_data));
    }

    void inline async::POST_form_data (
        handler<const response &> on_receive,
        path path,
        map<header, ASCII> form_data) {
        return (*this) (on_receive, REST.POST (path, form_data));
    }
}

#endif
