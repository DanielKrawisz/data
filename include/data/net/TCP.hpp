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
    /*
    struct address : string {
        
        address () : string {} {}
        address (const string &x) : string {x} {}
        operator asio::ip::address () const;
        bool valid () const;
        uint32 version () const;
    };*/
}

namespace data::net::IP::TCP {
    /*
    // A tcp endpoint is an ip address and port.
    struct endpoint : string {
        static constexpr auto pattern = 
            ctll::fixed_string {
                // ip v4. 
                "((?<V4>([0-9\\.]*))|"
                // ip v6 with braces around it. 
                "(\\[(?<V6>([0-9a-fA-F:\\.]*))\\])):"
                // port number, which can be 0 to 65535. 
                "(?<port>([0-9]{1,5}))"};
        
        endpoint () : string {} {}
        endpoint (const string &x) : string {x} {}
        endpoint (const IP::address &addr, uint16);
        bool valid () const;
        operator asio::ip::tcp::endpoint () const;
        IP::address address () const;
        uint16 port () const;
    };*/

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

