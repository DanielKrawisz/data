
#ifndef DATA_PARSE_HEX
#define DATA_PARSE_HEX

#include <data/parse/ASCII.hpp>

namespace data::parse::ASCII {

    struct hex              : predicate<either<is_digit, is_alpha>> {};
    struct hex_upper        : predicate<either<is_digit, is_upper>> {};
    struct hex_lower        : predicate<either<is_digit, is_lower>> {};

    struct hex_digit        : sequence<hex, hex> {};
    struct hex_digit_upper  : sequence<hex_upper, hex_upper> {};
    struct hex_digit_lower  : sequence<hex_lower, hex_lower> {};

    struct hex_string       : star<hex_digit> {};
    struct hex_string_upper : star<hex_digit_upper> {};
    struct hex_string_lower : star<hex_digit_lower> {};

    struct hex_number       : sequence<exactly<'0', 'x'>, hex_string> {};
    struct hex_number_upper : sequence<exactly<'0', 'x'>, hex_string_upper> {};
    struct hex_number_lower : sequence<exactly<'0', 'x'>, hex_string_lower> {};

}

#endif
