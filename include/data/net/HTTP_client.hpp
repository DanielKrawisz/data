// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_CLIENT
#define DATA_NET_HTTP_CLIENT

#include <data/net/REST.hpp>
#include <data/tools/rate_limiter.hpp>
#include <stdlib.h>

namespace data::net::HTTP {
    struct client_blocking {

        HTTP::REST REST;
        
        client_blocking (const HTTP::REST &rest, tools::rate_limiter rate = {});
        client_blocking (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate = {});

        // blocking methods
        response operator () (const request &r);
        response GET (path path, list<entry<UTF8, UTF8>> params = {});
        response POST (path path, map<header, ASCII> headers, string body);
        response POST_form_data (path path, map<header, ASCII> form_data = {});

    private:
        ptr<HTTP::SSL> SSL;

        tools::rate_limiter Rate;
    };

    struct client_async : std::enable_shared_from_this<client_async> {

        client_async (ptr<asio::io_context> io, const HTTP::REST &rest, tools::rate_limiter rate = {});
        client_async (ptr<asio::io_context> io, ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate = {});

        // async methods
        void operator () (handler<const response &>, const request &r);
        void GET (handler<const response &>, path path, list<entry<UTF8, UTF8>> params = {});
        void POST (handler<const response &>, path path, map<header, ASCII> headers, string body);
        void POST_form_data (handler<const response &>, path path, map<header, ASCII> form_data = {});

    private:
        ptr<asio::io_context> IO;
        ptr<HTTP::SSL> SSL;

        HTTP::REST REST;

        tools::rate_limiter Rate;

        list<std::pair<request, handler<const response &>>> Queue;
        bool Writing;

        void send_next_request ();
        void send_next_request_now ();
    };
        
    inline client_blocking::client_blocking (const HTTP::REST &rest, tools::rate_limiter rate) :
        client_blocking {std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline client_blocking::client_blocking (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate) :
        REST {rest}, SSL {ssl}, Rate {rate} {}


    inline client_async::client_async (ptr<asio::io_context> io, const HTTP::REST &rest, tools::rate_limiter rate) :
        client_async {io, std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline client_async::client_async (ptr<asio::io_context> io, ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate) :
        IO {io}, SSL {ssl}, REST {rest}, Rate {rate}, Queue {}, Writing {false} {}
    
    response inline client_blocking::GET (path path, list<entry<UTF8, UTF8>> params) {
        return (*this) (REST.GET (path, params));
    }

    void inline client_async::GET (
        handler<const response &> on_receive,
        path path,
        list<entry<UTF8, UTF8>> params) {
        return (*this) (on_receive, REST.GET (path, params));
    }

    response inline client_blocking::POST (path path, map<header, ASCII> headers, string body) {
        return (*this) (REST.POST (path, headers, body));
    }

    void inline client_async::POST (
        handler<const response &> on_receive,
        path path, map<header, ASCII> headers,
        string body) {
        return (*this) (on_receive, REST.POST (path, headers, body));
    }
    
    // POST form data
    response inline client_blocking::POST_form_data (path path, map<header, ASCII> form_data) {
        return (*this) (REST.POST (path, form_data));
    }

    void inline client_async::POST_form_data (
        handler<const response &> on_receive,
        path path,
        map<header, ASCII> form_data) {
        return (*this) (on_receive, REST.POST (path, form_data));
    }
}

#endif
