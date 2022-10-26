// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_REST
#define DATA_NETWORKING_REST

#include <data/networking/HTTP.hpp>

namespace data::networking {
    struct REST {
        
        string Port; // http or https
        string Host;
        
        // a typical GET request
        HTTP::request GET(string path, map<string, string> params = {}) const {
            return HTTP::request{HTTP::method::get, URL{Port, Host, path, params}};
        }
        
        // POST form data
        HTTP::request POST(string path, map<string, string> params = {}) const;
        
        // construct a more general POST request
        HTTP::request POST(string path, map<HTTP::header, string> headers, string body) const {
            return HTTP::request{
                HTTP::method::post, 
                Port, Host, path, headers, body};
        }
        
        // an HTTP request without host and port missing that can be used to
        // construct any other kind of request. 
        struct request {
            HTTP::method Method;
            string Path;
            map<string, string> Params;
            map<HTTP::header, string> Headers;
            string Body;
        };
        
        HTTP::request operator()(const request &r) const {
            return HTTP::request{r.Method, URL{Port, Host, r.Path, r.Params}, r.Headers, r.Body};
        }
        
    };
}

#endif
