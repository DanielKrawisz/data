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
    protected:
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
    struct serialized_session : session<const from &, const to &>, 
        protected parser<from>, virtual protected session<bytes_view> {
        
        virtual bytes serialize(const to &m) {
            return bytes(m);
        }
        
        void send(const to &m) final override {
            session<bytes_view, bytes_view>::send(serialize(m));
        }
        
        virtual ~serialized_session() {}
        
    protected:
        void receive(bytes_view b) final override {
            this->write(b.data(), b.size());
        }
        
        void parsed(const from &m) final override {
            session<const from &, const to &>::receive(m);
        }
        
    };
    
}

#endif

