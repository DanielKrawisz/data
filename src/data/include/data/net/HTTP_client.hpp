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

        awaitable<response> operator () (const request &r);
        awaitable<response> GET (path path, dispatch<UTF8, UTF8> params = {});
        awaitable<response> POST (path path, dispatch<UTF8, UTF8> form_data = {});

    private:
        ptr<HTTP::SSL> SSL;
        tools::rate_limiter Rate;
        ptr<session> Session;
    };
        
    inline client::client (const HTTP::REST &rest, tools::rate_limiter rate) :
        client {std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline client::client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, tools::rate_limiter rate) :
        REST {rest}, SSL {ssl}, Rate {rate}, Session {} {}
    
    awaitable<response> inline client::GET (path path, dispatch<UTF8, UTF8> params) {
        co_return co_await (*this) (REST.GET (path, params));
    }
    
    // POST form data
    awaitable<response> inline client::POST (path path, dispatch<UTF8, UTF8> form_data) {
        co_return co_await (*this) (REST.POST (path, form_data));
    }
}

#endif
