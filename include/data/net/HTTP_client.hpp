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
        caller &Http;
        
        REST Rest;
        
        tools::rate_limiter Rate;
        
        client (caller &http, const REST &rest, tools::rate_limiter rate = {});
        
        response operator () (const request &r);
        
        response GET (string path, list<entry<string, string>> params = {});
        
        // POST form data
        response POST (string path, map<string, string> form_data = {});
        
        response POST (string path, map<header, string> headers, string body);
        
    };
        
    inline client::client (caller &http, const REST &rest, tools::rate_limiter rate) :
        Http {http}, Rest {rest}, Rate {rate} {}
    
    response inline client::operator () (const request &r) {
        auto wait = Rate.getTime ();
        if (wait != 0) boost::asio::steady_timer {*Http.IOContext, boost::asio::chrono::seconds (wait)}.wait ();
        return Http (r);
    }
    
    response inline client::GET (string path, list<entry<string, string>> params) {
        return (*this) (Rest.GET (path, params));
    }
    
    // POST form data
    response inline client::POST (string path, map<string, string> form_data) {
        return (*this) (Rest.POST (path, form_data));
    }
    
    response inline client::POST (string path, map<header, string> headers, string body) {
        return (*this) (Rest.POST (path, headers, body));
    }
}

#endif
