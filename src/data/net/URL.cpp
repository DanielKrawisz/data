// Copyright (c) 2021 Katrina Knight
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/algorithm/string.hpp>
#include <data/net/URL.hpp>
#include <data/net/TCP.hpp>
#include <data/numbers.hpp>
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

    std::string encode (const data::UTF8 &input, const data::ASCII &additional_chars) {
        std::ostringstream encoded;
        encoded << std::hex << std::uppercase;

        for (const auto& ch : input) {
            if (ch <= 0x20 || ch >= 0x7F || std::strchr (additional_chars.c_str (), ch) != nullptr)
                encoded << '%' << std::setw (2) << static_cast<int> (static_cast<unsigned char> (ch));
            else encoded << ch;
        }

        return encoded.str ();
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
            if (input[i] == '%') {
                if (i + 2 >= input.size ()) return {};

                if (!std::isxdigit (static_cast<unsigned char> (input[i + 1])) ||
                    !std::isxdigit (static_cast<unsigned char> (input[i + 2])))
                    return {};

                decoded << from_hex (input.substr (i + 1, 2));
                i += 2;
            } else decoded << input[i];

        return {decoded.str ()};
    }

    maybe<data::ASCII> decode_not_reserved (string_view input) {

        if (!valid (input)) return {};
        data::UTF8 rt {};

        std::ostringstream decoded;

        for (std::size_t i = 0; i < input.size (); ++i)
            if (input[i] == '%') {

                if (i + 2 >= input.size ()) return {};

                if (!std::isxdigit (static_cast<unsigned char> (input[i + 1])) ||
                    !std::isxdigit (static_cast<unsigned char> (input[i + 2])))
                    return {};

                char ch = from_hex (input.substr (i + 1, 2));

                if (ch <= 0x20 || ch >= 0x7F || std::strchr (Reserved, ch) != nullptr) decoded << input[i];
                else {
                    decoded << ch;
                    i += 2;
                }
            } else decoded << input[i];

        return {decoded.str ()};
    }

}

namespace data::net {

    uint16 default_port (const protocol &p) {
        switch (protocol::name (p)) {
            default: return 0;
            case (protocol::FTP) : return 20;
            case (protocol::HTTP) : return 80;
            case (protocol::HTTPS) : return 443;
        }
    }

    maybe<IP::TCP::endpoint> URL::endpoint () const {
        auto addr = this->address ();
        if (!addr) return {};
        auto p = this->port_number ();
        uint16 port = bool (p) ? *p : default_port (this->protocol ());
        if (port == 0) return {};
        return {IP::TCP::endpoint {*addr, port}};
    }

    namespace {
        std::string write_params (dispatch<UTF8, UTF8> params) {
            std::stringstream q;

            auto i = params.begin ();
            
            if (i != params.end ()) {

                while (true) {
                    q << encoding::percent::encode ((*i).Key, ":#[]@=&") << "=" <<
                        encoding::percent::encode ((*i).Value, ":#[]@=&");
                    i++;
                    if (i == params.end ()) break;
                    q << "&";
                }
            }

            return q.str ();
        }
    }

    protocol::operator name () const {
        string self = *this;

        boost::to_lower (self);

        if (self == "ftp") return FTP;
        if (self == "tcp") return TCP;
        if (self == "http") return HTTP;
        if (self == "https") return HTTPS;
        if (self == "ws") return WS;
        if (self == "wss") return WSS;
        return unknown;
    }

    protocol protocol::encode (name n) {

        if (n == FTP) return "ftp";
        if (n == TCP) return "tcp";
        if (n == HTTP) return "http";
        if (n == HTTPS) return "https";
        if (n == WS) return "ws";
        if (n == WSS) return "wss";
        return "";

    }

    URL::make::operator URL () const {
        if (!Protocol) throw exception {"invalid URI; no protocol given"};

        std::stringstream url;
        url << static_cast<std::string> (*Protocol) << ":";

        if (UserInfo || Host || Port) {
            url << "//";

            if (UserInfo) url << static_cast<std::string> (*UserInfo) << "@";
            if (Host) url << static_cast<std::string> (*Host);
            if (Port) url << ":" << static_cast<std::string> (*Port);
        }

        if (Path) url << static_cast<std::string> (*Path);
        if (Query) url << "?" << static_cast<std::string> (*Query);
        if (Fragment) url << "#" << static_cast<std::string> (*Fragment);

        URL u {url.str ()};

        if (!u.valid ()) throw exception {} << "invalid URI: " << u;

        return u;

    }

    URL::make URL::make::protocol (const net::protocol &p) const {
        if (Protocol != nullptr) throw exception {"URL error: protocol already set."};
        if (!p.valid ()) throw exception {"URL error: invalid protocol."};

        make m = *this;
        m.Protocol = std::make_shared<pctstr> (p);
        return m;
    }

    URL::make URL::make::query_map (list<entry<UTF8, UTF8>> p) const {
        if (Query != nullptr) throw exception {"URL error: query already set."};

        make m = *this;
        m.Query = std::make_shared<pctstr> (write_params (p));
        return m;
    }

    URL::make URL::make::query (const ASCII &q) const {
        if (Query != nullptr) throw exception {"URL error: query already set."};

        make m = *this;
        m.Query = std::make_shared<pctstr> (q);
        return m;
    }

    URL::make URL::make::fragment (const UTF8 &u) const {
        if (Fragment != nullptr) throw exception {"URL error: fragment already set."};

        make m = *this;
        m.Fragment = std::make_shared<pctstr> (encoding::percent::encode (u, ":#[]@=&"));
        return m;
    }

    URL::make URL::make::port (const uint16 &u) const {
        if (Port != nullptr) throw exception {"URL error: port already set."};

        make m = *this;
        m.Port = std::make_shared<pctstr> (net::port {u});
        return m;
    }

    URL::make URL::make::address (const IP::address &ip) const {
        if (Host != nullptr) throw exception {"URL error: host already set."};
        auto version = ip.version ();
        if (version != 4 && version != 6) throw exception {} << "URL error: invalid IP address " << ip;

        make m = *this;

        if (version == 4) m.Host = std::make_shared<pctstr> (ip);
        else {
            std::stringstream literal;
            literal << "[" << static_cast<std::string> (ip) << "]";
            m.Host = std::make_shared<pctstr> (literal.str ());
        }
        return m;
    }

    URL::make URL::make::domain_name (const net::domain_name &name) const {

        if (Host != nullptr) throw exception {"URL error: host already set."};
        if (!name.valid ()) throw exception {} << "URL error: invalid domain name " << name;

        make m = *this;
        m.Host = std::make_shared<pctstr> (name);
        return m;
    }

    URL::make URL::make::registered_name (const UTF8 &z) const {
        if (Host != nullptr) throw exception {"URL error: host already set."};

        make m = *this;
        m.Host = std::make_shared<pctstr> (encoding::percent::encode (z, ":/?#"));
        return m;
    }

    URL::make URL::make::user_name_pass (const UTF8 &username, const UTF8 &pass) const {

        if (UserInfo != nullptr) throw exception {"URL error: user info already set."};

        std::stringstream user_info;
        user_info << encoding::percent::encode (username, ":/@?#") << ":" << encoding::percent::encode (pass, ":/@?#");

        make m = *this;
        m.UserInfo = std::make_shared<pctstr> (user_info.str ());
        return m;
    }

    URL::make URL::make::user_info (const UTF8 &info) const {

        make m = *this;
        if (m.UserInfo != nullptr) throw exception {"URL error: user info already set."};

        m.UserInfo = std::make_shared<pctstr> (encoding::percent::encode (info, "/@?#"));
        return m;
    }

    URL::make URL::make::path (const net::path &p) const {
        if (Path != nullptr && *Path != pctstr {""}) 
            throw exception {"URL error: path already set."};

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
        if (delim < 32 || delim >= 127 || strchr ("/?#[]", delim) == nullptr)
            throw exception {} << "invalid path delimiter " << delim;

        list<UTF8> x;

        list<string_view> zz = split (*this, string {} + delim);

        for (const string_view &z : zz) {
            auto meep = encoding::percent::decode (z);
            if (!meep) throw exception {} << "invalid percent-encoded string";
            x = x << *meep;
        }

        return x;
    }

    maybe<uint16> URL::port_number () const {
        auto zp = this->port ();
        if (!zp) return {};
        maybe<uint16> u {0};
        *u = std::stoi (*zp);
        return u;
    }

    maybe<IP::address> URL::address () const {
        auto hh = URI::address (*this);
        if (hh.data () == nullptr) return {};
        maybe<IP::address> ip {hh};
        if (!ip->valid ()) return {};
        return ip;
    }

    maybe<entry<UTF8, UTF8>> URL::user_name_pass () const {
        string_view hh = URI::user_info (*this);
        if (hh.data () == nullptr) return {};
        list<string_view> z = split (hh, ":");
        if (z.size () != 2) return {};
        return {entry<UTF8, UTF8> {*encoding::percent::decode (z[0]), *encoding::percent::decode (z[1])}};
    }

    dispatch<UTF8, UTF8> read_query_map (string_view query) {
        list<string_view> z = split (query, "&");

        dispatch<UTF8, UTF8> params;

        for (const string_view &e : z) {
            list<string_view> p = split (e, "=");
            if (p.size () != 2) return {};
            params = params << entry<UTF8, UTF8> {*encoding::percent::decode (p[0]), *encoding::percent::decode (p[1])};
        }

        return params;
    }

    maybe<dispatch<UTF8, UTF8>> URL::query_map () const {
        string_view q = URI::query (*this);
        if (q.data () == nullptr) return {};

        return read_query_map (q);
    }

    maybe<domain_name> URL::domain_name () const {
        if (this->address ()) return {};
        auto dn = this-> host ();
        if (!dn) return {};
        if (net::domain_name::valid (*dn)) return {net::domain_name {*dn}};
        return {};
    }

}

namespace data::net::IP {

    address::operator asio::ip::address () const {
        asio::error err {};
        auto addr = asio::ip::make_address (static_cast<string> (*this), err);
        if (err) throw exception {err};
        return addr;
    }

}

#include <tao/pegtl.hpp>

namespace pegtl {

    using namespace tao::pegtl;

    // Rules for parsing domain names
    struct domain_label : seq<alnum, star<sor<one<'-'>, alnum>>> {};
    struct domain_name : seq<domain_label, star<one<'.'>, domain_label>> {};
    struct domain_name_whole : seq<bof, domain_name, eof> {};

    struct gen_delim : sor<one<':'>, one<'/'>, one<'?'>, one<'#'>, one<'['>, one<']'>, one<'@'>> {};

    struct sub_delim : sor<one<'!'>, one<'$'>, one<'&'>, one<'\''>,
        one<'('>, one<')'>, one<'*'>, one<'+'>, one<','>, one<';'>, one<'='>> {};

    struct reserved : sor<gen_delim, sub_delim> {};

    struct unreserved : sor<alnum, one<'-'>, one<'.'>, one<'_'>, one<'~'>> {};

    struct percent_encoded : seq<one<'%'>, xdigit, xdigit> {};

    struct pchar : sor<unreserved, percent_encoded, sub_delim, one<':'>, one<'@'>> {};

    struct segment : star<pchar> {};

    struct segment_nz : plus<pchar> {};

    struct segment_nz_nc : plus<sor<unreserved, percent_encoded, sub_delim, one<'@'>>> {};

    struct path_ab_empty : star<seq<one<'/'>, segment>> {};

    struct path_rootless : seq<segment_nz, path_ab_empty> {};

    struct path_absolute : seq<one<'/'>, segment_nz, path_ab_empty> {};

    struct path_after_authority : path_ab_empty {};

    struct reg_name : star<sor<unreserved, percent_encoded, sub_delim>> {};

    struct scheme : seq<alpha, star<sor<alnum, one<'+'>, one<'-'>, one<'.'>>>> {};

    struct whole_scheme : seq<eof, scheme, eof> {};

    struct user_info : plus<sor<unreserved, percent_encoded, sub_delim, one<':'>>> {};

    struct final_section : star<sor<pchar, one<'/'>, one<'?'>>> {};

    struct query : final_section {};

    struct fragment : final_section {};

    struct ipv4_octet : sor<
        seq<string<'2', '5'>, range<'0', '5'>>,
        seq<one<'2'>, range<'0', '4'>, digit>,
        seq<one<'1'>, digit, digit>,
        seq<range<'1', '9'>, digit>,
        digit> {};

    struct ipv4 : seq<ipv4_octet, one<'.'>, ipv4_octet, one<'.'>, ipv4_octet, one<'.'>, ipv4_octet> {};
    struct ipv4_whole : seq<bof, ipv4, eof> {};

    struct h16 : seq<xdigit, opt<xdigit>, opt<xdigit>, opt<xdigit>> {};

    struct ls32 : sor<seq<h16, one<':'>, h16>, ipv4> {};

    struct ipv6 : sor<seq<rep<6, seq<h16, one<':'>>>, ls32>,
        seq<string<':', ':'>, rep<5, seq<h16, one<':'>>>, ls32>,
        seq<opt<h16>, string<':', ':'>, rep<4, seq<h16, one<':'>>>, ls32>,
        seq<opt<seq<h16, opt<seq<one<':'>, h16>>>>, string<':', ':'>, rep<3, seq<h16, one<':'>>>, ls32>,
        seq<opt<seq<h16, rep<2, opt<seq<one<':'>, h16>>>>>, string<':', ':'>, rep<2, seq<h16, one<':'>>>, ls32>,
        seq<opt<seq<h16, rep<3, opt<seq<one<':'>, h16>>>>>, string<':', ':'>, seq<h16, one<':'>>, ls32>,
        seq<opt<seq<h16, rep<4, opt<seq<one<':'>, h16>>>>>, string<':', ':'>, ls32>,
        seq<opt<seq<h16, rep<5, opt<seq<one<':'>, h16>>>>>, string<':', ':'>, h16>,
        seq<opt<seq<h16, rep<6, opt<seq<one<':'>, h16>>>>>, string<':', ':'>>> {};

    struct ip_future : seq<istring<'v'>, plus<xdigit>, one<'.'>, plus<sor<unreserved, sub_delim, one<':'>>>> {};

    struct ip_literal : seq<one<'['>, sor<ipv6, ip_future>, one<']'>> {};

    struct ipv6_whole : seq<ipv6, eof> {};

    struct ip_address_whole : seq<sor<ipv6, ipv4>, eof> {};

    struct host : sor<ip_literal, ipv4, reg_name> {};

    struct user_info_at : seq<user_info, one<'@'>> {};

    struct port : star<digit> {};

    struct authority : seq<opt<user_info_at>, host, opt<seq<one<':'>, port>>> {};

    struct authority_whole : seq<bof, authority, eof> {};

    struct path : sor<path_after_authority, path_absolute, path_rootless> {};

    struct path_whole : seq<bof, path, eof> {};

    struct hierarchical : sor<seq<string<'/', '/'>, authority, path_after_authority>, path_absolute, path_rootless> {};

    struct target : seq<sor<path_after_authority, path_absolute, path_rootless>, opt<seq<one<'?'>, query>>, opt<seq<one<'#'>, fragment>>> {};

    struct target_whole : seq<bof, target, eof> {};

    struct uri : seq<scheme, one<':'>, hierarchical, opt<seq<one<'?'>, query>>, opt<seq<one<'#'>, fragment>>> {};

    struct uri_whole : seq<bof, uri, eof> {};

}

namespace data {

    bool net::domain_name::valid (string_view c) {
        tao::pegtl::memory_input<> in (c, "domain_name");
        return tao::pegtl::parse<pegtl::domain_name_whole> (in);
    }

    bool encoding::percent::URI::valid () const {
        tao::pegtl::memory_input<> in (*this, "uri");
        return tao::pegtl::parse<pegtl::uri_whole> (in);
    }

    bool net::IP::address::valid () const {
        tao::pegtl::memory_input<> in (*this, "ip_address");
        return tao::pegtl::parse<pegtl::ip_address_whole> (in);
    }

    bool net::authority::valid (string_view c) {
        tao::pegtl::memory_input<> in (c, "authority");
        return tao::pegtl::parse<pegtl::authority_whole> (in);
    }

    bool net::path::valid (string_view c) {
        tao::pegtl::memory_input<> in (c, "path");
        return tao::pegtl::parse<pegtl::path_whole> (in);
    }

    bool net::target::valid (string_view c) {
        tao::pegtl::memory_input<> in (c, "target");
        return tao::pegtl::parse<pegtl::target_whole> (in);
    }

    int32 net::IP::address::version () const {
        tao::pegtl::memory_input<> in (*this, "ip_address");
        if (tao::pegtl::parse<pegtl::ipv4_whole> (in)) return 4;
        if (tao::pegtl::parse<pegtl::ipv6_whole> (in)) return 6;
        return -1;
    }

    template <typename Rule> struct domain_action : pegtl::nothing<Rule> {};

    template <> struct domain_action<pegtl::domain_label> {
        template <typename Input >
        static void apply (const Input& in, data::list<std::string> &labels) {
            labels = labels << in.string ();
        }
    };

    template <typename Rule> struct make_uri_action : pegtl::nothing<Rule> {};

    template <> struct make_uri_action<pegtl::scheme> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Protocol == nullptr) m.Protocol = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::user_info_at> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.UserInfo == nullptr)
                m.UserInfo = std::make_shared<encoding::percent::string> (in.begin (), in.begin () + in.size () - 1);
        }
    };

    template <> struct make_uri_action<pegtl::host> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Host == nullptr) m.Host = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::port> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Port == nullptr) m.Port = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::path_after_authority> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Path == nullptr) m.Path = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::path_absolute> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Path == nullptr) m.Path = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::path_rootless> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Path == nullptr) m.Path = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::query> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Query == nullptr) m.Query = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    template <> struct make_uri_action<pegtl::fragment> {
        template <typename Input >
        static void apply (const Input& in, net::URL::make &m) {
            if (m.Fragment == nullptr) m.Fragment = std::make_shared<encoding::percent::string> (in.string ());
        }
    };

    net::URL::make net::URL::read () const {
        make m {};
        tao::pegtl::memory_input<> in (*this, "uri");
        return tao::pegtl::parse<pegtl::uri_whole, make_uri_action> (in, m) ? m : make {};
    }

    net::URL::make net::URL::make::authority (const UTF8 &x) const {
        if (UserInfo || Host || Port) throw exception {"URL error: authority already set."};
        make m = *this;

        UTF8 encoded = encoding::percent::encode (x);
        tao::pegtl::memory_input<> in (encoded, "authority");

        return tao::pegtl::parse<pegtl::authority_whole, make_uri_action> (in, m) ? m : make {};
    }

    template <typename Rule> struct read_scheme_action : pegtl::nothing<Rule> {};

    template <> struct read_scheme_action<pegtl::scheme> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::scheme (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "scheme");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_scheme_action> (in, sub)) return {};
        return sub;
    }

    template <typename Rule> struct read_authority_action : pegtl::nothing<Rule> {};

    template <> struct read_authority_action<pegtl::authority> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::authority (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "authority");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_authority_action> (in, sub)) return {};
        return sub;
    }

    template <typename Rule> struct read_user_info_action : pegtl::nothing<Rule> {};

    template <> struct read_user_info_action<pegtl::user_info_at> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::user_info (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "user_info");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_user_info_action> (in, sub)) return {};
        return sub.substr (0, sub.size () - 1);
    }

    template <typename Rule> struct read_host_action : pegtl::nothing<Rule> {};

    template <> struct read_host_action<pegtl::host> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::host (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "host");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_host_action> (in, sub)) return {};
        return sub;
    }

    template <typename Rule> struct read_port_action : pegtl::nothing<Rule> {};

    template <> struct read_port_action<pegtl::port> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::port (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "port");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_port_action> (in, sub)) return {};
        return sub;
    }

    template <typename Rule> struct read_path_action : pegtl::nothing<Rule> {};

    template <> struct read_path_action<pegtl::path_after_authority> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    template <> struct read_path_action<pegtl::path_absolute> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    template <> struct read_path_action<pegtl::path_rootless> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::path (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "path");
        // in theory this will not happen.
        if (!tao::pegtl::parse<pegtl::uri_whole, read_path_action> (in, sub)) return {};
        return sub;
    }

    string_view encoding::percent::URI::target (string_view x) {
        string_view p = path (x);
        // in theory this should not happen.
        if (p.data () == nullptr) return p;
        return string_view {p.data (), x.size () - (p.data () - x.data ())};
    }

    template <typename Rule> struct read_query_action : pegtl::nothing<Rule> {};

    template <> struct read_query_action<pegtl::query> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::query (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "query");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_query_action> (in, sub)) return {};
        return sub;
    }

    template <typename Rule> struct read_fragment_action : pegtl::nothing<Rule> {};

    template <> struct read_fragment_action<pegtl::fragment> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::fragment (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "fragment");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_fragment_action> (in, sub)) return {};
        return sub;
    }

    template <typename Rule> struct read_ip_address_action : pegtl::nothing<Rule> {};

    template <> struct read_ip_address_action<pegtl::ipv4> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    template <> struct read_ip_address_action<pegtl::ipv6> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    template <> struct read_ip_address_action<pegtl::ip_future> {
        template <typename Input >
        static void apply (const Input& in, string_view &x) {
            x = string_view {&*in.begin (), static_cast<size_t> (in.end () - in.begin ())};
        }
    };

    string_view encoding::percent::URI::address (string_view x) {
        string_view sub;
        tao::pegtl::memory_input<> in (x, "ip_address");
        if (!tao::pegtl::parse<pegtl::uri_whole, read_ip_address_action> (in, sub)) return {};
        return sub;
    }

    encoding::percent::URI encoding::percent::URI::normalize () const {
        if (!valid ()) throw exception {} << "invalid URI: " << *this;

        // we can do this because we know the URL is valid.
        net::URL::make m {*decode_not_reserved (*this)};

        // again, since we know the URL is valid, we can do this.
        if (!m.Protocol) throw exception {} << "No protocol read in " << *this;

        for (char &x : *m.Protocol) x = std::tolower (x);

        if (m.Host) for (char &x : *m.Host) x = std::tolower (x);
        return static_cast<URI> (net::URL (m));
    }

    net::path net::target::path () const {
        string_view sub;
        tao::pegtl::memory_input<> in (*this, "path");
        // in theory this will not happen.
        if (!tao::pegtl::parse<pegtl::target_whole, read_path_action> (in, sub))
            throw data::exception {"invalid target"};
        return net::path {sub};
    }

    maybe<ASCII> net::target::query () const {
        string_view sub;
        tao::pegtl::memory_input<> in (*this, "query");
        if (!tao::pegtl::parse<pegtl::target_whole, read_query_action> (in, sub))
            throw data::exception {"invalid target"};
        if (sub.data () == nullptr) return {};
        return ASCII {sub};
    }

    maybe<list<entry<UTF8, UTF8>>> net::target::query_map () const {
        maybe<ASCII> q = query ();

        if (!bool (q)) return {};

        return read_query_map (*q);
    }

    maybe<UTF8> net::target::fragment () const {
        string_view sub;
        tao::pegtl::memory_input<> in (*this, "fragment");
        if (!tao::pegtl::parse<pegtl::target_whole, read_fragment_action> (in, sub))
            throw data::exception {"invalid target"};
        if (sub.data () == nullptr) return {};
        return encoding::percent::decode (sub);
    }

    maybe<ASCII> net::authority::port () const {
        string_view sub;
        tao::pegtl::memory_input<> in (*this, "port");
        if (!tao::pegtl::parse<pegtl::authority_whole, read_port_action> (in, sub))
            throw data::exception {"invalid authority"};
        if (sub.data () == nullptr) return {};
        return ASCII {sub};
    }

    maybe<UTF8> net::authority::host () const {
        string_view sub;
        tao::pegtl::memory_input<> in (*this, "host");
        if (!tao::pegtl::parse<pegtl::authority_whole, read_host_action> (in, sub))
            throw data::exception {"invalid authority"};
        if (sub.data () == nullptr) return {};
        return encoding::percent::decode (sub);
    }

    maybe<net::IP::address> net::authority::address () const {
        string_view sub;
        tao::pegtl::memory_input<> in (*this, "ip_address");
        if (!tao::pegtl::parse<pegtl::authority_whole, read_ip_address_action> (in, sub))
            throw data::exception {"invalid authority"};
        if (sub.data () == nullptr) return {};
        return encoding::percent::decode (sub);
    }

    maybe<uint16> net::authority::port_number () const {
        auto zp = this->port ();
        if (!zp) return {};
        maybe<uint16> u {0};
        *u = std::stoi (*zp);
        return u;
    }

    maybe<net::IP::TCP::endpoint> net::authority::endpoint () const {
        maybe<uint16> p = port_number ();
        maybe<net::IP::address> a = address ();
        if (!bool (p) || !bool (a)) return {};
        return net::IP::TCP::endpoint {*a, *p};
    }

}

namespace data::net::IP {

    // read the ip address as a series of bites.
    address::operator bytes () const {
        auto v = version ();
        if (v == -1) return {};

        auto pi = this->begin ();

        // version 4 address
        if (v == 4) {
            bytes b (4);
            auto bi = b.begin ();
            byte octet = 0;

            while (true) {
                if (pi == this->end () || *pi == '.') {
                    *bi = octet;
                    bi++;

                    if (bi == b.end ()) return b;

                    octet = 0;
                } else {
                    octet *= 10;
                    octet += static_cast<byte> (*pi - '0');
                }

                // we know we are not at the end because we
                // know that this is a vaild ipv4 address.
                pi++;
            }
        }

        // otherwise, version 6 address.

        // an initial ':' must be an initial "::". We skip ahead.
        if (*pi == ':') pi = pi += 1;

        // ports to the left of the "::".
        list<uint16_big> ports_left {};

        // ports to the right of the "::"
        list<uint16_big> ports_right {};

        list<uint16_big> *ports = &ports_left;

        // list of octet bytes which are optionally at the end.
        list<byte> octets;

        // whether we have encountered an IPV4 ending.
        bool ipv4_ending = false;
        while (true) {

            auto pb = pi;

            while (pi != this->end () && *pi != ':' && *pi != '.') pi++;

            auto end = pi == this->end ();
            if (pi - pb == 0) {

                ports = &ports_right;

                // this means that the "::" we already found is at the end of the ip address.
                if (end) break;

                // otherwise we have found a "::"
                pi++;
                continue;
            }

            if (!end && *pi == '.') ipv4_ending = true;

            if (ipv4_ending) {
                byte octet = 0;

                while (pb != pi) {
                    octet *= 10;
                    octet += static_cast<byte> (*pb - '0');
                    pb++;
                }

                octets <<= octet;
            } else {
                uint16_big port = 0;

                while (pb != pi) {
                    port <<= 4;
                    if (*pb <= 'F' && *pb >= 'A') port += static_cast<uint16> (*pb - 'A' + 10);
                    else if (*pb <= 'f' && *pb >= 'a') port += static_cast<uint16> (*pb - 'a' + 10);
                    else port += static_cast<uint16> (*pb - '0');
                    pb++;
                }

                *ports <<= port;
            }

            if (end) break;
            pi++;

        }

        bytes b (16);

        // now we can figure out how many zeros we need to fill in.
        int zeros = 8 - (data::size (ports_left) + data::size (ports_right) + data::size (octets) / 2);

        auto bi = b.begin ();
        for (uint16_big upl : ports_left) {
            *bi = upl[0];
            bi++;
            *bi = upl[1];
            bi++;
        }

        for (int i = 0; i < zeros; i++) {
            *bi = 0;
            bi++;
            *bi = 0;
            bi++;
        }

        for (uint16_big upr : ports_right) {
            *bi = upr[0];
            bi++;
            *bi = upr[1];
            bi++;
        }

        for (byte octet : octets) {
            *bi = octet;
            bi++;
        }

        return b;
    }

}

