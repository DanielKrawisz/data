
// Copyright (c) 2027 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_URL
#define DATA_NET_URL

#include <ctre.hpp>
#include <data/tools.hpp>
#include <data/encoding/ascii.hpp>

namespace data::net {

    struct URL;

    std::ostream &operator << (std::ostream &, const URL &);

    bool operator == (const URL &, const URL &);

    struct domain_name;

    std::ostream &operator << (std::ostream &, const domain_name &);

    bool operator == (const domain_name &, const domain_name &);

    struct port;

    std::ostream &operator << (std::ostream &, const port &);

    bool operator == (const port &, const port &);

    struct protocol;

    std::ostream &operator << (std::ostream &, const protocol &);

    bool operator == (const protocol &, const protocol &);

    port default_port (protocol);

    struct path;

    std::ostream &operator << (std::ostream &, const protocol &);

    bool operator == (const protocol &, const protocol &);
}

namespace data::net::IP {

    struct address;

    std::ostream &operator << (std::ostream &, const address &);

    bool operator == (const address &, const address &);

}

namespace data::net::IP::TCP {

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

    struct port : ASCII {
        static constexpr auto pattern = ctll::fixed_string {R"([0-9]*)"};
        //using ASCII::ASCII;
        port (const ASCII &x) : ASCII {x} {}

        port (uint16 number) : ASCII {std::to_string (number)} {}

        bool valid () const;
    };

    struct domain_name : ASCII {
        using ASCII::ASCII;
        domain_name (const ASCII &x) : ASCII {x} {
            std::cout << "  ...set demain name to " << *this << std::endl;
        }

        static bool valid (string_view);
        bool valid () const;
    };
}

#include <data/net/asio/socket.hpp>

namespace data::net::IP {

    struct address : UTF8 {

        using UTF8::UTF8;
        address (const UTF8 &x) : UTF8 {x} {}

        bool valid () const;
        int32 version () const;

        explicit operator bytes () const;
        operator asio::ip::address () const;
    };
}

// RFC3986 defines certain reserved characters which
// denote the structure of a URI. Percent encoding
// ensures that arbitrary UTF8 strings can be included
// in a URL without affecting the structure of the URL.
//
// A  UTF8 character is percent encoded as "%" followed
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

    // a %-encoded string.
    struct string;

    // If a character is reserved, then its percent-encoded
    // form is unequal to its non-encoded form. Otherwise
    // they are treated as equal.
    bool operator == (const string &, const string &);

    // decode back to UTF8.
    maybe<data::UTF8> decode (string_view);

    struct string : data::ASCII {
        using data::ASCII::ASCII;

        string (const data::ASCII &x) : data::ASCII {x} {}

        bool valid () const {
            return percent::valid (*this);
        }
    };

    // https://www.ietf.org/rfc/rfc3986.txt
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
        net::path path () const;
        maybe<data::ASCII> query () const;
        maybe<data::UTF8> fragment () const;

        bool valid () const;

        using string::string;
        URI (const string &x) : string {x} {}

        URI (const data::ASCII &protocol, const data::UTF8 &user_utya,
             const data::UTF8 &host, const data::ASCII &port, const net::path &path,
             const data::UTF8 &query, const data::UTF8 &fragment);

        // convert to a standard equivalent form in which all hex digits are upper case,
        // scheme and host are lower case, and only required digits are hex-encoded.
        URI normalize () const;

    };

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
        maybe<list<entry<UTF8, UTF8>>> query_map () const;

        // get user info as <username>:<password>.
        // (This is insecure but supported because we sometimes still use it.)
        maybe<entry<UTF8, UTF8>> user_name_pass () const;

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

            make authority (const ASCII &) const;

            make path (const net::path &) const;

            make query_map (list<entry<UTF8, UTF8>>) const;
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

    struct path : encoding::percent::string {
        using encoding::percent::string::string;
        path (const encoding::percent::string &x) : encoding::percent::string {x} {}
        path (list<UTF8>, char delim = '/');
        list<UTF8> read (char delim = '/') const;
    };
}

namespace data::net::IP::TCP {

    struct endpoint : URL {
        endpoint (const char *x) : URL {x} {}
        endpoint (const std::string &x) : URL {x} {}
        endpoint (const IP::address &addr, uint16 port) : URL {URL::make {}.protocol ("tcp").address (addr).port (port)} {}

        bool valid () const {
            return URL::valid () && this->protocol () == protocol::TCP && bool (this->port_number ()) &&
                bool (static_cast<const URL *> (this)->address ()) && !bool (this->user_info ()) &&
                !bool (this->fragment ()) && !bool (this->query ());
        }

        IP::address address () const {
            return *static_cast<const URL *> (this)->address ();
        }

        uint16 port () const {
            return *this->port_number ();
        }

        operator asio::ip::tcp::endpoint () const {
            return valid () ? asio::ip::tcp::endpoint {
                asio::ip::address::from_string (this->address ()),
                this->port ()
            } : asio::ip::tcp::endpoint {};
        }
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

#endif
