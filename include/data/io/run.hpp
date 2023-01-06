// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/networking/asio/session.hpp>

namespace data::io {

    struct process {
        void send (const string &);

        int exit ();
        bool closed ();
        void handle_error (const io_error &err);
    };
    
    // run an external command with standard in and standard out connected. 
    ptr<session> run (boost::asio::io_context &, string command,
        networking::receive_handler<process, string_view> std_out_handler,
        networking::receive_handler<process, string_view> std_err_handler,
        std::function<bool (io_error)> err_handler);
    
}

#endif

