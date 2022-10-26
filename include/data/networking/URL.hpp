
// Copyright (c) 2027 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_URL
#define DATA_NETWORKING_URL

#include <data/types.hpp>

namespace data::networking {
    
    struct URL {
        
        string Port; // "http" or "https"
        string Host;
        string Path;
        map<string, string> Params;
        
        URL(string port, string host, string path, map<string, string> params = {}) : 
            Port{port}, Host{host}, Path{path}, Params{params} {}
        
        URL(const string &);
        
        operator string() const;
        
    };
}

#endif
