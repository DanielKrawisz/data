// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_SESSION
#define DATA_SESSION

#include <data/cross.hpp>

namespace data::networking {
    
    template <typename from, typename to = from>
    struct session {
        virtual ~session() {}
        virtual void send(to) = 0;
        // will be called asynchronously when a message is received. 
        virtual void receive(from) = 0;
    };
    
    template <typename from>
    struct parser : public writer<byte> {
        virtual ~parser() {}
        virtual void parsed(const from &) = 0;
        
        virtual void parse_error(const string &invalid) = 0;
    };
    
    template <typename from, typename to = from>
    struct serialized_session : session<const from &, const to &>, parser<from>, virtual session<bytes_view> {
        using session<bytes_view>::send;
        using session<const from &, const to &>::receive;
        
        virtual bytes serialize(const to &m);
        
        void send(const to &m) final override;
        
        virtual ~serialized_session() {}
        
        virtual void receive(bytes_view b) final override;
        
        void parsed(const from &m) final override;
        
    };
    
    template <typename from, typename to = from>
    struct receiver : virtual session<from, to> {
        virtual ~receiver() {}
        std::function<void(from)> Receive;
        void receive(from x) final override {
            Receive(x);
        }
    };
    
    template <typename from, typename to>
    bytes inline serialized_session<from, to>::serialize(const to &m) {
        return bytes(m);
    }
    
    template <typename from, typename to>
    void inline serialized_session<from, to>::send(const to &m) {
        send(bytes_view(serialize(m)));
    }
    
    template <typename from, typename to>
    void inline serialized_session<from, to>::receive(bytes_view b) {
        this->write(b.data(), b.size());
    }
    
    template <typename from, typename to>
    void inline serialized_session<from, to>::parsed(const from &m) {
        receive(m);
    }
    
}

#endif

