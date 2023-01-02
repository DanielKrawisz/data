// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/networking/session.hpp>
#include <boost/asio.hpp>

namespace data::io {
    namespace io = boost::asio;

    struct session : networking::session<string_view> {
        virtual ~session() {}

        virtual int close() = 0;
        virtual void error (string_view) = 0;
    };
    
    // run an external command with standard in and standard out connected. 
    ptr<session> run(io::io_context &, string command, std::function<bool (string_view)> out, std::function<bool (string_view)> err);
    
}

#endif

