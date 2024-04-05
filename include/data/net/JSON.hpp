// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NET_JSON
#define DATA_NET_JSON

#include <data/net/serialized.hpp>
#include <nlohmann/json.hpp>

namespace data {
    using JSON = nlohmann::json;

    ptr<writer<char>> JSON_line_parser (handler<const JSON::exception &>, handler<const JSON &>);
}

namespace data::net {

    void inline open_JSON_session (
        handler<const JSON::exception &> error_handler,
        open<string_view, const std::string &> o,
        close_handler on_close,
        interaction<const JSON &> receiver) {
            serialized_session_read<const JSON &, const JSON &, char> (o, [] (const JSON &j) -> std::string {
                return j.dump ();
            }, [error_handler] (string_view x) -> JSON {
                try {
                    auto j = JSON::parse (x);
                    return j;
                } catch (const JSON::exception &x) {
                    error_handler (x);
                }
            }) (on_close, receiver);
    }

    // JSON line session means that we insert a new line at the end of every JSON type.
    void inline open_JSON_line_session (
        handler<const JSON::exception &> error_handler,
        open<string_view, const std::string &> o,
        close_handler on_close,
        interaction<const JSON &> receiver) {
            serialized_session_parsed<const JSON &, const JSON &, char> (o, [] (const JSON &j) -> std::string {
                return j.dump () + "\n";
            }, [error_handler] (handler<const JSON &> handler) -> ptr<writer<char>> {
                return JSON_line_parser (error_handler, handler);
            }) (on_close, receiver);
    }
    
}

#endif
