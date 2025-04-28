// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_ERROR
#define DATA_IO_ERROR

#include <data/maybe.hpp>
#include <data/string.hpp>
#include <argh.h>

namespace data::io {

    struct error {
        int Code;
        maybe<string> Message;
        error () : Code {0}, Message {} {}
        error (int code) : Code {code}, Message {} {}
        error (int code, const string &err): Code {code}, Message {err} {}
        error (const string &err): Code {1}, Message {err} {}
        operator bool () const {return bool (Message) || Code != 0; }
    };

}

#endif
