
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

    struct port: string {
        using string::string;

        port (uint16 number) : string {std::to_string(number)} {}
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
        
        URL (protocol protocol, string port, string host, string path, list<entry<string, string>> params = {}) :
            Protocol (protocol), Port {port}, Host {host}, Path {path}, Params {params} {}
        
        URL (const string &);
        
        operator string () const;
        
    };
    
    std::ostream &operator << (std::ostream &, const URL &);
    std::istream &operator >> (std::istream &, URL &);
}

#endif
