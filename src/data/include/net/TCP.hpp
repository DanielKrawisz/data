// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_TCP
#define DATA_NET_TCP

#include <data/net/URL.hpp>

namespace data::net::IP {
    
    struct exception : data::exception {
        exception (asio::error_code err) : data::exception {} {
            this->write ("IO error: ", err.message ());
        }
    };
}

namespace data::net::IP::TCP {

    // open a TCP connection.
    void open (asio::io_context &, endpoint, asio::error_handler, close_handler, interaction<string_view, const string &>);

    class server : std::enable_shared_from_this<server> {
        asio::io_context& IO;
        asio::ip::tcp::acceptor Acceptor;
        maybe<asio::ip::tcp::socket> Socket;
        interaction<string_view, const string &> Interact;
        
        void accept ();
        
    public:
        server (asio::io_context &io, uint16 port, interaction<string_view, const string &> interact);
    };
    
    inline server::server (asio::io_context &io, uint16 port, interaction<string_view, const string &> interact):
        IO {io}, Acceptor {io, asio::ip::tcp::endpoint (asio::ip::tcp::v4 (), port)}, Socket {}, Interact {interact} {}
    
}

#endif

