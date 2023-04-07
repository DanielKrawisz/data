// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/REST.hpp>

namespace data::net::HTTP {
    namespace {
        
        string encode_form_data (map<header, ASCII> form_data) {
            std::stringstream newBody;

            auto entries = form_data.values ();
            
            if (data::size (entries) > 0) {

                auto it = entries.begin ();

                while (true) {
                    newBody << boost::beast::http::to_string ((*it).Key) << "=" << (*it).Value;

                    it++;
                    if (it == entries.end ()) break;

                    newBody << "&";
                }
            }
            
            return newBody.str ();
        }
    }

    HTTP::request REST::POST (path path, map<header, ASCII> params) const {
        auto make_url = URL::make {}.protocol (Protocol).domain_name (Host).path (path);
        return HTTP::request (method::post, URL (bool (Port) ? make_url.port (*Port) : make_url),
            {{boost::beast::http::field::content_type, "application/x-www-form-urlencoded"}}, 
              encode_form_data (params));
    }
}
