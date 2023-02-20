// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/REST.hpp>

namespace data::net::HTTP {
    namespace {
        
        string encode_form_data (map<string, string> form_data) {
            string newBody;
            
            if (form_data.size ()>0) {

                for (const auto &it : form_data) {
                    newBody.append (it.Key+"="+it.Value+"&");
                }

                newBody.pop_back ();
            }
            
            return newBody;
        }
    }

    HTTP::request REST::POST (string path, map<string, string> params) const {
        return HTTP::request (method::post, URL {Protocol, Port, Host, path},
            {{boost::beast::http::field::content_type, "application/x-www-form-urlencoded"}}, 
              encode_form_data (params));
    }
}
