
#ifndef DATA_PARSE_ASCII
#define DATA_PARSE_ASCII

#include <data/parse/UTF8.hpp>
#include <data/parse/exactly.hpp>

namespace data::parse::ASCII {

    template <typename P> using predicate = UTF8::predicate<P>;
    template <typename A, typename B> using either = UTF8::either<A, B>;

    template <uint32_t Min, uint32_t Max>
    using in_range        = UTF8::in_range<Min, Max>;

    using is_digit        = in_range<'0', '9'>;

    using is_lower        = in_range<'a', 'z'>;

    using is_upper        = in_range<'A', 'Z'>;

    using is_alpha        = either<is_lower, is_upper>;

    using is_alnum        = either<is_digit, is_alpha>;

    struct any             : UTF8::ascii {};
    struct printable       : UTF8::printable_ascii {};
    struct digit           : UTF8::digit {};

    template <uint32_t Min, uint32_t Max>
    struct range           : UTF8::range<Min, Max> {};

    struct lower           : range<'a', 'z'> {};

    struct upper           : range<'A', 'Z'> {};

    struct alpha           : predicate<is_alpha> {};

    struct alnum           : predicate<is_alnum> {};

    struct hyphen          : one<'-'> {};

    struct dot             : one<'.'> {};

    struct dec_nat         : alternatives<one<'0'>, sequence<range<'1', '9'>, star<digit>>> {};

    template <auto max> requires std::unsigned_integral<decltype (max)>
    struct max_value_number {

        dec_nat machine;

        // Running value
        decltype (max) value = 0;

        constexpr bool valid () const {
            return machine.valid () && value <= max;
        }

        constexpr bool possible () const {
            return machine.possible () && value < max;
        }

        constexpr void step (std::string_view x, char c) {
            // First, let dec_nat decide if this character is valid as a digit
            machine.step (x, c);
            if (possible ()) value = value * 10 + (c - '0');
        }
    };


}

#endif

