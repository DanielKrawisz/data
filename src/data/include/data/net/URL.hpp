
// Copyright (c) 2023-2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_URL
#define DATA_NET_URL

#include <ctre.hpp>
#include <data/encoding/ascii.hpp>
#include <data/maybe.hpp>
#include <data/list.hpp>

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

#include <data/net/asio/socket.hpp>

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

// RFC3986 defines certain reserved characters which
// denote the structure of a URI. Percent encoding
// ensures that arbitrary UTF8 strings can be included
// in a URL without affecting the structure of the URL.
//
// A UTF8 character is percent encoded as "%" followed
// by the hexidecimal value of the encoded character.
//
// Control characters and non-ASCII characters are
// required to be percent encoded. Other characetrs
// may optionally be percent encoded.
namespace data::encoding::percent {

    bool valid (string_view input);

    constexpr const char *Reserved = ":/?#[]@!$&'()*+,;=";
    constexpr const char *Delimiters = ":/?#[]@";
    constexpr const char *Subdelimiters = "!$&'()*+,;=";

    bool inline is_reserved (char c) {
        return strchr (Reserved, c) != nullptr;
    }

    // encode the string, ensuring that the given characters are encoded.
    std::string encode (const data::UTF8 &, const data::ASCII &required = "");

    bool equivalent (const std::string &, const std::string &);

    // a %-encoded string.
    struct string;

    // decode back to UTF8.
    maybe<data::UTF8> decode (string_view);

    struct string : data::ASCII {
        using data::ASCII::ASCII;

        string (const data::ASCII &x) : data::ASCII {x} {}

        // If a character is reserved, then its percent-encoded
        // form is unequal to its non-encoded form. Otherwise
        // they are treated as equal.
        bool operator == (const string &x) const {
            return equivalent (*this, x);
        }

        bool valid () const {
            return percent::valid (*this);
        }
    };

    // https://www.ietf.org/rfc/rfc3986.txt
    // A URI is a universal resource indicator, as opposed to locator.
    // the URI may not enable you to retrieve the document but it will
    // uniquely specify the document.
    struct URI : string {

        static string_view scheme (string_view);
        static string_view authority (string_view);
        static string_view path (string_view);
        static string_view query (string_view);
        static string_view fragment (string_view);

        static string_view user_info (string_view);
        static string_view host (string_view);
        static string_view port (string_view);

        static string_view address (string_view);

        // everything from the path to the end.
        static string_view target (string_view);

        data::ASCII scheme () const;
        maybe<data::UTF8> user_info () const;
        maybe<data::UTF8> host () const;
        maybe<data::ASCII> port () const;
        maybe<data::UTF8> authority () const;
        net::target target () const;
        net::path path () const;
        maybe<data::ASCII> query () const; // the part after ? and before #
        maybe<data::UTF8> fragment () const; // the part after the #

        bool valid () const;

        using string::string;
        URI (const string &x) : string {x} {}

        URI (const data::ASCII &protocol, const data::UTF8 &user_utya,
             const data::UTF8 &host, const data::ASCII &port, const net::path &path,
             const data::UTF8 &query, const data::UTF8 &fragment);

        // convert to a standard equivalent form in which all hex digits are upper case,
        // scheme and host are lower case, and only required digits are hex-encoded.
        // this is used to test equality of URLs.
        URI normalize () const;

    };
    /* funny mnemonic to remember which parts of a URL can be percent encoded.

        "Ugly Hippos Push Strange Quests, Possibly Even Frightening Frilly Unicorns!" 🦄

        Each word stands for a part of a URL:

        Ugly → Userinfo (username:password@)

        Hippos → Host (only percent-encoded for IPv6 literals in some contexts — usually not encoded)

        Push → Path

        Strange → Search (query string)

        Quests → Query (alt word for search—helps reinforce)

        Possibly → Port (rarely encoded; mostly numeric, but technically valid)

        Even → Encoded path segments

        Frightening → Fragment

        Frilly → (for double “F” mnemonic)

        Unicorns → (Unifying idea — "URLs are weird but lovable")
    */

}

namespace data::net {

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

            // insecure
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

            ptr<pctstr> Path;
            ptr<pctstr> Query;
            ptr<pctstr> Fragment;
        };

        static ASCII write_path (list<UTF8>);

    private:
        make read () const;
    };

    // path must be a percent encoded string because
    // since percent encoding is optional, it is possible
    // that a delimeter character is percent encoded,
    // which would change structure of the path if it
    // were decoded.
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

namespace data::encoding::percent {

    bool inline operator == (const URI &a, const URI &b) {
        return static_cast<string> (a.normalize ()) == static_cast<string> (b.normalize ());
    }

    std::ostream inline &operator << (std::ostream &o, const URI &u) {
        return o << static_cast<string> (u);
    }

    data::ASCII inline URI::scheme () const {
        string_view x = scheme (*this);
        if (x.data () == nullptr) throw exception {} << "invalid URI " << *this;
        return data::ASCII {x};
    }

    net::path inline URI::path () const {
        string_view x = path (*this);
        if (x.data () == nullptr) throw exception {} << "invalid URI " << *this;
        return net::path {x};
    }

    maybe<data::UTF8> inline URI::authority () const {
        string_view x = authority (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

    maybe<data::UTF8> inline URI::user_info () const {
        string_view x = user_info (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

    maybe<data::UTF8> inline URI::host () const {
        string_view x = host (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

    maybe<data::ASCII> inline URI::port () const {
        string_view x = port (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    maybe<data::ASCII> inline URI::query () const {
        string_view x = query (*this);
        if (x.data () == nullptr) return {};
        return {data::ASCII {x}};
    }

    maybe<data::UTF8> inline URI::fragment () const {
        string_view x = fragment (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

}

namespace data::net::IP::TCP {

    inline endpoint::endpoint (const IP::address &addr, uint16 port) : URL {URL::make {}.protocol ("tcp").address (addr).port (port)} {}
    inline endpoint::endpoint (const char *x) : URL {x} {}
    inline endpoint::endpoint (const std::string &x) : URL {x} {}

    bool inline endpoint::valid () const {
        return URL::valid () && this->protocol () == protocol::TCP && bool (this->port_number ()) &&
            bool (static_cast<const URL *> (this)->address ()) && !bool (this->user_info ()) &&
            !bool (this->fragment ()) && !bool (this->query ());
    }

    IP::address inline endpoint::address () const {
        return *static_cast<const URL *> (this)->address ();
    }

    uint16 inline endpoint::port () const {
        return *this->port_number ();
    }

    inline endpoint::operator asio::ip::tcp::endpoint () const {
        return valid () ? asio::ip::tcp::endpoint {
            asio::ip::make_address (this->address ()),
            this->port ()
        } : asio::ip::tcp::endpoint {};
    }
}

#endif
