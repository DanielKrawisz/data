
#ifndef DATA_PARSE_IP
#define DATA_PARSE_IP

#include <data/parse/hex.hpp>

namespace data::parse::IP {

    using namespace ASCII;

    struct port : max_value_number<uint32 (65535)> {};

    struct V4_number : max_value_number<uint16 (255)> {};

    struct V4 : sequence<V4_number, rep<3, sequence<dot, V4_number>>> {};

    struct hextet : repeated<hex, 1, 4> {};

    struct ls32 : alternatives<sequence<hextet, one<':'>, hextet>, V4> {};
/*
    struct ipv6 : alternatives<
        sequence<rep<6, sequence<hextet, one<':'>>>, ls32>,
        sequence<exactly<':', ':'>, rep<5, sequence<hextet, one<':'>>>, ls32>,
        sequence<optional<hextet>, exactly<':', ':'>, rep<4, sequence<hextet, one<':'>>>, ls32>,
        sequence<optional<sequence<hextet, optional<sequence<one<':'>, hextet>>>>, exactly<':', ':'>, rep<3, sequence<hextet, one<':'>>>, ls32>,
        sequence<optional<sequence<hextet, rep<2, optional<sequence<one<':'>, hextet>>>>>, exactly<':', ':'>, rep<2, sequence<hextet, one<':'>>>, ls32>,
        sequence<optional<sequence<hextet, rep<3, optional<sequence<one<':'>, hextet>>>>>, exactly<':', ':'>, sequence<hextet, one<':'>>, ls32>,
        sequence<optional<sequence<hextet, rep<4, optional<sequence<one<':'>, hextet>>>>>, exactly<':', ':'>, ls32>,
        sequence<optional<sequence<hextet, rep<5, optional<sequence<one<':'>, hextet>>>>>, exactly<':', ':'>, hextet>,
        sequence<optional<sequence<hextet, rep<6, optional<sequence<one<':'>, hextet>>>>>, exactly<':', ':'>>> {};*/

    struct V6 {    // Number of completed 16-bit groups (i.e., how many ':' separators we've finalized)
        int groups = 0;

        // Number of hex digits in the current group (0–4)
        int group_digits = 0;

        // Whether we've seen a "::" compression
        bool saw_double_colon = false;

        // Whether the last character processed was a single ':'
        // Used to detect "::"
        bool last_colon = false;

        // Permanent failure flag
        bool dead = false;

        // Whether we switched to IPv4 tail parsing
        bool in_ipv4 = false;

        // where the ipv4 starts if we have one.
        size_t ipv4_start = 0;

        // The IPv4 sub-machine
        V4 ipv4;

        constexpr bool possible () const {
            return !dead;
        }

        constexpr bool valid () const {
            if (dead) return false;
            if (in_ipv4) return ipv4.valid ();
            if (last_colon && !saw_double_colon) return false;
            return (saw_double_colon && groups < 7) || groups == 7;
        }

        constexpr void step (std::string_view prefix, char c) {
            if (in_ipv4) {
                ipv4.step (prefix.substr (ipv4_start, prefix.size () - ipv4_start), c);
                if (!ipv4.possible () && !ipv4.valid ()) dead = true;
                return;
            }

            auto is_hex = [] (char x) {
                return std::isxdigit (static_cast<unsigned char> (x));
            };

            if (is_hex (c)) {
                if (group_digits == 4) {
                    dead = true;
                    return;
                }

                group_digits++;

                // can't begin with :
                if (prefix.size () == 1 && last_colon) dead = true;

                last_colon = false;
                return;
            }

            if (c == ':') {
                if (last_colon) {
                    if (saw_double_colon) {
                        dead = true;
                        return;
                    }

                    saw_double_colon = true;
                    last_colon = false;
                    return;
                }

                if (group_digits == 0 && prefix.size () > 0) {
                    dead = true;
                    return;
                }

                groups++;
                group_digits = 0;
                last_colon = true;

                if (groups > 7 || saw_double_colon && groups > 6) {
                    dead = true;
                }

                return;
            }

            if (c == '.') {
                if (group_digits == 0) {
                    dead = true;
                    return;
                }

                if (!saw_double_colon) {
                    if (groups != 6) dead = true;
                } else {
                    if (groups > 6) dead = true;
                }

                // switch to IPv4 mode
                in_ipv4 = true;

                // We have already read some hex digits,
                // but IPv4 expects decimal digits.
                // So those digits must all be decimal.
                // start of the current group
                ipv4_start = prefix.size () - group_digits;

                for (size_t i = ipv4_start; i < prefix.size (); ++i) {
                    ipv4.step (prefix.substr (ipv4_start, i - ipv4_start), prefix[i]);
                    if (!ipv4.possible ()) {
                        dead = true;
                        return;
                    }
                }

                // now feed '.'
                ipv4.step (prefix.substr (ipv4_start, prefix.size () - ipv4_start), c);
                return;
            }

            dead = true;
        }
    };

    struct address : alternatives<V6, V4> {};

    using endpoint = parse::sequence<address, exactly<':'>, port>;

}

#endif
