// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

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

        ptr<socket> session::connect(io::io_context &io, const endpoint &p) {
            ptr<socket> x {new socket (io)};
            io_error error;
            x->connect(io::ip::tcp::endpoint (p), error);

            if (error) throw exception {error};
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
        
        ptr<session> open (
            io::io_context &context,
            endpoint e,
            function<void (io_error)> error_handler,
            receive_handler<session, string_view> receive) {

            ptr<session> ss { new session {session::connect (context, e), error_handler}};

            ss->wait_for_message (receive, error_handler);

            return ss;
        }
    }
}
