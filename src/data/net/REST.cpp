// Copyright (c) 2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/REST.hpp>

namespace data::net::HTTP {
        
    UTF8 REST::encode_form_data (list<entry<UTF8, UTF8>> form_data) {
        std::stringstream newBody;

        auto it = form_data.begin ();

        if (it != form_data.end ()) {

            while (true) {
                newBody << std::string (it->Key) << "=" << it->Value;

                it++;
                if (it == form_data.end ()) break;

                newBody << "&";
            }
        }
            
        return newBody.str ();
    }

    HTTP::request::make REST::operator () (const HTTP::request::make &r) const {
        auto rr = r.host (Host);
        if (bool (Authorization)) rr = rr.authorization (*Authorization);
        return rr;
    }
}
