// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_NETWORKING_TCP
#define DATA_NETWORKING_TCP

#include <data/net/session.hpp>
#include <data/net/asio/async_stream_session.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <ctre.hpp>
#include <data/io/unimplemented.hpp>

namespace data::net::IP {
    namespace io = boost::asio;
    using io_error = boost::system::error_code;
    
    struct exception : data::exception {
        exception (io_error err) : data::exception {} {
            this->write ("IO error: ", err.message ());
        }
    };
    
    struct address : string {
        /* Note: the program crashes when we try to use these. 
        static constexpr auto address_v4_pattern = ctll::fixed_string{"(((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)\\.?\\b){4})"};
        static constexpr auto address_v6_pattern = ctll::fixed_string{"((([0-9A-Fa-f]{1,4}:){7}([0-9A-Fa-f]{1,4}|:))|(([0-9A-Fa-f]{1,4}:){6}(:[0-9A-Fa-f]{1,4}|((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){5}(((:[0-9A-Fa-f]{1,4}){1,2})|:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3})|:))|(([0-9A-Fa-f]{1,4}:){4}(((:[0-9A-Fa-f]{1,4}){1,3})|((:[0-9A-Fa-f]{1,4})?:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){3}(((:[0-9A-Fa-f]{1,4}){1,4})|((:[0-9A-Fa-f]{1,4}){0,2}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){2}(((:[0-9A-Fa-f]{1,4}){1,5})|((:[0-9A-Fa-f]{1,4}){0,3}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(([0-9A-Fa-f]{1,4}:){1}(((:[0-9A-Fa-f]{1,4}){1,6})|((:[0-9A-Fa-f]{1,4}){0,4}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:))|(:(((:[0-9A-Fa-f]{1,4}){1,7})|((:[0-9A-Fa-f]{1,4}){0,5}:((25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}))|:)))(%.+)?"};*/
        
        address () : string {} {}
        address (const string &x) : string {x} {}
        operator io::ip::address () const;
        bool valid () const;
        uint32 version () const;
    };
}

namespace data::net::IP::TCP {
    using socket = io::ip::tcp::socket;
    
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
        operator io::ip::tcp::endpoint () const;
        IP::address address () const;
        uint16 port () const;
    };
    
    // https://dens.website/tutorials/cpp-asio
    
    // we need to use enable_shared_from_this because of the possibility that 
    // tcp_stream will go out of scope and be deleted before one of the 
    // handlers is called from async_read_until or async_write. 
    struct session final : public asio::async_stream_session<session, socket, char> {
        using asio::async_stream_session<session, socket, char>::async_stream_session;

        void close () final override {
            throw method::unimplemented {"TCP::session::close"};
        }

        bool closed () final override {
            throw method::unimplemented {"TCP::session::closed"};
        }

        function<void (io_error)> HandleError;
        
        ~session () {
            close ();
        }

        static ptr<socket> connect (io::io_context &, const endpoint &);
    };

    // open a TCP connection.
    ptr<session> open (
        io::io_context &,
        endpoint,
        function<void (io_error)> error_handler,
        receive_handler<session, string_view>);

    class server {
        io::io_context& IO;
        io::ip::tcp::acceptor Acceptor;
        
        virtual ptr<session> new_session (ptr<socket>) = 0;
        
        void accept ();
        
    public:
        server (boost::asio::io_context& io_context, uint16 port);
    };
    
    inline server::server (io::io_context& io_context, uint16 port):
        IO (io_context), Acceptor (io_context, io::ip::tcp::endpoint (io::ip::tcp::v4 (), port)) {}
    
}

#endif

