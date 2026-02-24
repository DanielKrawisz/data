
#ifndef DATA_PARSE_URL
#define DATA_PARSE_URL

#include <data/parse/hex.hpp>
#include <data/parse/IP.hpp>

namespace data::parse::URL {

    using namespace ASCII;

    struct is_unreserved {
        bool operator () (uint32 c) const {
            return is_alnum {} (c) ||
                c == '-' ||
                c == '.' ||
                c == '_' ||
                c == '~';
        }
    };

    struct unreserved : predicate<is_unreserved> {};

    struct is_gen_delim {
        bool operator () (uint32 c) const {
            switch (c) {
                case ':': case '/': case '?': case '#':
                case '[': case ']': case '@':
                    return true;
                default:
                    return false;
            }
        }
    };

    struct is_sub_delim {
        bool operator () (uint32 c) const {
            switch (c) {
                case '!': case '$': case '&': case '\'':
                case '(': case ')': case '*': case '+':
                case ',': case ';': case '=':
                    return true;
                default:
                    return false;
            }
        }
    };

    using is_reserved = either<is_gen_delim, is_sub_delim>;

    struct gen_delim : predicate<is_gen_delim> {};
    struct sub_delim : predicate<is_sub_delim> {};
    struct reserved : predicate<is_reserved> {};

    struct pct :
        sequence<
            one<'%'>,
            hex_digit,
            hex_digit> {};

    struct pchar :
        alternatives<
            unreserved,
            sub_delim,
            pct,
            one<':'>,
            one<'@'>> {};

    struct segment    : star<pchar> {};
    struct segment_nz : plus<pchar> {};

    struct segment_nz_nc : plus<alternatives<unreserved, pct, sub_delim, one<'@'>>> {};

    struct path_abempty : star<sequence<one<'/'>, segment>> {};

    struct path_rootless : sequence<segment_nz, path_abempty> {};

    struct path_absolute : sequence<one<'/'>, optional<sequence<segment_nz, star<sequence<one<'/'>, segment>>>>> {};

    struct path_after_authority : path_abempty {};

    struct reg_name : star<alternatives<unreserved, sub_delim, pct>> {};

    struct scheme : sequence<alpha, star<alternatives<alnum, one<'+'>, one<'-'>, one<'.'>>>> {};

    struct query_char : alternatives<pchar, one<'/'>, one<'?'>> {};

    struct query : star<query_char> {};

    struct fragment : star<query_char> {};

    struct domain_label : max_size<sequence<plus<alnum>, star<sequence<hyphen, plus<alnum>>>>, 63> {};

    struct domain_name : max_size<sequence<domain_label, repeated<sequence<dot, domain_label>>>, 253> {};

    struct userinfo : star<alternatives<
        unreserved,
        pct,
        sub_delim,
        one<':'>>> {};

    struct ip_future : sequence<one<'v'>, plus<hex>, one<'.'>, plus<alternatives<unreserved, sub_delim, one<':'>>>> {};

    struct ip_literal : sequence<one<'['>, alternatives<IP::V6, ip_future>, one<']'>> {};

    struct host : alternatives<ip_literal, IP::V4, reg_name> {};

    struct authority : sequence</*optional<sequence<userinfo, one<'@'>>>, */host, optional<sequence<one<':'>, IP::port>>> {};

    struct path : alternatives<path_after_authority, path_absolute, path_rootless> {};

    struct hierarchical : alternatives<sequence<exactly<'/', '/'>, authority, path_after_authority>, path_absolute, path_rootless> {};

    struct target : sequence<alternatives<path_after_authority, path_absolute, path_rootless>,
        optional<sequence<one<'?'>, query>>,
        optional<sequence<one<'#'>, fragment>>> {};

    struct URI : sequence<scheme, one<':'>, hierarchical, optional<sequence<one<'?'>, query>>, optional<sequence<one<'#'>, fragment>>> {};
}

namespace data::parse::TCP {

    struct endpoint : sequence<alternatives<URL::ip_literal, IP::V4>, exactly<':'>, IP::port> {};

}

#endif
