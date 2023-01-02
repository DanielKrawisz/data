// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_JSON
#define DATA_NETWORKING_JSON

#include <data/networking/connection.hpp>
#include <nlohmann/json.hpp>
#include <sstream>

namespace data {
    using JSON = nlohmann::json;
}

namespace data::networking {
    
    struct JSON_line_connection : serialized_connection<char, JSON> {
        std::stringstream Stream;
        
        void write(const char *data, size_t size) final override;
        
        string serialize(const JSON &j) final override;
        
        virtual void parse_error(const string &invalid) override;
    };
    
    string inline JSON_line_connection::serialize(const JSON &j) {
        return j.dump() + "\n";
    }
    
    void inline JSON_line_connection::parse_error(const string &invalid) {
        throw exception{} << "Invalid JSON string: \"" << invalid << "\"";
    }
    
}

#endif