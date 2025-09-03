
// Copyright (c) 2023-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_PERCENT
#define DATA_ENCODING_PERCENT

#include <data/encoding/ascii.hpp>
#include <data/maybe.hpp>

// RFC3986 defines certain reserved characters which
// denote the structure of a URI. Percent encoding
// ensures that arbitrary UTF8 strings can be included
// in a URL without affecting the structure of the URL.
//
// A UTF8 character is percent encoded as "%" followed
// by the hexidecimal value of the encoded character.
//
// Control characters and non-ASCII characters are
// required to be percent encoded. Other characetrs
// may optionally be percent encoded.
namespace data::encoding::percent {

    bool valid (string_view input);

    constexpr const char *Reserved = ":/?#[]@!$&'()*+,;=";
    constexpr const char *Delimiters = ":/?#[]@";
    constexpr const char *Subdelimiters = "!$&'()*+,;=";

    bool inline is_reserved (char c) {
        return strchr (Reserved, c) != nullptr;
    }

    // encode the string, ensuring that the given characters are encoded.
    std::string encode (const data::UTF8 &, const data::ASCII &required = "");

    bool equivalent (const std::string &, const std::string &);

    // a %-encoded string.
    struct string;

    // decode back to UTF8.
    maybe<data::UTF8> decode (string_view);

    struct string : data::ASCII {
        using data::ASCII::ASCII;

        string (const data::ASCII &x) : data::ASCII {x} {}

        // If a character is reserved, then its percent-encoded
        // form is unequal to its non-encoded form. Otherwise
        // they are treated as equal.
        bool operator == (const string &x) const {
            return equivalent (*this, x);
        }

        bool valid () const {
            return percent::valid (*this);
        }
    };

    // https://www.ietf.org/rfc/rfc3986.txt
    // A URI is a universal resource indicator, as opposed to locator.
    // the URI may not enable you to retrieve the document but it will
    // uniquely specify the document.
    struct URI : string {

        static string_view scheme (string_view);
        static string_view authority (string_view);
        static string_view path (string_view);
        static string_view query (string_view);
        static string_view fragment (string_view);

        static string_view user_info (string_view);
        static string_view host (string_view);
        static string_view port (string_view);

        static string_view address (string_view);

        // everything from the path to the end.
        static string_view target (string_view);

        data::ASCII scheme () const;
        maybe<data::UTF8> user_info () const;
        maybe<data::UTF8> host () const;
        maybe<data::ASCII> port () const;
        maybe<data::UTF8> authority () const;
        string target () const;
        string path () const;
        maybe<data::ASCII> query () const; // the part after ? and before #
        maybe<data::UTF8> fragment () const; // the part after the #

        bool valid () const;

        using string::string;
        URI (const string &x) : string {x} {}

        URI (const data::ASCII &protocol, const data::UTF8 &user_utya,
             const data::UTF8 &host, const data::ASCII &port, const string &path,
             const data::UTF8 &query, const data::UTF8 &fragment);

        // convert to a standard equivalent form in which all hex digits are upper case,
        // scheme and host are lower case, and only required digits are hex-encoded.
        // this is used to test equality of URLs.
        URI normalize () const;

    };
    /* funny mnemonic to remember which parts of a URL can be percent encoded.
     *
     *       "Ugly Hippos Push Strange Quests, Possibly Even Frightening Frilly Unicorns!" 🦄
     *
     *       Each word stands for a part of a URL:
     *
     *       Ugly → Userinfo (username:password@)
     *
     *       Hippos → Host (only percent-encoded for IPv6 literals in some contexts — usually not encoded)
     *
     *       Push → Path
     *
     *       Strange → Search (query string)
     *
     *       Quests → Query (alt word for search—helps reinforce)
     *
     *       Possibly → Port (rarely encoded; mostly numeric, but technically valid)
     *
     *       Even → Encoded path segments
     *
     *       Frightening → Fragment
     *
     *       Frilly → (for double “F” mnemonic)
     *
     *       Unicorns → (Unifying idea — "URLs are weird but lovable")
     */

    bool inline operator == (const URI &a, const URI &b) {
        return static_cast<string> (a.normalize ()) == static_cast<string> (b.normalize ());
    }

    std::ostream inline &operator << (std::ostream &o, const URI &u) {
        return o << static_cast<string> (u);
    }

    data::ASCII inline URI::scheme () const {
        string_view x = scheme (*this);
        if (x.data () == nullptr) throw exception {} << "invalid URI " << *this;
        return data::ASCII {x};
    }

    string inline URI::path () const {
        string_view x = path (*this);
        if (x.data () == nullptr) throw exception {} << "invalid URI " << *this;
        return string {x};
    }

    string inline URI::target () const {
        string_view x = target (*this);
        if (x.data () == nullptr) throw exception {} << "invalid URI " << *this;
        return string {x};
    }

    maybe<data::UTF8> inline URI::authority () const {
        string_view x = authority (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

    maybe<data::UTF8> inline URI::user_info () const {
        string_view x = user_info (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

    maybe<data::UTF8> inline URI::host () const {
        string_view x = host (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }

    maybe<data::ASCII> inline URI::port () const {
        string_view x = port (*this);
        if (x.data () == nullptr) return {};
        return data::ASCII {x};
    }

    maybe<data::ASCII> inline URI::query () const {
        string_view x = query (*this);
        if (x.data () == nullptr) return {};
        return {data::ASCII {x}};
    }

    maybe<data::UTF8> inline URI::fragment () const {
        string_view x = fragment (*this);
        if (x.data () == nullptr) return {};
        return decode (x);
    }
}


#endif
