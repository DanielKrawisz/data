// Copyright (c) 2021 Daniel Krawisz
// Distributed under the Open BSV software license, see the accompanying file LICENSE.

#ifndef DATA_CONNECTION
#define DATA_CONNECTION

#include <data/cross.hpp>

namespace data::networking {
    
    template <typename from, typename to = from>
    struct connection {
        virtual ~connection() {}
        virtual void send(to) = 0;
        // will be called asynchronously when a message is received. 
        virtual void receive(from) = 0;
    };
    
    template <typename word, typename from, typename to = from>
    struct serialized_connection : connection<const from &, const to &>, parser<word, from>, virtual connection<std::view<word>> {
        using connection<bytes_view>::send;
        using connection<const from &, const to &>::receive;
        
        virtual bytes serialize(const to &m);
        
        void send(const to &m) final override;
        
        virtual ~serialized_connection() {}
        
        virtual void receive(bytes_view b) final override;
        
        void parsed(const from &m) final override;
        
    };
    
    template <typename from, typename to = from>
    struct receiver : virtual connection<from, to> {
        virtual ~receiver() {}
        std::function<void(from)> Receive;
        void receive(from x) final override {
            Receive(x);
        }
        
        receiver(std::function<void(from)> receive): Receive{receive} {}
    };
    
    template <typename from, typename to>
    bytes inline serialized_connection<from, to>::serialize(const to &m) {
        return bytes(m);
    }
    
    template <typename from, typename to>
    void inline serialized_connection<from, to>::send(const to &m) {
        send(bytes_view(serialize(m)));
    }
    
    template <typename from, typename to>
    void inline serialized_connection<from, to>::receive(bytes_view b) {
        this->write(b.data(), b.size());
    }
    
    template <typename from, typename to>
    void inline serialized_connection<from, to>::parsed(const from &m) {
        receive(m);
    }
    
}

#endif


