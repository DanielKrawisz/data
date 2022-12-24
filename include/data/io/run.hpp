// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/networking/session.hpp>
#include <boost/asio.hpp>

namespace data::io {
    
    // run an external command with standard in and standard out connected. 
    ptr<session<string_view>> run(io::io_context &, string command, std::function<void(string_view)>);
    
}

#endif

