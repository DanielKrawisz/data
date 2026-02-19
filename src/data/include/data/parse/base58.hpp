
#ifndef DATA_PARSE_BASE58
#define DATA_PARSE_BASE58

#include <data/parse/ASCII.hpp>
#include <data/parse/exactly.hpp>

namespace data::parse::ASCII {

    struct is_base58 {
        bool operator () (uint32 c) const {
            if (c >= '1' && c <= '9') return true;
            if (c >= 'A' && c <= 'H') return true;
            if (c >= 'J' && c <= 'N') return true;
            if (c >= 'P' && c <= 'Z') return true;
            if (c >= 'a' && c <= 'k') return true;
            if (c >= 'm' && c <= 'z') return true;
            return false;
        }
    };

    using base58 = char_predicate<is_base58>;

    using base58_string = star<base58>;

    using base58_number = machine::sequence<exactly<'0', 'o'>, base58_string>;

}

#endif
