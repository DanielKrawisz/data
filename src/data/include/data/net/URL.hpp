
// Copyright (c) 2023-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_URL
#define DATA_NET_URL

#include <data/net/URL/percent.hpp>
#include <data/net/asio/TCP.hpp>
#include <data/dispatch.hpp>

// Implementation of a URL as described in RFC3986
// https://www.rfc-editor.org/rfc/rfc3986

namespace data::net {

    // universal resource locator.
    struct URL;

    std::ostream &operator << (std::ostream &, const URL &);

    bool operator == (const URL &, const URL &);

    // protocol is the first part of the URL, and it could be something like ftp or http.
    struct protocol;

    std::ostream &operator << (std::ostream &, const protocol &);

    bool operator == (const protocol &, const protocol &);

    // the domain name appears in URLs that do not directly refer to an IP address.
    struct domain_name;

    std::ostream &operator << (std::ostream &, const domain_name &);

    bool operator == (const domain_name &, const domain_name &);

    // a port may be directly specified in a url or may be inferred
    // from the protocol.
    struct port;

    std::ostream &operator << (std::ostream &, const port &);

    bool operator == (const port &, const port &);

    uint16 default_port (const protocol &);

    // the path is the part of the URL including and after
    // the first '/' and before the first '?'.
    struct path;

    std::ostream &operator << (std::ostream &, const path &);

    // the path all the way to the end.
    struct target;

    std::ostream &operator << (std::ostream &, const target &);

    bool operator == (const target &, const target &);

    // the authority is the domain name or an IP address, possibly with port included.
    struct authority;

    std::ostream &operator << (std::ostream &, const authority &);

    bool operator == (const authority &, const authority &);

}

namespace data::net::IP {

    struct address;

    std::ostream &operator << (std::ostream &, const address &);

    bool operator == (const address &, const address &);

}

namespace data::net::IP::TCP {

    // a TCP endpoint is a URL consisting of TCP as the protocol with an ip address and port.
    struct endpoint;

    std::ostream &operator << (std::ostream &, const endpoint &);

    bool operator == (const endpoint &, const endpoint &);

}

namespace data::net {

    struct protocol : ASCII {
        static constexpr auto pattern = ctll::fixed_string {R"([A-Za-z][A-Za-z0-9+\-\.]*)"};

        using ASCII::ASCII;
        protocol (const ASCII &x) : ASCII {x} {}

        bool valid () const;

        enum name {
            unknown,
            FTP,
            TCP,
            HTTP,
            HTTPS,
            WS,
            WSS
        };

        explicit operator name () const;

        protocol (name);

        bool operator == (const name &) const;

        static protocol encode (name);
    };

    // port is a uint16 that may be in a url, but it may be inferred from the protocol.
    // sometimes people call the protocol the port.
    struct port : ASCII {
        static constexpr auto pattern = ctll::fixed_string {R"([0-9]*)"};
        //using ASCII::ASCII;
        port (const ASCII &x) : ASCII {x} {}

        port (uint16 number) : ASCII {std::to_string (number)} {}

        bool valid () const;
    };

    struct domain_name : ASCII {
        using ASCII::ASCII;
        domain_name (const ASCII &x) : ASCII {x} {}

        static bool valid (string_view);
        bool valid () const;
    };

    struct authority : UTF8 {
        using UTF8::UTF8;

        authority (const domain_name &d);
        authority (const IP::address &ad);

        authority (const domain_name &d, uint16 port);
        authority (const IP::address &ad, uint16 port);

        maybe<IP::address> address () const;
        maybe<IP::TCP::endpoint> endpoint () const;
        maybe<UTF8> host () const;
        maybe<ASCII> port () const;
        maybe<uint16> port_number () const;

        static bool valid (string_view);
        bool valid () const;

    };
}

namespace data::net::IP {

    struct address : UTF8 {

        using UTF8::UTF8;
        address (const UTF8 &x) : UTF8 {x} {}
        address (const byte_array<4> &);
        address (const byte_array<16> &);

        bool valid () const;

        // 4 or 6 or -1 for error.
        int32 version () const;

        // will be length 4 for ipv4 and 16 for ipv6
        explicit operator bytes () const;
        operator asio::ip::address () const;
    };

    // for when you need just one type of address.
    struct v4_address : address {
        v4_address (const UTF8 &x) : address {x} {}
        v4_address (const byte_array<4> &);

        explicit operator byte_array<4> () const;
    };

    struct v6_address : address {
        v6_address (const UTF8 &x) : address {x} {}
        v6_address (const byte_array<16> &);

        explicit operator byte_array<16> () const;
    };
}

namespace data::net {
    // A path must begin with '/' but '/' is not
    // necessarily a delimiter. path must be a
    // percent-encoded string because a delimeter
    // character may be percent encoded, which would
    // change structure of the path if it were
    // decoded. Thus, two paths can be percent
    // equal without being equal.
    struct path : encoding::percent::string {
        using encoding::percent::string::string;
        path (list<UTF8>, char delim = '/');
        list<UTF8> read (char delim = '/') const;

        static bool valid (string_view);
        bool valid () const;
    };

    // same with target, which includes path.
    struct target : encoding::percent::string {
        static bool valid (string_view);

        using encoding::percent::string::string;

        net::path path () const;

        maybe<ASCII> query () const; // the part after ? and before #
        maybe<dispatch<UTF8, UTF8>> query_map () const;

        maybe<UTF8> fragment () const; // the part after the #

        bool valid () const;

        // make a target bit by bit.
        struct make {
            operator target () const;

            make () {}
            explicit make (const target &);

            make path (const net::path &) const;

            make query_map (dispatch<UTF8, UTF8>) const;
            make query (const ASCII &) const;

            make fragment (const UTF8 &) const;

            using pctstr = encoding::percent::string;

            // we use pointers here instead of maybes
            // so that we don't copy these over and over
            // as we add more elements.
            ptr<pctstr> Path;
            ptr<pctstr> Query;
            ptr<pctstr> Fragment;

        };

        make read () const;

    };

    struct URL : encoding::percent::URI {

        // get the protocol of the URL, which is the part before the first ":"
        net::protocol protocol () const;

        // attempt to get port as a number.
        maybe<uint16> port_number () const;

        // get the port or the protocol if not available.
        ASCII port_DNS () const;

        // attempt to get host as a DNS string.
        maybe<net::domain_name> domain_name () const;

        // attempt to get host as an IP address.
        maybe<IP::address> address () const;

        // attempt to get query as a map of key pairs in the form <key>=<value>&...
        maybe<dispatch<UTF8, UTF8>> query_map () const;

        // get user info as <username>:<password>.
        // (This is insecure but supported because we sometimes still use it anyway)
        maybe<entry<UTF8, UTF8>> user_name_pass () const;

        // attempt to read as a TCP endpoint.
        maybe<IP::TCP::endpoint> endpoint () const;

        using encoding::percent::URI::URI;
        URL (const encoding::percent::URI &x) : encoding::percent::URI {x} {}

        // for making urls in steps.
        struct make {
            operator URL () const;

            make () {}
            explicit make (const URL &);

            make protocol (const net::protocol &) const;
            make scheme (const ASCII &) const;

            make port (const uint16 &) const;

            make address (const IP::address &) const;
            make domain_name (const net::domain_name &) const;

            // Handles the case that some other registeration process
            // besides DNS is being used and we are using the
            // registered name as the host.
            make registered_name (const UTF8 &) const;

            make user_info (const UTF8 &info) const;

            // insecure for web
            make user_name_pass (const UTF8 &username, const UTF8 &pass) const;

            make authority (const UTF8 &) const;

            make path (const net::path &) const;
            make target (const net::target &) const;

            make query_map (dispatch<UTF8, UTF8>) const;
            make query (const ASCII &) const;

            make fragment (const UTF8 &) const;

            using pctstr = encoding::percent::string;

            ptr<pctstr> Protocol;

            ptr<pctstr> UserInfo;
            ptr<pctstr> Host;
            ptr<pctstr> Port;

            ptr<target::make> Target;
        };

    private:
        make read () const;
    };
}

namespace data::net::IP::TCP {

    struct endpoint : URL {

        endpoint () : URL {} {}
        endpoint (const IP::address &addr, uint16 port);

        // input endpoint as a string.
        endpoint (const char *x);
        endpoint (const std::string &x);

        bool valid () const;

        IP::address address () const;
        uint16 port () const;

        uint16 port_number () const {
            return port ();
        }

        data::UTF8 authority () const {
            return *static_cast<const URL *> (this)->authority ();
        }

        data::UTF8 host () const {
            return *static_cast<const URL *> (this)->host ();
        }
        
        void user_info () const = delete;
        void domain_name () const = delete;
        void query () const = delete;
        void fragment () const = delete;

        // we use asio for the backend of some of this stuff.
        operator asio::ip::tcp::endpoint () const;
    };
}

namespace data {

    bool case_insensitive_equal (const string &, const string &);

}

namespace data::net {

    bool inline IP::operator == (const IP::address &a, const IP::address &b) {
        return case_insensitive_equal (a, b);
    }

    bool inline operator == (const domain_name &a, const domain_name &b) {
        return case_insensitive_equal (a, b);
    }

    bool inline operator == (const protocol &a, const protocol &b) {
        return case_insensitive_equal (a, b);
    }

    bool inline operator == (const port &a, const port &b) {
        return case_insensitive_equal (a, b);
    }

    bool inline operator == (const authority &a, const authority &b) {
        return static_cast<const UTF8 &> (a) == static_cast<const UTF8 &> (b);
    }

    bool inline operator == (const target &a, const target &b) {
        return static_cast<const UTF8 &> (a) == static_cast<const UTF8 &> (b);
    }

    std::ostream inline &IP::operator << (std::ostream &o, const IP::address &x) {
        return o << static_cast<string> (x);
    }

    std::ostream inline &operator << (std::ostream &o, const domain_name &x) {
        return o << static_cast<string> (x);
    }

    std::ostream inline &operator << (std::ostream &o, const protocol &x) {
        return o << static_cast<string> (x);
    }

    std::ostream inline &operator << (std::ostream &o, const port &x) {
        return o << static_cast<string> (x);
    }

    std::ostream inline &operator << (std::ostream &o, const authority &x) {
        return o << static_cast<string> (x);
    }

    std::ostream inline &operator << (std::ostream &o, const target &x) {
        return o << static_cast<string> (x);
    }

    std::ostream inline &operator << (std::ostream &o, const path &x) {
        return o << static_cast<string> (x);
    }

    bool inline net::protocol::valid () const {
        return ctre::match<pattern> (*this);
    }

    inline protocol::protocol (name n) : string {encode (n)} {}

    bool inline protocol::operator == (const name &n) const {
        return *this == protocol (n);
    }

    bool inline port::valid () const {
        return ctre::match<pattern> (*this) || ctre::match<protocol::pattern> (*this);
    }

    inline URL::make::make (const URL &u) : make {u.read ()} {}

    inline target::make::make (const target &t) : make {t.read ()} {}

    bool inline operator == (const URL &a, const URL &b) {
        return static_cast<string> (a.normalize ()) == static_cast<string> (b.normalize ());
    }

    std::ostream inline &operator << (std::ostream &o, const URL &u) {
        return o << static_cast<string> (u);
    }

    bool inline IP::TCP::operator == (const IP::TCP::endpoint &a, const IP::TCP::endpoint &b) {
        return static_cast<string> (a) == static_cast<string> (b.normalize ());
    }

    std::ostream inline &IP::TCP::operator << (std::ostream &o, const IP::TCP::endpoint &u) {
        return o << static_cast<string> (u);
    }

    protocol inline URL::protocol () const {
        return net::protocol {this->scheme ()};
    }

    URL::make inline URL::make::scheme (const ASCII &x) const {
        return protocol (x);
    }

    ASCII inline URL::port_DNS () const {
        auto p = this->port ();
        if (!p) return this->protocol ();
        return *p;
    }

    bool inline domain_name::valid () const {
        return valid (*this);
    }

    inline authority::authority (const domain_name &d): UTF8 {d} {}
    inline authority::authority (const IP::address &ad): UTF8 {ad} {}

    inline authority::authority (const domain_name &d, uint16 port): UTF8 {std::string {d} + ":" + net::port {port}} {}
    inline authority::authority (const IP::address &ad, uint16 port): UTF8 {std::string {ad} + ":" + net::port {port}} {}

    bool inline path::valid () const {
        return valid (*this);
    }

    bool inline target::valid () const {
        return valid (*this);
    }

    bool inline authority::valid () const {
        return valid (encoding::percent::encode (*this));
    }
}

namespace data::net::IP::TCP {

    inline endpoint::endpoint (const IP::address &addr, uint16 port) : URL {URL::make {}.protocol ("tcp").address (addr).port (port)} {}
    inline endpoint::endpoint (const char *x) : URL {x} {}
    inline endpoint::endpoint (const std::string &x) : URL {x} {}

    bool inline endpoint::valid () const {
        return URL::valid () && this->protocol () == protocol::TCP && bool (static_cast<const URL *> (this)->port_number ()) &&
            bool (static_cast<const URL *> (this)->address ()) && !bool (static_cast<const URL *> (this)->user_info ()) &&
            !bool (static_cast<const URL *> (this)->fragment ()) && !bool (static_cast<const URL *> (this)->query ());
    }

    IP::address inline endpoint::address () const {
        return *static_cast<const URL *> (this)->address ();
    }

    uint16 inline endpoint::port () const {
        return *static_cast<const URL *> (this)->port_number ();
    }

    inline endpoint::operator asio::ip::tcp::endpoint () const {
        return valid () ? asio::ip::tcp::endpoint {
            asio::ip::make_address (this->address ()),
            this->port ()
        } : asio::ip::tcp::endpoint {};
    }
}

#endif
