
#ifndef DATA_PARSE_URL
#define DATA_PARSE_URL

#include <data/parse/hex.hpp>

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

    using unreserved = predicate<is_unreserved>;

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

    using gen_delims = predicate<is_gen_delim>;
    using sub_delims = predicate<is_sub_delim>;
    using reserved = predicate<is_reserved>;

    using pct_encoded =
        sequence<
            one<'%'>,
            hex_digit,
            hex_digit>;

    using pchar =
        alternatives<
            unreserved,
            pct_encoded,
            sub_delims,
            one<':'>,
            one<'@'>>;

    using segment    = star<pchar>;
    using segment_nz = plus<pchar>;

    using path_abempty = star<sequence<one<'/'>, segment>>;

    using path_absolute = sequence<one<'/'>, optional<sequence<segment_nz, star<sequence<one<'/'>, segment>>>>>;

    using query_char = alternatives<pchar, one<'/'>, one<'?'>>;

    using query = star<query_char>;

    using domain_label = max_size<sequence<plus<alnum>, star<sequence<hyphen, plus<alnum>>>>, 63>;

    using domain_name = max_size<sequence<domain_label, repeated<sequence<dot, domain_label>>>, 253>;

    using fragment = star<query_char>;

    using userinfo =
        star<
            alternatives<
                unreserved,
                pct_encoded,
                sub_delims,
                one<':'>>>;

    struct port {
        std::uint32_t value = 0;
        std::size_t digits = 0;
        bool dead = false;

        bool possible () const {
            return !dead;
        }

        bool valid () const {
            return !dead && digits > 0 && value <= 65535;
        }

        port step (std::string_view prefix, char c) const {
            port next = *this;

            if (!std::isdigit (static_cast<unsigned char> (c))) {
                next.dead = true;
                return next;
            }

            if (digits == 5) {
                next.dead = true;
                return next;
            }

            next.value = value * 10 + (c - '0');
            next.digits++;

            if (next.value > 65535) {
                next.dead = true;
            }

            return next;
        }
    };
}

#endif
