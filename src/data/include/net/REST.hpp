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
        HTTP::request POST (path path, map<ASCII, ASCII> params = {}) const;
        
        // construct a more general POST request
        HTTP::request POST (path path, map<header, ASCII> headers, string body) const;

        REST (const protocol &, const domain_name &);
        REST (const protocol &, const domain_name &, uint16 port);
        REST (const protocol &, const domain_name &, const path &);
        REST (const protocol &, const domain_name &, const path &, uint16 port);

        // http or https
        protocol Protocol;
        domain_name Host;
        // the part of the path that is common to the whole API.
        path Path;
        maybe<uint16> Port;
        // if this is present, it will be put in the Authorization header.
        maybe<ASCII> Authorization;

        // an HTTP request without host and port missing that can be used to
        // construct any other kind of request. 
        struct request {
            // normally GET or POST but HTTP has many other methods defined for it.
            method Method;

            // The part of the URL after the host but before '?'
            path Path;

            // The part after '?' and before '#'
            maybe<list<entry<UTF8, UTF8>>> Query;

            // the part after '#'
            maybe<data::UTF8> Fragment;
            map<header, ASCII> Headers;
            string Body;

            request (method m, path p,
                maybe<list<entry<UTF8, UTF8>>> q = {},
                maybe<data::UTF8> frag = {},
                map<header, ASCII> h = {},
                string body = ""): Method {m}, Path {p}, Query {q}, Fragment {frag}, Headers {h}, Body {body} {}
        };
        
        HTTP::request operator () (const request &r) const;

        static string encode_form_data (map<ASCII, ASCII> form_data);
        
    };
    
    HTTP::request inline REST::GET (path path) const {
        return operator () (REST::request {method::get, path});
    }
    
    HTTP::request inline REST::GET (path path, list<entry<UTF8, UTF8>> params) const {
        return operator () (REST::request {method::get, path, params});
    }
    
    // construct a more general POST request
    HTTP::request inline REST::POST (path path, map<header, ASCII> headers, string body) const {
        return operator () (REST::request {method::post, path, {}, {}, headers, body});
    }

    HTTP::request inline REST::POST (path path, map<ASCII, ASCII> params) const {
        return operator () (REST::request {method::post, path, {}, {},
            {{header::content_type, "application/x-www-form-urlencoded"}},
              encode_form_data (params)});
    }

    inline REST::REST (const protocol &pro, const domain_name &host):
        Protocol {pro}, Host {host}, Path {}, Port {} {}

    inline REST::REST (const protocol &pro, const domain_name &host, uint16 p):
        Protocol {pro}, Host {host}, Path {}, Port {p} {}

    inline REST::REST (const protocol &pro, const domain_name &host, const path &p):
        Protocol {pro}, Host {host}, Path {p}, Port {} {}

    inline REST::REST (const protocol &pro, const domain_name &host, const path &p, uint16 port):
        Protocol {pro}, Host {host}, Path {p}, Port {port} {}

}

#endif
