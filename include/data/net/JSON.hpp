// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_JSON
#define DATA_NET_JSON

#include <data/net/serialized.hpp>
#include <nlohmann/json.hpp>

namespace data {
    using JSON = nlohmann::json;

    ptr<writer<char>> JSON_line_parser (handler<parse_error>, handler<const JSON &>);
}

namespace data::net {

    void open_JSON_line_session (
        handler<parse_error> error_handler,
        open<string_view, const string &> o,
        interaction<const JSON &> receiver,
        close_handler on_close) {
            serialized_session<const JSON &, const JSON &, char> (o, [] (const JSON &j) -> string {
                return j.dump () + "\n";
            }, curry (parser<char, const JSON &> (JSON_line_parser), error_handler)) (on_close, receiver);
    }

    void open_JSON_session (
        handler<parse_error> error_handler,
        open<string_view, const string &> o,
        interaction<const JSON &> receiver,
        close_handler on_close) {
            serialized_session<const JSON &, const JSON &, char> (o, [] (const JSON &j) -> string {
                return j.dump ();
            }, curry (parser<char, const JSON &> (JSON_line_parser), error_handler)) (on_close, receiver);
    }
    
}

#endif
