
#ifndef DATA_PARSE_JSON
#define DATA_PARSE_JSON

#include <data/parse/ASCII.hpp>
#include <data/parse/hex.hpp>

namespace data::parse::JSON {

    struct is_whitespace {
        constexpr bool operator () (uint32_t c) const noexcept {
            return
            c == 0x20 ||  // space
            c == 0x09 ||  // \t
            c == 0x0A ||  // \n
            c == 0x0D;    // \r
        }
    };

    template <typename X> using predicate = UTF8::predicate<X>;

    struct ws : star<predicate<is_whitespace>> {};

    struct true_literal  : exactly<'t','r','u','e'> {};
    struct false_literal : exactly<'f','a','l','s','e'> {};
    struct null_literal  : exactly<'n','u','l','l'> {};

    struct natural : ASCII::dec_nat {};

    struct decimal_point : one<'.'> {};
    struct minus_sign : one<'-'> {};
    struct plus_sign : one<'+'> {};
    struct exp : alternatives<one<'E'>, one<'e'>> {};

    struct fractional : sequence<decimal_point, plus<ASCII::digit>> {};

    struct exponent :
        sequence<
            exp,
            optional<
                alternatives<
                    plus_sign,
                    minus_sign>>,
            plus<ASCII::digit>> {};

    struct number :
        sequence<
            optional<minus_sign>,
            natural,
            optional<fractional>,
            optional<exponent>> {};

    struct is_unescaped_char {
        constexpr bool operator () (uint32_t c) const noexcept {
            return
                c >= 0x20 &&          // no control chars
                c != 0x22 &&          // not "
                c != 0x5C;            // not backslash
        }
    };

    struct unescaped_char : predicate<is_unescaped_char> {};

    struct escaped_char :
        sequence<
            one<'\\'>,
            alternatives<
                one<'"'>,
                one<'\\'>,
                one<'/'>,
                one<'b'>,
                one<'f'>,
                one<'n'>,
                one<'r'>,
                one<'t'>>> {};

    struct unicode_escape : sequence<exactly<'\\', 'u'>, rep<4, ASCII::hex>> {};

    struct string_char : alternatives<unescaped_char, escaped_char, unicode_escape> {};

    struct string : sequence<ASCII::quote, star<string_char>, ASCII::quote> {};

    struct value;

    struct comma : sequence<ws, one<','>, ws> {};

    struct osb : sequence<one<'['>, ws> {};
    struct csb : sequence<ws, one<']'>> {};

    struct array :
        sequence<
            osb,
            optional<
                sequence<
                    value,
                    star<
                        sequence<
                            comma,
                            value>>>>,
            csb> {};

    struct colon :
        sequence<
            ws,
            one<':'>,
            ws
        > {};

    struct member :
        sequence<
            string,
            colon,
            value
        > {};

    struct ocb : sequence<one<'{'>, ws> {};
    struct ccb : sequence<ws, one<'}'>> {};

    struct object :
        sequence<
            ocb,
            ws,
            optional<
                sequence<
                    member,
                    star<
                        sequence<
                            comma,
                            member>>>>,
            ccb> {};

    struct value :
        alternatives<
            string,
            number,
            object,
            array,
            true_literal,
            false_literal,
            null_literal
        > {};

    struct document :
        sequence<
            ws,
            value,
            ws
        > {};
}

#endif

