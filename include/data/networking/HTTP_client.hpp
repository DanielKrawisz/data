// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_API
#define DATA_API

#include <data/networking/REST.hpp>
#include <data/tools/rate_limiter.hpp>
#include <stdlib.h>

namespace data::networking {
    struct HTTP_client {
        networking::HTTP &Http;
        
        REST Rest;
        
        tools::rate_limiter Rate;
        
        HTTP_client(networking::HTTP &http, const REST &rest, tools::rate_limiter rate = {}) : Http{http}, Rest{rest}, Rate{rate} {}
        
        HTTP::response operator()(const HTTP::request &r) {
            auto wait = Rate.getTime();
            if (wait != 0) boost::asio::steady_timer{Http.IOContext, boost::asio::chrono::seconds(wait)}.wait();
            return Http(r);
        }
        
        HTTP::response GET(string path, map<string, string> params = {}) {
            return (*this)(Rest.GET(path, params));
        }
        
        // POST form data
        HTTP::response POST(string path, map<string, string> form_data = {}) {
            return (*this)(Rest.POST(path, form_data));
        }
        
        HTTP::response POST(string path, map<HTTP::header, string> headers, string body) {
            return (*this)(Rest.POST(path, headers, body));
        }
        
    };
}
#endif
