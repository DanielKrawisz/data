// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/REST.hpp>

namespace data::net::HTTP {
        
    string REST::encode_form_data (map<ASCII, ASCII> form_data) {
        std::stringstream newBody;

        auto entries = form_data.values ();
            
        if (data::size (entries) > 0) {

            auto it = entries.begin ();

            while (true) {
                newBody << std::string (it->Key) << "=" << it->Value;

                it++;
                if (it == entries.end ()) break;

                newBody << "&";
            }
        }
            
        return newBody.str ();
    }

    HTTP::request REST::operator () (const request &r) const {
        auto make_url = URL::make {}.protocol (Protocol).domain_name (Host).path (Path + r.Path);
        if (bool (r.Query)) make_url = make_url.query_map (*r.Query);
        if (bool (r.Fragment)) make_url = make_url.fragment (*r.Fragment);
        if (bool (Port)) make_url = make_url.port (*Port);
        return HTTP::request {r.Method, URL (make_url),
            bool (Authorization) ? r.Headers.insert (header::authorization, *Authorization): r.Headers,
            r.Body};
    }
}
