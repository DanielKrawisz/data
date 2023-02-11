// Copyright (c) 2021-2023 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <data/net/TCP.hpp>
#include <data/net/async/message_queue.hpp>

namespace data::net::IP {
    
    bool address::valid () const {
        asio::error_code err {};
        auto addr = asio::ip::make_address (static_cast<string> (*this), err);
        return !bool (err);
    }

    address::operator asio::ip::address () const {
        asio::error_code err {};
        auto addr = asio::ip::make_address (static_cast<string> (*this), err);
        if (err) throw exception {err};
        return addr;
    }
    
    uint32 address::version () const {
        asio::error_code err {};
        auto addr = asio::ip::make_address (static_cast<string> (*this), err);
        return bool (err) ? 0 : addr.is_v4 () ? 4 : 6;
    }
    
    namespace TCP {
            
        address endpoint::address() const {
            auto result = ctre::match<pattern> (*this);
            if (!bool (result)) return {};
            auto address_v4 = result.get<"V4"> ();
            return bool (address_v4) ? IP::address (result.get<"V6"> ().to_string ()) : IP::address (address_v4.to_string ());
        }

        uint16 endpoint::port () const {
            auto result = ctre::match<pattern> (*this);
            if (!bool(result)) return {};
            std::stringstream port_stream;
            port_stream << result.get<"port"> ().to_string ();
            uint16 port;
            port_stream >> port;
            return port;
        }

        endpoint::operator asio::ip::tcp::endpoint () const {
            auto result = ctre::match<pattern> (*this);
            if (!bool(result)) return {};
            auto address_v4 = result.get<"V4"> ();
            std::stringstream port_stream;
            port_stream << result.get<"port"> ().to_string ();
            uint16 port;
            port_stream >> port;
            return asio::ip::tcp::endpoint{
                asio::ip::address (IP::address{bool (address_v4) ? result.get<"V6"> ().to_string () : address_v4.to_string ()}),
                port};
        }
        
        endpoint::endpoint (const IP::address &addr, uint16 port) {
            auto version = addr.version ();
            if (version == 0) return;
            std::stringstream ss;
            if (addr.version () == 6) ss << "[" << addr << "]";
            ss << ":" << port;
            *this = endpoint {ss.str ()};
        }
        
        bool endpoint::valid () const {
            auto result = ctre::match<pattern> (*this);
            if (!bool (result)) return false;
            auto address_v4 = result.get<"V4"> ();
            bool is_v4 = bool(address_v4);
            IP::address addr = is_v4 ? IP::address (address_v4.to_string ()) : IP::address (result.get<"V6"> ().to_string ());
            uint32 version = addr.version ();
            if (version == 0 || (version == 4) != is_v4) return false;
            std::stringstream port_stream;
            port_stream << result.get<"port"> ().to_string ();
            uint32 port;
            port_stream >> port;
            return port < 65536;
        }

        using socket = asio::socket<asio::ip::tcp::socket>;

        ptr<socket> connect (asio::io_context &io, const endpoint &p, asio::error_handler on_error, close_handler on_close) {
            ptr<asio::ip::tcp::socket> x {new asio::ip::tcp::socket (io)};
            asio::error_code error;
            x->connect (asio::ip::tcp::endpoint (p), error);

            if (error) throw exception {error};
            return ptr<socket> {new socket {x, on_error, on_close}};
        }
        
        void open (
            asio::io_context &context,
            endpoint end,
            asio::error_handler on_error,
            interaction<string_view, const string &> interact,
            close_handler on_close) {

            ptr<socket> ss = connect (context, end, on_error, on_close);

            ptr<session<const string &>> zz {static_cast<session<const string &> *>
                (new async::message_queue<const string &> {
                    std::static_pointer_cast<async::write_stream<const string &>> (ss)})};

            async::wait_for_message<string_view> (std::static_pointer_cast<async::read_stream<string_view>> (ss),
                [xx = interact (zz)] (string_view z) -> void {
                    return xx (z);
                });
        }

        void server::accept () {
            Socket.emplace (IO);

            Acceptor.async_accept (*Socket, [self = shared_from_this ()] (asio::error_code error) {
                ptr<asio::ip::tcp::socket> t {new asio::ip::tcp::socket {std::move (*self->Socket)}};

                auto on_error = [] (const asio::error_code &) -> void {};
                auto on_close = [] () -> void {};

                ptr<socket> ss {new socket {t, on_error, on_close}};

                ptr<session<const string &>> zz {static_cast<session<const string &> *>
                (new async::message_queue<const string &> {
                    std::static_pointer_cast<async::write_stream<const string &>> (ss)})};

                async::wait_for_message<string_view>
                (std::static_pointer_cast<async::read_stream<string_view>> (ss),
                    [xx = self->Interact (zz)] (string_view z) -> void {
                        return xx (z);
                    });

                self->accept ();
            });
        }
    }
}
