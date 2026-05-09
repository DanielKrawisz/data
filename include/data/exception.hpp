// Copyright (c) 2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_EXCEPTION
#define DATA_EXCEPTION

#include <exception>
#include <sstream>

/*
    Exception Utilities
    ===================

    This exception system provides stream-style message construction using
    operator <<, allowing exceptions to be assembled naturally in the same
    style as std::ostream.

    Basic Usage
    -----------

        throw exception {} << "failed to open file: " << path;

    Since all streamed values are forwarded through a std::ostream-compatible
    interface, anything writable to std::ostream may be appended:

        throw exception {}
            << "expected "
            << expected
            << ", got "
            << actual;

    Exceptions may optionally carry an error code or similar payload:

        throw exception {errno}
            << "socket bind failed";

        throw exception {404}
            << "resource not found: "
            << url;

    The message is constructed incrementally and may contain arbitrary
    streamable types, manipulators, formatting flags, etc.

        throw exception {}
            << std::hex
            << "invalid value 0x"
            << value;

    Custom Exception Types
    ----------------------

    New exception types may inherit from exception::base<T> in order to:

        * preserve stream-style message construction
        * customize behavior
        * add metadata
        * add constructors or helpers
        * integrate with existing exception hierarchies

    Example:

        struct parse_error : exception::base<parse_error> {};

        throw parse_error {}
            << "unexpected token at line "
            << line;

    Additional state may also be stored:

        struct network_error : exception::base<network_error> {
            int port {};

            network_error (int p): port {p} {}
        };

        throw network_error {8080}
            << "connection refused";

    The derived exception continues to support operator<< exactly like the
    base exception type.

    Notes
    -----

    * Messages are built lazily through stream insertion.
    * Any type compatible with std::ostream may be appended.
    * Stream manipulators such as std::hex, std::setw, std::setfill, etc.
      are supported.
    * The design allows concise exception construction without manual string
      concatenation.

    In practice this produces exception code that reads more like a flowing
    diagnostic sentence than a pile of stitched-together fragments.
*/

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

