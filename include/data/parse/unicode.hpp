
#ifndef DATA_PARSE_UTF8
#define DATA_PARSE_UTF8

#include <data/parse/control.hpp>

namespace data::parse::UTF8 {

    template <typename Predicate>
    struct predicate {
        uint32_t codepoint = 0;
        uint8_t expected = 0;
        uint8_t seen = 0;
        bool consumed = false;
        bool ok = true;

        constexpr bool possible () const {
            return ok && !consumed;
        }

        constexpr bool valid () const {
            return ok && consumed;
        }

        constexpr void step (string_view, char c) {
            unsigned char uc = static_cast<unsigned char>(c);

            if (!ok || consumed) {
                ok = false;
                return;
            }

            // First byte
            if (seen == 0) {
                if ((uc & 0x80) == 0x00) {
                    // 1-byte ASCII
                    codepoint = uc;
                    consumed = Predicate {} (codepoint);
                    ok = consumed;
                    return;
                } else if ((uc & 0xE0) == 0xC0) {
                    codepoint = uc & 0x1F;
                    expected = 2;
                } else if ((uc & 0xF0) == 0xE0) {
                    codepoint = uc & 0x0F;
                    expected = 3;
                } else if ((uc & 0xF8) == 0xF0) {
                    codepoint = uc & 0x07;
                    expected = 4;
                } else {
                    ok = false;
                    return;
                }

                seen = 1;
                return;
            }

            // Continuation byte
            if ((uc & 0xC0) != 0x80) {
                ok = false;
                return;
            }

            codepoint = (codepoint << 6) | (uc & 0x3F);
            ++seen;

            if (seen == expected) {
                consumed = Predicate {} (codepoint);
                ok = consumed;
            }
        }
    };

    struct valid_utf8 {
        bool operator () (uint32_t cp) const {
            return cp <= 0x10FFFF &&
            !(cp >= 0xD800 && cp <= 0xDFFF);
        }
    };

    struct printable_utf8 {
        bool operator () (uint32_t cp) const {
            if (!valid_utf8 {} (cp))
                return false;

                // C0 controls
                if (cp <= 0x1F)
                    return false;

            // DEL + C1 controls
            if (cp >= 0x7F && cp <= 0x9F)
                return false;

            return true;
        }
    };

    template <uint32_t Min, uint32_t Max>
    struct in_range {
        bool operator () (uint32_t cp) const {
            return cp >= Min && cp <= Max;
        }
    };

    template <uint32_t Val>
    struct equal_to {
        bool operator () (uint32_t cp) const {
            return cp == Val;
        }
    };

    template <typename A, typename B>
    struct either {
        bool operator () (uint32_t cp) const {
            return A {} (cp) || B {} (cp);
        }
    };

    struct is_whitespace {
        constexpr bool operator () (uint32_t c) const noexcept {
            return
                // ASCII whitespace
                (c >= 0x0009 && c <= 0x000D) ||
                c == 0x0020 ||

                // Next line
                c == 0x0085 ||

                // Non-breaking space
                c == 0x00A0 ||

                // Ogham space mark
                c == 0x1680 ||

                // En quad .. hair space
                (c >= 0x2000 && c <= 0x200A) ||

                // Line separator
                c == 0x2028 ||

                // Paragraph separator
                c == 0x2029 ||

                // Narrow no-break space
                c == 0x202F ||

                // Medium mathematical space
                c == 0x205F ||

                // Ideographic space
                c == 0x3000;
        }
    };

    struct any             : predicate<valid_utf8> {};

    struct printable       : predicate<printable_utf8> {};

    template <uint32_t Min, uint32_t Max>
    struct range           : predicate<in_range<Min, Max>> {};

    struct ascii           : range<0x00, 0x7F> {};

    struct printable_ascii : range<0x20, 0x7E> {};

    struct digit           : range<'0', '9'> {};

    struct alpha           : predicate<either<in_range<'A', 'Z'>, in_range<'a', 'z'>>> {};

    struct whitespace      : predicate<is_whitespace> {};


}

#endif
