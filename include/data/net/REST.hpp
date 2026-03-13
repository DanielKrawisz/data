// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_REST
#define DATA_NET_REST

#include <data/net/HTTP.hpp>

namespace data::net::HTTP {

    // REST is for converting typical REST API formats into a regular HTTP request.
    struct REST {
        using form = dispatch<UTF8, UTF8>;
        
        // a typical GET request
        HTTP::request::make GET (path path) const;
        HTTP::request::make GET (path path, form params) const;
        
        // POST form data
        HTTP::request::make POST (path path, form params = {}) const;

        REST (const ASCII &);
        REST (const ASCII &, const path &);

        authority Host;

        // the part of the path that is common to the whole API.
        path Path;

        // if this is present, it will be put in the Authorization header.
        maybe<ASCII> Authorization;

        HTTP::request::make operator () (HTTP::method meth, const path &path) const;

        static encoding::percent::string encode_form_data (form);
        
    };
    
    HTTP::request::make inline REST::GET (path path) const {
        return operator () (method::get, path);
    }
    
    HTTP::request::make inline REST::GET (path path, dispatch<UTF8, UTF8> params) const {
        return operator () (method::get, path).query (encode_form_data (params));
    }

    HTTP::request::make inline REST::POST (path path, dispatch<UTF8, UTF8> params) const {
        return operator () (method::post, path).body (bytes (encode_form_data (params)),
            content::application_x_www_form_urlencoded);
    }

    inline REST::REST (const ASCII &host):
        Host {host}, Path {} {}

    inline REST::REST (const ASCII &host, const path &p):
        Host {host}, Path {p} {}

}

#endif
