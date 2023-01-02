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
    struct serialized_connection : connection<const from &, const to &>, parser<word, from>, virtual connection<std::basic_string_view<word>> {
        using connection<std::basic_string_view<word>>::send;
        using connection<const from &, const to &>::receive;
        
        virtual std::basic_string<word> serialize(const to &m);
        
        void send(const to &m) final override;
        
        virtual ~serialized_connection() {}
        
        virtual void receive(std::basic_string_view<word> b) final override;
        
        void parsed(const from &m) final override;
        
    };
    
    template <typename from, typename to = from>
    struct receiver : virtual connection<from, to> {
        virtual ~receiver() {}
        std::function<void (from)> Receive;
        void receive(from x) final override {
            Receive(x);
        }
        
        receiver(std::function<void(from)> receive): Receive{receive} {}
    };
    
    template <typename word, typename from, typename to>
    void inline serialized_connection<word, from, to>::send(const to &m) {
        send (std::basic_string_view<word> (serialize (m)));
    }
    
    template <typename word, typename from, typename to>
    void inline serialized_connection<word, from, to>::receive(std::basic_string_view<word> b) {
        this->write(b.data(), b.size());
    }
    
    template <typename word, typename from, typename to>
    void inline serialized_connection<word, from, to>::parsed(const from &m) {
        receive(m);
    }
    
}

#endif


