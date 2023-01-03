// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/networking/asio/message_queue.hpp>
#include <boost/process.hpp>

namespace data::io {

    struct session : networking::asio::message_queue<session, boost::process::async_pipe, string_view, const string &> {
        virtual ~session() {}

        virtual int close() = 0;
        virtual void error (string_view) = 0;
    };
    
    // run an external command with standard in and standard out connected. 
    ptr<session> run(io::io_context &, string command, std::function<bool (string_view)> out, std::function<bool (string_view)> err);
    
}

#endif

