// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_ERROR
#define DATA_NET_ERROR

#include <data/net/JSON.hpp>
#include <data/net/URL.hpp>
#include <data/net/HTTP.hpp>

// https://www.rfc-editor.org/rfc/rfc7159
// https://www.rfc-editor.org/rfc/rfc7807

namespace data::net {

    struct error : JSON {
        bool valid () const;
        URL type () const;
        maybe<string> title () const;
        maybe<net::HTTP::status> status () const;
        maybe<string> detail () const;
        maybe<URL> instance () const;

        error (JSON &&j) : JSON {j} {}
        error (const JSON &j) : JSON {j} {}

        // none of these fields are actually required.
        static bool valid (const JSON &j) {
            return j.is_object ();
        }

        static URL type (const JSON &);
        static maybe<string> title (const JSON &);
        static maybe<net::HTTP::status> status (const JSON &);
        static maybe<string> detail (const JSON &);
        static maybe<URL> instance (const JSON &);
    };

    bool inline error::valid () const {
        return valid (*this);
    }

    URL inline error::type () const {
        return type (*this);
    }

    maybe<string> inline error::title () const {
        return title (*this);
    }

    maybe<net::HTTP::status> inline error::status () const {
        return status (*this);
    }

    maybe<string> inline error::detail () const {
        return detail (*this);
    }

    maybe<URL> inline error::instance () const {
        return instance (*this);
    }

}

#endif
