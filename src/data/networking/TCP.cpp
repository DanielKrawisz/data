#include <data/networking/TCP.hpp>

namespace data::networking::IP {
    
    bool address::valid () const {
        io_error err {};
        auto addr = io::ip::make_address (static_cast<string> (*this), err);
        return !bool (err);
    }

    address::operator io::ip::address () const {
        io_error err {};
        auto addr = io::ip::make_address (static_cast<string> (*this), err);
        if (err) throw exception{err};
        return addr;
    }
    
    uint32 address::version () const {
        io_error err {};
        auto addr = io::ip::make_address (static_cast<string> (*this), err);
        return bool (err) ? 0 : addr.is_v4 () ? 4 : 6;
    }
    
    namespace TCP {

        // begin waiting for the next message asynchronously. 
        void session::wait_for_message () {
            Socket.async_read_some(boost::asio::buffer (Buffer, buffer_size),
                [self = shared_from_this()](const io_error& error, size_t bytes_transferred) -> void {
                    if (error) return self->handle_error(error);
                    
                    try {
                        self->receive(bytes_view{self->Buffer, bytes_transferred});
                        self->wait_for_message();
                    } catch (...) {
                        self->Socket.close();
                    }
                });
        }
        
        void session::send(bytes_view b) {
            boost::asio::async_write(Socket, io::buffer(b), io::transfer_all(), 
                [self = shared_from_this()](const io_error& error, size_t) -> void {
                    if (error) self->handle_error(error);
                });
        }

        socket session::connect(io::io_context &io, const endpoint &p) {
            socket x(io);
            io_error error;
            x.connect(io::ip::tcp::endpoint(p), error);

            if(error) throw exception{error};
            return x;
        }
            
        address endpoint::address() const {
            auto result = ctre::match<pattern>(*this);
            if (!bool(result)) return {};
            auto address_v4 = result.get<"V4">();
            return bool(address_v4) ? IP::address(result.get<"V6">().to_string()) : IP::address(address_v4.to_string());
        }

        uint16 endpoint::port() const {
            auto result = ctre::match<pattern>(*this);
            if (!bool(result)) return {};
            std::stringstream port_stream;
            port_stream << result.get<"port">().to_string();
            uint16 port;
            port_stream >> port;
            return port;
        }

        endpoint::operator io::ip::tcp::endpoint() const {
            auto result = ctre::match<pattern>(*this);
            if (!bool(result)) return {};
            auto address_v4 = result.get<"V4">();
            std::stringstream port_stream;
            port_stream << result.get<"port">().to_string();
            uint16 port;
            port_stream >> port;
            return io::ip::tcp::endpoint{
                io::ip::address(IP::address{bool(address_v4) ? result.get<"V6">().to_string() : address_v4.to_string()}), 
                port};
        }
        
        endpoint::endpoint(const IP::address &addr, uint16 port) {
            auto version = addr.version();
            if (version == 0) return;
            std::stringstream ss;
            if (addr.version() == 6) ss << "[" << addr << "]";
            ss << ":" << port;
            *this = endpoint{ss.str()};
        }
        
        bool endpoint::valid() const {
            auto result = ctre::match<pattern>(*this);
            if (!bool(result)) return false;
            auto address_v4 = result.get<"V4">();
            bool is_v4 = bool(address_v4);
            IP::address addr = is_v4 ? IP::address(address_v4.to_string()) : IP::address(result.get<"V6">().to_string());
            uint32 version = addr.version();
            if (version == 0 || (version == 4) != is_v4) return false;
            std::stringstream port_stream;
            port_stream << result.get<"port">().to_string();
            uint32 port;
            port_stream >> port;
            return port < 65536;
        }
        
        void server::accept() {
            Socket.emplace(IO);
            
            Acceptor.async_accept(*Socket, [&] (io_error error) {
                new_session(std::move(*Socket));
                accept();
            });
        }
        
        struct session_receiver final : receiver<bytes_view>, session {
            session_receiver(socket &&x, std::function<void(bytes_view)> receive) : 
                receiver<bytes_view>{receive}, session{std::move(x)} {} 
        };
        
        ptr<networking::session<bytes_view>> connect(
            io::io_context &io, const endpoint &e, 
            std::function<void(bytes_view)> receive) {
            return std::static_pointer_cast<networking::session<bytes_view>>(
                std::make_shared<session_receiver>(session::connect(io, e), receive));
        }
    }
}
