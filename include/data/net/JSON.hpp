// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_JSON
#define DATA_NETWORKING_JSON

#include <data/net/serialized.hpp>
#include <nlohmann/json.hpp>

namespace data {
    using JSON = nlohmann::json;

    ptr<writer<char>> JSON_line_parser (function<void (const JSON &)>, function<void (parse_error)>);
}

namespace data::net {

    struct open_JSON_line_session {
        function<void (parse_error)> ErrorHandler;
        open<session<const string &>, string_view> Open;

        ptr<session<JSON>> operator () (receive_handler<session<JSON>, JSON> receiver) {
            return open_serialized_session<char, JSON> {&JSON_line_parser, ErrorHandler, Open, [] (const JSON &j) -> string {
                return j.dump () + "\n";
            }} (receiver);
        }
    };
    
}

#endif
