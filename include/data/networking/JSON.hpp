// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_JSON
#define DATA_NETWORKING_JSON

#include <data/networking/session.hpp>
#include <nlohmann/json.hpp>
#include <sstream>

namespace data {
    using JSON = nlohmann::json;
}

namespace data::networking {
    
    struct JSON_line_session : serialized_session<JSON> {
        std::stringstream Stream;
        
        void write(const byte *data, size_t size) final override;
        
        bytes serialize(const JSON &j) final override {
            return bytes::from_string(j.dump() + "\n");
        }
        
        virtual void parse_error(const string &invalid) override {
            throw exception{} << "Invalid JSON string: \"" << invalid << "\"";
        }
    };
    
}

#endif
