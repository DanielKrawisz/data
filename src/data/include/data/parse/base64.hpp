

#ifndef DATA_PARSE_BASE64
#define DATA_PARSE_BASE64

#include <data/parse/ASCII.hpp>

namespace data::parse::ASCII {

    struct is_base64 {
        bool operator() (uint32_t c) const {
            return is_alnum {} (c) ||
            c == '+' ||
            c == '/';
        }
    };

    using base64 = predicate<is_base64>;

    using base64_string = sequence<star<sequence<base64, base64, base64>>,
        alternatives<sequence<>,
            sequence<base64, base64, exactly<'=', '='>>,
            sequence<base64, base64, base64, one<'='>>>>;

}

#endif

