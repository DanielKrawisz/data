// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include "list.hpp"
#include <exception>

namespace data {

    template <typename x>
    struct istream {
        virtual void operator>>(x&) = 0;
        virtual void operator>>(std::vector<x>&) = 0;
    };
    
    template <typename x> 
    struct ostream {
        virtual void operator<<(const x) = 0;
        virtual void operator<<(const std::vector<x>&) = 0;
    };
    
    class reader : public virtual istream<byte> {
        virtual void operator>>(uint16_t&) = 0;
        virtual void operator>>(uint32_t&) = 0;
        virtual void operator>>(uint64_t&) = 0;
        virtual void operator>>(int16_t&) = 0;
        virtual void operator>>(int32_t&) = 0;
        virtual void operator>>(int64_t&) = 0;
    };
    
    class writer : public virtual ostream<byte> {
        virtual void operator<<(const uint16_t) = 0;
        virtual void operator<<(const uint32_t) = 0;
        virtual void operator<<(const uint64_t) = 0;
        virtual void operator<<(const int16_t) = 0;
        virtual void operator<<(const int32_t) = 0;
        virtual void operator<<(const int64_t) = 0;
    };
    
    static const string EndOfStreamError = string{"End of stream"};
    
    struct end_of_stream : std::exception {
        const char* what() const noexcept final override {
            return EndOfStreamError.c_str();
        }
    };
    
    template <typename x>
    struct stream : public istream<x>, public ostream<x> {};

}

#endif
