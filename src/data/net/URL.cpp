// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/URL.hpp>
#include <data/net/TCP.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <iomanip>

namespace data {

    bool case_insensitive_equal (const string &a, const string &b) {

        if (a.size () != b.size ()) return false;

        auto ait = a.begin ();
        auto bit = b.begin ();
        while (ait != a.end ()) {
            if (std::tolower (*ait) != std::tolower (*bit)) return false;
            ait++;
            bit++;
        }

        return true;
    }
}

namespace data::encoding::percent {

    bool valid (string_view input) {
        if (!ASCII::valid (input)) return false;

        for (size_t i = 0; i < input.size (); ++i) {

            char c = input[i];

            // must not be a control character.
            if (c <= 32 || c == 127) return false;

            // Check if the character is a percent sign.
            if (c == '%') {
                // Make sure there are at least two more characters after the percent sign.
                if (i + 2 >= input.size ())
                    return false;

                // Check that the next two characters are valid hexadecimal digits.
                if (!std::isxdigit (static_cast<unsigned char> (input[i + 1])) ||
                    !std::isxdigit (static_cast<unsigned char> (input[i + 2])))
                    return false;

                // Skip the next two characters (the hexadecimal digits).
                i += 2;
            }
        }

        return true;
    }

    bool operator == (const string &a, const string &b) {

        if (!a.valid () || !b.valid ()) return false;

        auto a_iter = a.begin ();
        auto b_iter = b.begin ();

        while (a_iter != a.end () && b_iter != b.end ()) {
            char a_char = *a_iter;
            char b_char = *b_iter;

            int a_decoded;
            int b_decoded;

            if (a_char == '%') {
                a_decoded = (std::tolower (*(a_iter + 1)) - '0') * 16 + (std::tolower (*(a_iter + 2)) - '0');
                a_iter += 3;
            } else {
                a_decoded = a_char;
                a_iter++;
            }

            if (b_char == '%') {
                b_decoded = (std::tolower (*(b_iter + 1)) - '0') * 16 + (std::tolower (*(b_iter + 2)) - '0');
                b_iter += 3;
            } else {
                b_decoded = b_char;
                b_iter++;
            }

            if (a_decoded != b_decoded) return false;

            if (is_reserved (a_char) && a_char != b_char) return false;
        }

        // Both iterators should reach the end of the strings at the same time.
        return a_iter == a.end () && b_iter == b.end ();
    }

    string encode (const data::UTF8 &input, const data::ASCII &additional_chars) {
        std::ostringstream encoded;
        encoded << std::hex << std::uppercase;

        for (const auto& ch : input) {
            if (ch <= 0x20 || ch >= 0x7F || std::strchr (additional_chars.c_str (), ch) != nullptr)
                encoded << '%' << std::setw (2) << static_cast<int> (static_cast<unsigned char> (ch));
            else encoded << ch;
        }

        return string {encoded.str ()};
    }

    unsigned char from_hex (string_view str) {
        int value;

        if (std::sscanf (str.data (), "%2x", &value) != 1) throw std::invalid_argument ("Invalid hex encoding");

        return static_cast<unsigned char> (value);
    }

    maybe<data::UTF8> decode (string_view input) {
        if (!valid (input)) return {};
        data::UTF8 rt {};

        std::ostringstream decoded;

        for (std::size_t i = 0; i < input.size (); ++i)
            if (input[i] == '%' && i + 2 < input.size ()) {
                decoded << from_hex (input.substr (i + 1, 2));
                i += 2;
            } else {
                decoded << input[i];
            }

        return {decoded.str ()};
    }

}

namespace data::net {
    namespace {
        string write_params (list<entry<UTF8, UTF8>> params) {
            std::stringstream q;
            q << "?";

            auto i = params.begin ();

            while (true) {
                q << encoding::percent::encode ((*i).Key, ":#[]@=&") << "=" <<
                    encoding::percent::encode ((*i).Value, ":#[]@=&");
                i++;
                if (i == params.end ()) break;
                q << "&";
            }

            return q.str ();
        }
    }

    protocol::operator name () const {
        string self = *this;

        boost::to_lower (self);

        if (self == "ftp") return FTP;
        if (self == "http") return HTTP;
        if (self == "https") return HTTPS;
        if (self == "ws") return WS;
        if (self == "wss") return WSS;
        return unknown;
    }

    protocol protocol::encode (name n) {
        if (n == FTP) return "ftp";
        if (n == HTTP) return "http";
        if (n == HTTPS) return "https";
        if (n == WS) return "ws";
        if (n == WSS) return "wss";
        return "";
    }

    URL::make::operator URL () const {
        if (!Protocol) throw exception {"invalid URI; no protocol given"};

        std::stringstream url;
        url << *Protocol << ":";

        if (Authority) {
            url << "//";

            if (Authority->UserInfo) url << *Authority->UserInfo << "@";
            if (Authority->Host) url << *Authority->Host;
            if (Authority->Port) url << ":" << *Authority->Port;
        }

        if (Query) url << *Query;
        if (Fragment) url << *Fragment;

        return URL {url.str ()};

    }

    URL::make URL::make::protocol (const net::protocol &p) const {
        if (Protocol != nullptr) throw exception {"URL error: protocol already set."};
        if (!p.valid ()) throw exception {"URL error: Invalid protocol."};

        make m = *this;
        m.Protocol = std::make_shared<pctstr> (p);
        return m;
    }

    URL::make URL::make::scheme (const ASCII &scheme) const {
        if (!qi::parse (scheme.data (), scheme.data () + scheme.size (), parser {}.Scheme))
            throw exception {"URL error: invalid scheme."};

        return protocol (net::protocol {scheme.substr (0, scheme.size () - 1)});
    }

    URL::make URL::make::authority (const ASCII &authority) const {
        if (Authority != nullptr) throw exception {"URL error: protocol already set."};

        if (!qi::parse (authority.data (), authority.data () + authority.size (), parser {}.Authority))
            throw exception {"URL error: invalid authority."};

        make m = *this;
        m.Authority = make::make_authority::read (authority);
        return m;
    }

    URL::make URL::make::query (list<entry<UTF8, UTF8>> p) const {
        if (Query != nullptr) throw exception {"URL error: query already set."};

        make m = *this;
        m.Query = std::make_shared<pctstr> (write_params (p));
        return m;
    }
/*
    URL::make URL::make::fragment (const ASCII &f) const {
        if (Fragment != nullptr) throw exception {"URL error: fragment already set."};

        if (!qi::parse (f.data (), f.data () + f.size (), parser {}.Fragment))
            throw exception {"URL error: invalid fragment."};

        make m = *this;
        m.Fragment = std::make_shared<pctstr> (f);
        return m;
    }*/

    URL::make URL::make::query (const ASCII &q) const {
        if (Query != nullptr) throw exception {"URL error: query already set."};

        make m = *this;
        m.Query = std::make_shared<pctstr> (q);
        return m;
    }

    URL::make URL::make::fragment (const UTF8 &u) const {
        if (Fragment != nullptr) throw exception {"URL error: fragment already set."};

        std::stringstream z;
        z << "#" << encoding::percent::encode (u, ":#[]@=&");

        make m = *this;
        m.Fragment = std::make_shared<pctstr> (z.str ());
        return m;
    }

    URL::make URL::make::port (const uint16 &u) const {

        make m = *this;
        if (m.Authority == nullptr) m.Authority = std::make_shared<make_authority> ();
        if (m.Authority->Port != nullptr) throw exception {"URL error: port already set."};

        m.Authority->Port = std::make_shared<pctstr> (net::port {u});
        return m;
    }

    URL::make URL::make::host_ip_address (const IP::address &ip) const {

        make m = *this;
        if (m.Authority == nullptr) m.Authority = std::make_shared<make_authority> ();
        if (m.Authority->Host != nullptr) throw exception {"URL error: host already set."};
        if (ip.valid ()) throw exception {"URL error: Invalid IP address"};

        m.Authority->Host = std::make_shared<pctstr> (ip);
        return m;
    }

    URL::make URL::make::host_domain_name (const domain_name &name) const {
        make m = *this;
        if (m.Authority == nullptr) m.Authority = std::make_shared<make_authority> ();
        if (m.Authority->Host != nullptr) throw exception {"URL error: host already set."};
        if (name.valid ()) throw exception {"URL error: Invalid domain name"};

        m.Authority->Host = std::make_shared<pctstr> (name);
        return m;
    }

    URL::make URL::make::user_info (const UTF8 &username, const UTF8 &pass) const {

        make m = *this;
        if (m.Authority == nullptr) m.Authority = std::make_shared<make_authority> ();
        if (m.Authority->UserInfo != nullptr) throw exception {"URL error: user info already set."};

        std::stringstream user_info;
        user_info << encoding::percent::encode (username, ":/@?#") << ":" << encoding::percent::encode (pass, ":/@?#");

        m.Authority->UserInfo = std::make_shared<pctstr> (user_info.str ());
        return m;
    }

    URL::make URL::make::path (const net::path &p) const {
        if (Path != nullptr) throw exception {"URL error: path already set."};

        make m = *this;
        m.Path = std::make_shared<pctstr> (p);
        return m;
    }

    path::path (list<UTF8> uf, char delim) {
        if (delim < 32 || delim >= 127 || strchr ("/?#[]%", delim) != nullptr)
            throw exception {} << "invalid path delimiter " << delim;

        std::string to_encode = std::string {"/?#[]%"} + delim;

        std::stringstream p;

        for (const UTF8 &u : uf) p << delim << encoding::percent::encode (u, ASCII {to_encode});

        *this = encoding::percent::string {p.str ()};

    }

    list<UTF8> path::read (char delim) const {
        if (delim < 32 || delim >= 127 || strchr ("/?#[]", delim) != nullptr)
            throw exception {} << "invalid path delimiter " << delim;

        list<UTF8> x;

        list<std::string> zz = split (*this, string {} + delim);

        for (const std::string &z : zz) {
            auto meep = encoding::percent::decode (z);
            if (!meep) throw exception {} << "invalid percent-encoded string";
            x = x << *meep;
        }

        return x;
    }

}

namespace data::net::IP {

    address::operator asio::ip::address () const {
        asio::error_code err {};
        auto addr = asio::ip::make_address (static_cast<string> (*this), err);
        if (err) throw exception {err};
        return addr;
    }

    namespace TCP {
/*
        endpoint::operator asio::ip::tcp::endpoint () const {
            auto result = ctre::match<pattern> (*this);
            if (!bool(result)) return {};
            auto address_v4 = result.get<"V4"> ();
            std::stringstream port_stream;
            port_stream << result.get<"port"> ().to_string ();
            uint16 port;
            port_stream >> port;
            auto addr_v4_string = address_v4.to_string();
            auto addr_v6_string = result.get<"V6"> ().to_string ();
            auto addr_string = bool (!address_v4) ?  addr_v6_string : addr_v4_string;
            auto addr=asio::ip::address::from_string (addr_string);
            return asio::ip::tcp::endpoint{
                addr,
                port};
        }*/
    }

}

#include <boost/phoenix.hpp>

namespace data::net {
    IP::address::parser::parser () : parser::base_type (IPAddress) {
        using qi::uint_parser;
        using qi::char_;
        using qi::repeat;
        using qi::lit;

        ipv4_octet = qi::char_ ('0') | (qi::char_('1', '9') >> -qi::digit) |
            (qi::char_ ('1') >> qi::digit >> qi::digit) | (qi::char_ ('2') >> qi::char_ ('0', '4') >> qi::digit) |
            (qi::char_ ('2') >> qi::char_ ('5') >> qi::char_ ('0', '5'));

        IPv4 = ipv4_octet >> qi::char_ ('.') >> ipv4_octet >> qi::char_ ('.') >> ipv4_octet >> qi::char_ ('.') >> ipv4_octet;

        h16 = repeat (1, 4)[char_ ("0-9a-fA-F")];
        ls32 = (h16 >> ':' >> h16) | IPv4;

        IPv6 = -(repeat (6)[h16 >> ':']) >> ls32
            | "::" >> repeat (5)[h16 >> ':'] >> ls32
            | -repeat (1)[h16] >> "::" >> repeat (4)[h16 >> ':'] >> ls32
            | -repeat (2)[h16 >> ':'] >> "::" >> repeat (3)[h16 >> ':'] >> ls32
            | -repeat (3)[h16 >> ':'] >> "::" >> repeat (2)[h16 >> ':'] >> ls32
            | -repeat (4)[h16 >> ':'] >> "::" >> h16 >> ':' >> ls32
            | -repeat (5)[h16 >> ':'] >> "::" >> ls32
            | -repeat (6)[h16 >> ':'] >> "::" >> h16
            | -repeat (7)[h16 >> ':'] >> "::";

        IPFuture = "v" >> +char_ ("0-9a-fA-F") >> "." >> +char_ ("!#$%&'*+-._`|~0-9a-zA-Z();/?:@&=+$,[]");

        IPAddress = IPv4 | IPv6 | IPFuture;

        WholeIPAddress = IPAddress >> qi::eoi;
        WholeIPv4 = IPv4 >> qi::eoi;
        WholeIPv6 = IPv6 >> qi::eoi;
        WholeIPFuture = IPFuture >> qi::eoi;
    }

    domain_name::parser::parser () : parser::base_type (Name) {
        using qi::char_;
        using qi::raw;

        // Domain name components should start with an alphanumeric character,
        // followed by any combination of alphanumeric characters, hyphens, or underscores.
        Label = raw[(char_ ("a-zA-Z0-9") >> *(char_ (R"(a-zA-Z0-9-)")))];

        // Labels separated by dots
        Name = Label % '.';

        Whole = Name >> qi::eoi;
    }

    URL::make URL::read () const {

        parser p {};

        string_view scheme;
        string_view user_info;
        string_view host;
        string_view port;
        string_view path;
        string_view query;
        string_view fragment;

        p.Scheme[([&scheme] (const char *b, const char *e) {
            scheme = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.user_info[([&user_info] (const char *b, const char *e) {
            user_info = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.Host[([&host] (const char *b, const char *e) {
            host = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.port[([&port] (const char *b, const char *e) {
            port = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.path[([&path] (const char *b, const char *e) {
            path = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.Query[([&query] (const char *b, const char *e) {
            query = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.Fragment[([&fragment] (const char *b, const char *e) {
            fragment = string_view {b, static_cast<size_t> (e - b)};
        })];

        if (!qi::parse (this->data (), this->data () + this->size (), p.WholeURI)) return {};

        make m {};

        m.Protocol = std::make_shared<encoding::percent::string> (scheme);

        if (user_info.size () > 0 || host.size () > 0 || port.size () > 0) {
            m.Authority = std::make_shared<make::make_authority> ();

            if (user_info.size () > 0) m.Authority->UserInfo = std::make_shared<encoding::percent::string> (user_info);
            if (host.size () > 0) m.Authority->Host = std::make_shared<encoding::percent::string> (host);
            if (port.size () > 0) m.Authority->Port = std::make_shared<encoding::percent::string> (port);
        }

        if (path.size () > 0) m.Path = std::make_shared<encoding::percent::string> (path);
        if (query.size () > 0) m.Query = std::make_shared<encoding::percent::string> (query);
        if (fragment.size () > 0) m.Fragment = std::make_shared<encoding::percent::string> (fragment);

        return m;
    }

    maybe<uint16> URL::port_number () const {
        auto zp = this->port ();
        if (!zp) return {};
        maybe<uint16> u {0};
        *u = std::stoi (*zp);
        return u;
    }

    maybe<IP::address> URL::host_address () const {
        auto hh = this->host ();
        if (!hh) return {};
        maybe<IP::address> ip {*hh};
        if (!ip->valid ()) return {};
        return ip;
    }

    ptr<URL::make::make_authority> URL::make::make_authority::read (const ASCII &u) {

        string_view port;
        string_view user_info;
        string_view host;

        parser p;

        p.port[([&port] (const char *b, const char *e) {
            port = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.user_info[([&user_info] (const char *b, const char *e) {
            user_info = string_view {b, static_cast<size_t> (e - b)};
        })];

        p.Host[([&host] (const char *b, const char *e) {
            host = string_view {b, static_cast<size_t> (e - b)};
        })];

        if (!qi::parse (u.data (), u.data () + u.size (), p.Authority)) return nullptr;

        ptr<make_authority> auth = std::make_shared<make_authority> ();

        if (host.size () > 0) auth->Host = std::make_shared<pctstr> (host);
        if (user_info.size () > 0) auth->UserInfo = std::make_shared<pctstr> (user_info);
        if (host.size () > 0) auth->Port = std::make_shared<pctstr> (host);

        return auth;

    }
}

namespace data::encoding::percent {

    URI::parser::parser () : parser::base_type (URI), DomainName {}, IPAddress {} {

        delimiter = qi::char_ (":/?#[]@");
        subdelimiter = qi::char_ ("!$&'()*+,;=");
        reserved = delimiter | subdelimiter;
        unreserved = qi::alnum | qi::digit | qi::char_ ("+-.~");
        hex_char = qi::char_ ("0-9a-fA-F");
        ptc_encoded = "%" >> hex_char >> hex_char;
        pchar = unreserved | ptc_encoded | subdelimiter | qi::char_ (":@");
        segment = *pchar;
        final_segment = *(pchar | qi::char_ ("/?"));
        port = *qi::digit;
        path = *("/" >> segment);
        path_rootless = +pchar >> path;
        path_absolute = "/" >> -path_rootless;
        path_no_scheme = +(unreserved | ptc_encoded | subdelimiter | qi::char_ ("@")) >> path;
        hierarchical = -("//" >> Authority >> path) | path_absolute | path_no_scheme | path_rootless;
        user_info = +(unreserved | ptc_encoded | subdelimiter | qi::char_ (":"));

        Host = IPAddress | DomainName | *(unreserved | ptc_encoded | subdelimiter);

        Scheme = qi::no_case[qi::alpha >> *(qi::alnum | qi::char_ ("+-."))];

        Authority = -(user_info >> "@") >> Host >> -(":" >> port);

        Query = "?" >> final_segment;

        Fragment = "#" >> final_segment;

        URI = Scheme >> ":" >> hierarchical >> -("?" >> Query) >> -("#" >> Fragment);
        WholeURI = URI >> qi::eoi;
    }

    string_view URI::scheme (string_view x) {
        parser p {};

        string_view z;

        p.Scheme[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::authority (string_view x) {
        parser p {};

        string_view z;

        p.Authority[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::path (string_view x) {
        parser p {};

        string_view z;

        p.path[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::query (string_view x) {
        parser p {};

        string_view z;

        p.Query[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::fragment (string_view x) {
        parser p {};

        string_view z;

        p.Fragment[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::user_data (string_view x) {
        parser p {};

        string_view z;

        p.user_info[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::host (string_view x) {
        parser p {};

        string_view z;

        p.Host[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

    string_view URI::port (string_view x) {
        parser p {};

        string_view z;

        p.port[([&z] (const char *b, const char *e) {
            z = string_view {b, static_cast<size_t> (e - b)};
        })];

        qi::parse (x.begin (), x.end (), p);

        return z;
    }

}
