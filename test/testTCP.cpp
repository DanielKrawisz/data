// Copyright (c) 2013 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "data/net/TCP.hpp"
#include "gtest/gtest.h"

namespace data::net::IP::TCP {
    struct echo_server : server {
        echo_server (asio::io_context &io, uint16 port) :
            server {io, port, [] (ptr<session<const string &>> session) -> handler<string_view> {
                    return [session] (string_view x) {
                        session->send (string {x});
                    };
                }} {}
    };

    TEST (TCPTest, TestTCP) {
        asio::io_context io {};

        echo_server {io, 3456};

        open (io, endpoint {"127.0.0.1:3456"}, [] (asio::error_code) {}, [] () {},
            [] (ptr<session<const string &>> session) -> handler<string_view> {
                ptr<string> test_string = std::make_shared<string> ("thruuup");
                ptr<string> return_string = std::make_shared<string> ();
                session->send (*test_string);
                return [test_string, return_string, session] (string_view x) {
                    *return_string += x;
                    if (return_string->size () >= test_string->size ()) {
                        EXPECT_EQ (test_string, return_string);
                        session->close ();
                    }
                };
            });

        io.run ();

    }

}

