// Copyright (c) 2022 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_IO_RUN
#define DATA_IO_RUN

#include <data/networking/asio/async_to.hpp>
#include <data/networking/asio/async_from.hpp>
#include <boost/process.hpp>

namespace data::io {

    struct session : networking::asio::session<string_view, const string &>,
        networking::asio::async_to<boost::process::async_pipe, char>,
        networking::asio::async_from<boost::process::async_pipe, char> {
        virtual ~session() {}

        void close() final override;
        void handle_error (const io_error &err) final override;
    };
    
    // run an external command with standard in and standard out connected. 
    ptr<session> run(boost::asio::io_context &, string command, std::function<bool (string_view)> out, std::function<bool (string_view)> err);
    
}

#endif

