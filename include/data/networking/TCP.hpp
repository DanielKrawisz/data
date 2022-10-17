// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_NETWORKING_TCP
#define DATA_NETWORKING_TCP

#include <data/networking/session.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>

namespace data::networking {
    
    namespace io = boost::asio;
    using io_error = boost::system::error_code;
    using tcp = io::ip::tcp;
    
    // https://dens.website/tutorials/cpp-asio
    
    // we need to use enable_shared_from_this because of the possibility that 
    // tcp_stream will go out of scope and be deleted before one of the 
    // handlers is called from async_read_until or async_write. 
    class tcp_session : virtual public session<bytes_view>, protected std::enable_shared_from_this<tcp_session> {
        
        tcp::socket &Socket;
        io::streambuf Buffer;
        
        // begin waiting for the next message asynchronously. 
        void wait_for_message();
        
        virtual void handle_error(const io_error &err) {
            std::cout << "tcp error: " << err.message() << "\n";
        } 
        
    public:
        // note: message cannot be longer than 65536 bytes or this function 
        // is not thread-safe. 
        void send(bytes_view) final override;
        
        // we schedule a wait new message as soon as the object is created. 
        tcp_session(tcp::socket &x) : Socket{x}, Buffer{65536} {
            wait_for_message();
        }
        
        virtual ~tcp_session() {
            Socket.close();
        }
    
    };
    
}

#endif

