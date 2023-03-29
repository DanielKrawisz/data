//
// Created by nekosune on 05/07/19.
//

#include <data/encoding/ascii.hpp>

namespace data::encoding::ASCII {

    string::operator bytes () const {
        return valid () ? bytes (slice<byte> ((byte*) std::string::data (), std::string::size ())) : bytes ();
    }

    string write (const bytes_view x) {
        string q;
        q.resize (x.size ());
        for (int i = 0; i < x.size (); i++) q[i] = static_cast<char> (x[i]);
        return q;
    }

}

