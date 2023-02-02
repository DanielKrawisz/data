// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_JSON
#define DATA_NET_JSON

#include <data/net/serialized.hpp>
#include <nlohmann/json.hpp>

namespace data {
    using JSON = nlohmann::json;

    ptr<writer<char>> JSON_line_parser (handler<const JSON &>, handler<parse_error>);
}

namespace data::net {

    void open_JSON_line_session (
        handler<parse_error> error_handler,
        open<session<const string &>, string_view> open,
        interaction<const JSON &> receiver) {
            open_serialized_session<char, const JSON &> (open, [] (const JSON &j) -> string {
                return j.dump () + "\n";
            }, &JSON_line_parser, error_handler, receiver);
    }
    
}

#endif
