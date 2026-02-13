
#ifndef DATA_PARSE_HEX
#define DATA_PARSE_HEX

#include <data/parse/ASCII.hpp>

namespace data::parse::ASCII {

    using hex              = predicate<either<digit, alpha>>;
    using hex_upper        = predicate<either<digit, upper>>;
    using hex_lower        = predicate<either<digit, lower>>;

    using hex_digit        = sequence<hex, hex>;
    using hex_digit_upper  = sequence<hex_upper, hex_upper>;
    using hex_digit_lower  = sequence<hex_lower, hex_lower>;

    using hex_string       = star<hex_digit>;
    using hex_string_upper = star<hex_digit_upper>;
    using hex_string_lower = star<hex_digit_lower>;

    using hex_number       = sequence<exactly<'0', 'x'>, hex_string>;
    using hex_number_upper = sequence<exactly<'0', 'x'>, hex_string_upper>;
    using hex_number_lower = sequence<exactly<'0', 'x'>, hex_string_lower>;

}

#endif
