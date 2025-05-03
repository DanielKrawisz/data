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

        HTTP::REST REST;
        
        client (const HTTP::REST &rest, tools::rate_limiter rate = {});
        client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate = {});

        // async methods
        awaitable<response> operator () (const request &r);
        awaitable<response> GET (path path, list<entry<UTF8, UTF8>> params = {});
        awaitable<response> POST (path path, map<header, ASCII> headers, string body);
        awaitable<response> POST_form_data (path path, map<ASCII, ASCII> form_data = {});

        // handler methods
        void operator () (exec, asio::error_handler, handler<const response &>, request r);
        void GET (exec, asio::error_handler, handler<const response &>, path path, list<entry<UTF8, UTF8>> params = {});
        void POST (exec, asio::error_handler, handler<const response &>, path path, map<header, ASCII> headers, string body);
        void POST_form_data (exec, asio::error_handler, handler<const response &>, path path, map<ASCII, ASCII> form_data = {});

    private:
        ptr<HTTP::SSL> SSL;

        tools::rate_limiter Rate;
    };
        
    inline client::client (const HTTP::REST &rest, tools::rate_limiter rate) :
        client {std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline client::client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate) :
        REST {rest}, SSL {ssl}, Rate {rate} {}
    
    awaitable<response> inline client::GET (path path, list<entry<UTF8, UTF8>> params) {
        co_return co_await (*this) (REST.GET (path, params));
    }

    awaitable<response> client::POST (path path, map<header, ASCII> headers, string body) {
        co_return co_await (*this) (REST.POST (path, headers, body));
    }
    
    // POST form data
    awaitable<response> inline client::POST_form_data (path path, map<ASCII, ASCII> form_data) {
        co_return co_await (*this) (REST.POST (path, form_data));
    }

    void inline client::GET (exec e, asio::error_handler err, handler<const response &> rh, path path, list<entry<UTF8, UTF8>> params) {
        return (*this) (e, err, rh, REST.GET (path, params));
    }

    void inline client::POST (exec e, asio::error_handler err, handler<const response &> rh, path path, map<header, ASCII> headers, string body) {
        return (*this) (e, err, rh, REST.POST (path, headers, body));
    }

    void inline client::POST_form_data (exec e, asio::error_handler err, handler<const response &> rh, path path, map<ASCII, ASCII> form_data) {
        return (*this) (e, err, rh, REST.POST (path, form_data));
    }
}

#endif
