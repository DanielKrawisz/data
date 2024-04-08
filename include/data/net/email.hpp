
// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_EMAIL
#define DATA_NET_EMAIL

#include <data/encoding/ascii.hpp>

namespace data::net::email {

    struct address;

    std::ostream &operator << (std::ostream &, const address &);

    bool operator == (const address &, const address &);

    struct address : ASCII {
        using ASCII::ASCII;
        address (const ASCII &x) : ASCII {x} {}

        static bool valid (string_view);
        bool valid () const;

        static string_view local_part (string_view);
        static string_view domain (string_view);
    };
}


#endif
