// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_REST
#define DATA_NET_REST

#include <data/net/HTTP.hpp>

namespace data::net {
    // REST is for converting typical REST API formats into a regular HTTP request.
    struct REST {
        
        // a typical GET request
        HTTP::request GET (string path, list<entry<string, string>> params = {}) const;
        
        // POST form data
        HTTP::request POST (string path, map<string, string> params = {}) const;
        
        // construct a more general POST request
        HTTP::request POST (string path, map<HTTP::header, string> headers, string body) const;
        
        // an HTTP request without host and port missing that can be used to
        // construct any other kind of request. 
        struct request {
            HTTP::method Method;
            string Path;
            list<entry<string, string>> Params;
            map<HTTP::header, string> Headers;
            string Body;
        };
        
        HTTP::request operator () (const request &r) const;

        protocol Protocol; // http or https

        port Port;

        string Host;

        REST (protocol, port, string);
        REST (protocol, string);
        
    };
    
    HTTP::request inline REST::GET (string path, list<entry<string, string>> params) const {
        return HTTP::request {HTTP::method::get, URL {Protocol, Port, Host, path, params}};
    }
    
    // construct a more general POST request
    HTTP::request inline REST::POST (string path, map<HTTP::header, string> headers, string body) const {
        return HTTP::request {
            HTTP::method::post, 
            URL {Protocol, Port, Host, path}, headers, body};
    }
    
    HTTP::request inline REST::operator () (const request &r) const {
        return HTTP::request {r.Method, URL {Protocol, Port, Host, r.Path, r.Params}, r.Headers, r.Body};
    }

    inline REST::REST (protocol pro, port p, string host): Protocol {pro}, Port {p}, Host {host} {}
}

#endif
