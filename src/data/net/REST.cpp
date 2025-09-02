// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/REST.hpp>

namespace data::net::HTTP {
        
    encoding::percent::string REST::encode_form_data (dispatch<UTF8, UTF8> params) {
        std::stringstream q;

        auto i = params.begin ();

        if (i != params.end ()) {

            while (true) {
                q << static_cast<std::string> (encoding::percent::encode ((*i).Key, ":#[]@=&")) << "=" <<
                    static_cast<std::string> (encoding::percent::encode ((*i).Value, ":#[]@=&"));
                i++;
                if (i == params.end ()) break;
                q << "&";
            }
        }

        return q.str ();
    }

    HTTP::request::make REST::operator () (const HTTP::request::make &r) const {
        auto rr = r.host (Host);
        if (bool (Authorization)) rr = rr.authorization (*Authorization);
        return rr;
    }
}
