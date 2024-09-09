// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_REST
#define DATA_NET_REST

#include <data/net/HTTP.hpp>

namespace data::net::HTTP {
    // REST is for converting typical REST API formats into a regular HTTP request.
    struct REST {
        
        // a typical GET request
        HTTP::request GET (path path) const;
        HTTP::request GET (path path, list<entry<UTF8, UTF8>> params) const;
        
        // POST form data
        HTTP::request POST (path path, map<header, ASCII> params = {}) const;
        
        // construct a more general POST request
        HTTP::request POST (path path, map<header, ASCII> headers, string body) const;

        REST (const protocol &, const domain_name &);
        REST (const protocol &, const domain_name &, uint16);

        // http or https
        protocol Protocol;
        domain_name Host;
        maybe<uint16> Port;

        // an HTTP request without host and port missing that can be used to
        // construct any other kind of request. 
        struct request {
            // normally GET or POST but HTTP has many other methods defined for it.
            method Method;

            // The part of the URL after the host but before '?'
            path Path;

            // The part after '?' and before '#'
            maybe<list<entry<UTF8, UTF8>>> Query;

            // the part after '"'
            maybe<data::UTF8> Fragment;
            map<header, ASCII> Headers;
            string Body;
        };
        
        HTTP::request operator () (const request &r) const;
        
    };
    
    HTTP::request inline REST::GET (path path) const {
        auto make_url = URL::make {}.protocol (Protocol).domain_name (Host).path (path);
        return HTTP::request {method::get, URL (bool (Port) ? make_url.port (*Port) : make_url)};
    }
    
    HTTP::request inline REST::GET (path path, list<entry<UTF8, UTF8>> params) const {
        auto make_url = URL::make {}.protocol (Protocol).domain_name (Host).path (path).query_map (params);
        return HTTP::request {method::get, URL (bool (Port) ? make_url.port (*Port) : make_url)};
    }
    
    // construct a more general POST request
    HTTP::request inline REST::POST (path path, map<header, ASCII> headers, string body) const {
        auto make_url = URL::make {}.protocol (Protocol).domain_name (Host).path (path);
        return HTTP::request {method::post, URL (bool (Port) ? make_url.port (*Port) : make_url), headers, body};
    }

    inline REST::REST (const protocol &pro, const domain_name &host, uint16 p): Protocol {pro}, Port {p}, Host {host} {}
    inline REST::REST (const protocol &pro, const domain_name &host) : Protocol {pro}, Port {}, Host {host} {}

}

#endif
