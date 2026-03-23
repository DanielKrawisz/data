// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_HTTP_CLIENT
#define DATA_NET_HTTP_CLIENT

#include <data/net/REST.hpp>
#include <data/tools/rate_limiter.hpp>
#include <stdlib.h>

namespace data::net::HTTP {

    // manage a remote connection to a server and abstract requests
    // as function calls. The connection is closed on destruction.
    struct client {

        HTTP::REST REST;
        
        client (const HTTP::REST &rest, rate_limiter rate = {});
        client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, rate_limiter rate = {});

        awaitable<response> operator () (const request &r);
        awaitable<response> GET (path path, dispatch<UTF8, UTF8> params = {});
        awaitable<response> POST (path path, dispatch<UTF8, UTF8> form_data = {});

    private:
        // optional. If not present we use http.
        ptr<HTTP::SSL> SSL;

        // optional.
        rate_limiter Rate;

        // TODO add a timer to close the session if we wait too long.
        ptr<stream> Session;
    };
        
    inline client::client (const HTTP::REST &rest, rate_limiter rate) :
        client {std::make_shared<HTTP::SSL> (HTTP::SSL::tlsv12_client), rest, rate} {
            SSL->set_default_verify_paths ();
            SSL->set_verify_mode (asio::ssl::verify_peer);
        }

    inline client::client (ptr<HTTP::SSL> ssl, const HTTP::REST &rest, rate_limiter rate) :
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
