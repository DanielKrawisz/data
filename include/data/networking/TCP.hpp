// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_TCP
#define DATA_NETWORKING_TCP

#include <data/networking/session.hpp>
#include <data/networking/asio/async_to.hpp>
#include <data/networking/asio/async_from.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <ctre.hpp>
#include <iostream>

namespace data::networking::IP {
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

namespace data::networking::IP::TCP {
    using socket = io::ip::tcp::socket;
    
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
    
    ptr<asio::session<string_view, const string &>> connect(io::io_context &, const endpoint &, std::function<bool (string_view)> receive);
    
    // https://dens.website/tutorials/cpp-asio
    
    // we need to use enable_shared_from_this because of the possibility that 
    // tcp_stream will go out of scope and be deleted before one of the 
    // handlers is called from async_read_until or async_write. 
    class session : public asio::session<string_view, const string &>,
        public asio::async_to<socket, char>,
        public asio::async_from<socket, char> {
        
        ptr<socket> Socket;

    public:
        void close ();
        bool closed () final override;

        void handle_error (const io_error &err) override;
        
        // we schedule a wait new message as soon as the object is created. 
        session (ptr<socket>);
        
        virtual ~session () {
            close ();
        }
        
        static ptr<socket> connect (io::io_context &, const endpoint &);
    
    };

    class server {
        io::io_context& IO;
        io::ip::tcp::acceptor Acceptor;
        
        virtual ptr<session> new_session (ptr<socket>) = 0;
        
        void accept ();
        
    public:
        server (boost::asio::io_context& io_context, std::uint16_t port);
    };
    
    void inline session::handle_error (const io_error &err) {
        throw exception {err};
    } 
    
    // we schedule a wait new message as soon as the object is created. 
    // ensure that the object is ready to receive messages before this constructor happens!
    inline session::session (ptr<socket> x) :
        asio::async_to<socket, char> {x.get()},
        asio::async_from<socket, char> {x.get()} {
        this->wait_for_message();
    }
    
    void inline session::close () {
        this->Socket->close();
    }
    
    inline server::server (boost::asio::io_context& io_context, std::uint16_t port):
        IO(io_context), Acceptor(io_context, io::ip::tcp::endpoint(io::ip::tcp::v4(), port)) {}
    
}

#endif

