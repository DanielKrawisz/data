// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STRING
#define DATA_STRING

#include "tools.hpp"

namespace data {

    // the only difference between data::string and std::string is that
    // data::string prints with "" around it.
    struct string : std::string {
        using std::string::string;

        string (const std::string &x) : std::string {x} {}
        string (std::string &&x) : std::string {x} {}

        explicit operator bytes () const;
    };

    string drop (const std::string &, int64 begin);
    string take (const std::string &, int64 end);

    string substring (const std::string &, int64 begin);
    string substring (const std::string &, int64 begin, int64 end);

    string to_upper (const std::string &);

    string to_lower (const std::string &);

    // split a string by a delimiter.
    list<string_view> split (const string_view &s, const string_view &delimiter);

    template <typename X>
    string string_join (list<X> l) {
        std::stringstream ss;
        for (const X &x : l) ss << x;
        return ss.str ();
    }

    std::strong_ordering inline operator <=> (const string &a, const string &b) {
        return static_cast<const std::string &> (a) <=> static_cast<const std::string &> (b);
    }

    std::ostream inline &operator << (std::ostream &o, const string &x) {
        return o << "\"" << static_cast<std::string> (x) << "\"";
    }

    string inline drop (const std::string &x, int64 begin) {
        return substring (x, begin);
    }

    string inline take (const std::string &x, int64 end) {
        return substring (x, 0, end);
    }

    string inline substring (const std::string &x, int64 begin) {
        return substring (x, begin, x.size ());
    }

    string inline to_upper (const std::string &x) {
        string m = x;
        std::transform (m.begin (), m.end (), m.begin (),
            [] (unsigned char c) { return std::toupper (c); });
        return m;
    }

    string inline to_lower (const std::string &x) {
        string m = x;
        std::transform (m.begin (), m.end (), m.begin (),
            [] (unsigned char c) { return std::tolower (c); });
        return m;
    }

}

#endif

