// Copyright (c) 2013 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/bind/bind.hpp>
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

    struct echo_client {
    public:
        echo_client(boost::asio::io_context& io) : timer(io, boost::asio::chrono::seconds(5)), io(io) {
            timer.async_wait (boost::bind (&echo_client::connect,this));
        }
        void connect() {
            std::cout << " Starting to connect" << std::endl;
            open (io, endpoint {"127.0.0.1:3456"}, [] (asio::error_code temp) { }, [] () {},
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
        }
    private:
        boost::asio::steady_timer timer;
        asio::io_context& io;
    };

    TEST (TCPTest, TestTCP) {
        asio::io_context io;
        echo_server {io, 3456};
        std::cout << "Setup server?" << std::endl;
        //echo_client{io};
        std::cout << "Setup client?" << std::endl;
        io.run ();

    }

}

