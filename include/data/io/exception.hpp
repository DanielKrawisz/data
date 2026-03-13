// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_IO_EXCEPTION
#define DATA_IO_EXCEPTION

#include <exception>
#include <sstream>

namespace data {
    
    struct exception : std::exception {
        int Code;
        
        std::string message () const {
            return Message;
        }
        
        const char* what () const noexcept final override {
            return Message.c_str ();
        }
        
        template <typename X>
        void write (X x) {
            std::stringstream ss;
            ss << Message << x;
            Message = ss.str ();
        }
        
        template <typename X, typename... Y>
        void write (X x, Y... y) {
            write (x);
            write (y...);
        }
        
        template <typename X>
        exception &operator << (X x) {
            write (x);
            if (Code == 0) Code = 1;
            return *this;
        }
        
        exception () : Code {1}, Message {} {}

        exception (int code) : Code {code}, Message {} {}
        
        exception (const std::string &x) : Code {1}, Message {} {
            write (x);
        }

        template <typename derived> struct base;
        
    private:
        std::string Message;
    };

    // more specific exceptions can derive from this and preserve the << operation.
    template <typename derived>
    struct exception::base : exception {
        using exception::exception;

        template <typename X>
        derived &operator << (X x) {
            write (x);
            if (Code == 0) Code = 1;
            return dynamic_cast<derived &> (*this);
        }
    };

    struct out_of_range : exception::base<out_of_range> {
        out_of_range (const std::string &msg = "") {
            *this << "out of range";
            if (msg != "") *this << ": " << msg;
        }
    };
}

#endif

