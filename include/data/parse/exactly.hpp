
#ifndef DATA_PARSE_EXACTLY
#define DATA_PARSE_EXACTLY

#include <data/parse/unicode.hpp>

namespace data::parse {

    template <auto C>
    constexpr uint32 to_code_point () {
        using T = decltype (C);

        static_assert (std::is_integral_v<T>, "exactly<> requires integral literals");

        return static_cast<uint32_t> (C);
    }

    template <auto... Cs>
    using exactly =
        sequence<
            UTF8::predicate<
                UTF8::equal_to<to_code_point<Cs> ()>
            >...
        >;

    template <auto c> struct one : exactly<c> {};

}

#endif
