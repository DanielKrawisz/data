
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

    using is_alpha        = either<in_range<'a', 'z'>, in_range<'A', 'Z'>>;

    using is_alnum        = either<is_digit, is_alpha>;

    using any             = UTF8::ascii;
    using printable       = UTF8::printable_ascii;
    using digit           = UTF8::digit;

    template <uint32_t Min, uint32_t Max>
    using range           = UTF8::range<Min, Max>;

    using lower           = range<'a', 'z'>;

    using upper           = range<'A', 'Z'>;

    using alpha           = predicate<is_alpha>;

    using alnum           = predicate<is_alnum>;

    using hyphen          = one<'-'>;

    using dot             = one<'.'>;

}

#endif

