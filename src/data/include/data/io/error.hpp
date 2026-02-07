// Copyright (c) 2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_ERROR
#define DATA_IO_ERROR

#include <iostream>
#include <data/maybe.hpp>
#include <data/string.hpp>

namespace data::io {

    struct error {
        enum code : int {
            ok = 0,

            // Default value when no code is provided.
            generic = 1,

            // Transient failure: network, lock, timeout.
            // Automation may retry; no human yet.
            try_again = 2,

            // Bad input, misuse, unmet preconditions.
            // User can fix it; no retry; no escalation.
            user_action = 3,

            // Environmental or configuration issue.
            // A human operator must intervene.
            operator_action = 4,

            // Invariant violation, bug, corrupted state.
            // Escalate to developers.
            programmer_action = 5,

            unknown = 6
        };

        code Code;
        maybe<string> Message;
        error () : Code {0}, Message {} {}
        error (code code) : Code {code}, Message {} {}
        error (code code, const string &err): Code {code}, Message {err} {}
        error (const string &err): Code {5}, Message {err} {}
        operator bool () const {return bool (Message) || Code != ok; }
    };

    std::ostream inline &operator << (std::ostream &o, const error &e) {
        if (e.Code == error::ok) return o << "success";
        o << "error ";
        if (e.Message) o << "message: " << *e.Message << "; ";
        o << "code " << static_cast<int> (e.Code) << ": ";
        switch (e.Code) {
            case 0: return o << "not an error";
            case 2: return o << "try again";
            case 3: return o << "user should resolve his invalid action";
            case 4: return o << "summon administrator to resolve this issue";
            case 5: return o << "call the programmer to fix this bug";
            default: return o;
        }
    }

}

#endif
