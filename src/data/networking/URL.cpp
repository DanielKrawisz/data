// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/networking/URL.hpp>

namespace data::networking {
    namespace {
        
        string write_params(list<entry<string, string>> params) {
            string path;
            
            if(params.size()>0) {
                path.append("?");
                for (const auto &it : params) {
                    path.append(it.Key + "=" + it.Value + "&");
                }
                path.pop_back();
            }
            
            return path;
        }
        
    }
    
    std::ostream &operator<<(std::ostream &o, const URL &u) {
        return o << u.Port << "://" << u.Host << u.Path << write_params(u.Params);
    }
    
    URL::operator string() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
}
