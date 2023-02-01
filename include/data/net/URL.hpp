
// Copyright (c) 2027 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_URL
#define DATA_NET_URL

#include <data/tools.hpp>

namespace data::net {

    struct protocol {

        enum name {
            invalid,
            FTP,
            HTTP,
            HTTPS,
            WS,
            WSS
        };

        name Name;

        protocol (const string &);
        protocol (name);
        operator string ();

        bool operator == (const protocol &) const;
    };

    struct port: string {
        using string::string;

        port (uint16 number) : string {std::to_string (number)} {}
    };

    port default_port (protocol);

    std::ostream &operator << (std::ostream &, const port &);
    std::istream &operator >> (std::istream &, port &);
    
    struct URL {
        protocol Protocol;
        port Port;
        string Host;
        string Path;
        list<entry<string, string>> Params;
        
        URL (protocol pr, port p, string host, string path, list<entry<string, string>> params = {}) :
            Protocol (pr), Port {p}, Host {host}, Path {path}, Params {params} {}

        URL (protocol pr, string host, string path, list<entry<string, string>> params = {});
        
        URL (const string &);
        
        operator string () const;
        
    };
    
    std::ostream &operator << (std::ostream &, const URL &);
    std::istream &operator >> (std::istream &, URL &);
}

#endif
