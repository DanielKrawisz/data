// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NET_TCP
#define DATA_NET_TCP

#include <data/net/session.hpp>
#include <data/net/asio/socket.hpp>
#include <ctre.hpp>
#include <data/io/unimplemented.hpp>

namespace data::net::IP {
    
    struct exception : data::exception {
        exception (asio::error_code err) : data::exception {} {
            this->write ("IO error: ", err.message ());
        }
    };
    
    struct address : string {
        /* Note: the program crashes when we try to use these. 
        static constexpr auto address_v4_pattern = ctll::fixed_string{"(((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4})"};
        static constexpr auto address_v6_pattern = ctll::fixed_string{"((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?"};
        */
        
        address () : string {} {}
        address (const string &x) : string {x} {}
        operator asio::ip::address () const;
        bool valid () const;
        uint32 version () const;
    };
}

namespace data::net::IP::TCP {
    
    // A tcp endpoint is an ip address and port.
    struct endpoint : string {
        static constexpr auto pattern = 
            ctll::fixed_string{
                // ip v4. 
                "((?<V4>([0-9\\.]*))|"
                // ip v6 with braces around it. 
                "(\\[(?<V6>([0-9a-fA-F:\\.]*))\\])):"
                // port number, which can be 0 to 65535. 
                "(?<port>([0-9]{1,5}))"};
        
        endpoint () : string {} {}
        endpoint (const string &x) : string{x} {}
        endpoint (const IP::address &addr, uint16);
        bool valid () const;
        operator asio::ip::tcp::endpoint () const;
        IP::address address () const;
        uint16 port () const;
    };

    // open a TCP connection.
    void open (asio::io_context &, endpoint, asio::error_handler, close_handler, interaction<string_view, const string &>);

    class server : std::enable_shared_from_this<server> {
        asio::io_context& IO;
        asio::ip::tcp::acceptor Acceptor;
        maybe<asio::ip::tcp::socket> Socket;
        interaction<string_view, const string &> Interact;
        
        void accept ();
        
    public:
        server (asio::io_context &io, uint16 port);
    };
    
    inline server::server (asio::io_context &io, uint16 port):
        IO (io), Acceptor (io, asio::ip::tcp::endpoint (asio::ip::tcp::v4 (), port)) {}
    
}

#endif

