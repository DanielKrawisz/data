// Copyright (c) 2023 - 2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STRING
#define DATA_STRING

#include <data/stack.hpp>
#include <data/bytes.hpp>

namespace data {

    // the only difference between data::string and std::string is that
    // data::string prints with "" around it.
    struct string : std::string {
        using std::string::string;

        string (const std::string &x) : std::string {x} {}
        string (std::string &&x) : std::string {x} {}

        explicit operator bytes () const;

        explicit string (const bytes &);

        template <typename ...X>
        static string write (X &&...);
    };

    string drop (const std::string &, int64 begin);
    string take (const std::string &, int64 end);

    string substring (const std::string &, int64 begin);
    string substring (const std::string &, int64 begin, int64 end);

    string to_upper (const std::string &);

    string to_lower (const std::string &);

    // split a string by a delimiter.
    stack<string_view> split (string_view s, string_view delimiter);

    template <Sequence list>
    string string_join (list, string_view delimiter = {});

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

    inline string::string (const bytes &x) : data::string {} {
        this->resize (x.size ());
        for (int i = 0; i < x.size (); i++) (*this)[i] = static_cast<char> (x[i]);
    }

    namespace {
        template <typename ...X> struct write_string;

        template <> struct write_string<> {
            void operator () (std::ostream &) {}
        };

        template <typename X> struct write_string<X> {
            void operator () (std::ostream &ss, X &&x) {
                ss << x;
            }
        };

        template <typename X, typename ...Y> struct write_string<X, Y...> {
            void operator () (std::ostream &ss, X &&x, Y &&...y) {
                write_string<Y...> {} (ss << x, std::forward<Y> (y)...);
            }
        };
    }

    template <typename ...X>
    string string::write (X &&...x) {
        std::stringstream ss;
        write_string<X...> {} (ss, std::forward<X> (x)...);
        return ss.str ();
    }

    template <Sequence list>
    string string_join (list s, string_view delimiter) {
        if (data::empty (s)) return "";
        std::stringstream ss;
        ss << static_cast<std::string> (first (s));
        for (const string &x : rest (s)) ss << delimiter << static_cast<std::string> (x);
        return ss.str ();
    }

}

#endif
