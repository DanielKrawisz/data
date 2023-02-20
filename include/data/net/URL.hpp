
// Copyright (c) 2027 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_URL
#define DATA_NET_URL

#include <data/tools.hpp>

namespace data::net {

    struct protocol : string {

        enum name {
            invalid,
            FTP,
            HTTP,
            HTTPS,
            WS,
            WSS
        };

        explicit operator name () const;

        using string::string;

        protocol (name);

        bool operator == (const protocol &) const;
        bool operator == (const name &) const;

        bool valid () const;

        static string to_string (name);
    };

    std::ostream &operator << (std::ostream &, const protocol &);
    std::istream &operator >> (std::istream &, protocol &);

    struct port: string {
        using string::string;
        port (const string &p) : string {p} {}
        port (uint16 number) : string {std::to_string (number)} {}
    };

    port default_port (protocol);

    std::ostream &operator << (std::ostream &, const port &);
    std::istream &operator >> (std::istream &, port &);
    
    struct URL {
        const net::protocol &protocol () const;
        net::port port () const;
        const string &host () const;
        const string &path () const;
        list<entry<string, string>> params () const;
        
        URL (net::protocol pr, net::port p, string host, string path, list<entry<string, string>> params = {}) :
            Protocol (pr), Port {p}, Host {host}, Path {path}, Params {params} {}

        URL (net::protocol pr, string host, string path, list<entry<string, string>> params = {}) :
            Protocol (pr), Port {}, Host {host}, Path {path}, Params {params} {}
        
        URL (const string &);
        URL (const char *);
        
        operator string () const;

    private:
        net::protocol Protocol;
        net::port Port;
        string Host;
        string Path;
        list<entry<string, string>> Params;
    };
    
    std::ostream &operator << (std::ostream &, const URL &);
    std::istream &operator >> (std::istream &, URL &);

    inline URL::URL (const char *meep): URL {string {meep}} {}

    const protocol inline &URL::protocol () const {
        return Protocol;
    }

    port inline URL::port () const {
        return Port == "" ? net::port {string (Protocol)} : Port;
    }

    const string inline &URL::host () const {
        return Host;
    }

    const string inline &URL::path () const {
        return Path;
    }

    list<entry<string, string>> inline URL::params () const {
        return Params;
    }

    bool inline protocol::valid () const {
        return name (*this) != invalid;
    }

    inline protocol::protocol (name n) : string {to_string (n)} {}

    bool inline protocol::operator == (const protocol &p) const {
        return name (*this) == name (p);
    }

    bool inline protocol::operator == (const name &n) const {
        return name (*this) == n;
    }

    std::ostream inline &operator << (std::ostream &o, const protocol &p) {
        return o << static_cast<string> (p);
    }

    std::ostream inline &operator << (std::ostream &o, const port &p) {
        return o << static_cast<string> (p);
    }

    std::ostream inline &operator << (std::ostream &o, const URL &u) {
        return o << string (u);
    }
}

#endif
