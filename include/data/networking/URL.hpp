
// Copyright (c) 2027 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_URL
#define DATA_NETWORKING_URL

#include <data/tools.hpp>

namespace data::networking {

    enum class protocol {
        invalid,
        FTP,
        HTTP,
        HTTPS,
        WS,
        WSS
    };

    uint16 default_port_number (protocol);

    struct port {
        protocol Protocol;
        optional<uint16> Number;

        uint16 number () const {
            return bool(Number) ? *Number : default_port_number (Protocol);
        }

        port (const protocol p) : Protocol {p}, Number {} {}
        port (const protocol p, uint16 number) : Protocol {p}, Number {number} {}
        port (const string &);
        operator string() const;
    };

    std::ostream &operator << (std::ostream &, const port &);
    std::istream &operator >> (std::istream &, port &);
    
    struct URL {
        
        port Port;
        string Host;
        string Path;
        list<entry<string, string>> Params;
        
        URL (string port, string host, string path, list<entry<string, string>> params = {}) :
            Port{port}, Host{host}, Path{path}, Params{params} {}
        
        URL (const string &);
        
        operator string () const;
        
    };
    
    std::ostream &operator << (std::ostream &, const URL &);
    std::istream &operator >> (std::istream &, URL &);
}

#endif
