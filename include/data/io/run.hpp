// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/types.hpp>

namespace data::io {

    struct process {
        void send (const string &);

        int exit();
        void handle_error (const io_error &err);
    };
    
    // run an external command with standard in and standard out connected. 
    ptr<session> run(boost::asio::io_context &, string command, std::function<bool (string_view)> out, std::function<bool (string_view)> err);
    
}

#endif

