// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>
#include <data/slice.hpp>

namespace data {
    static const std::string EndOfStreamError {"End of stream"};
    
    struct end_of_stream : std::exception {
        const char *what () const noexcept final override {
            return EndOfStreamError.c_str ();
        }
    };

    template <std::integral word>
    struct writer {
        virtual void write (const word *, size_t size) = 0;
        virtual ~writer () {}
    };

    template <std::integral word>
    writer<word> &operator << (writer<word> &w, word x);

    template <std::integral word>
    writer<word> &operator << (writer<word> &w, view<word> x);
    
    template <std::integral word>
    writer<word> &operator << (writer<word> &w, const std::string &x);

    template <std::integral word>
    struct reader {
        virtual void read (word *, size_t size) = 0;
        virtual void skip (size_t size) = 0;
        virtual ~reader () {}
    };
    
    template <std::integral word>
    reader<word> inline &operator >> (reader<word> &r, slice<word> x) {
        r.read (x.data (), x.size ());
        return r;
    }
    
    template <std::integral word>
    reader<word> inline &operator >> (reader<word> &r, word &x) {
        r.read (&x, 1);
        return r;
    }

    // a message writer has the concept of an end to a message.
    template <typename message, std::integral word>
    struct message_writer : virtual writer<word> {
        virtual message complete () = 0;
        virtual ~message_writer () {};
    };

    template <typename message, std::integral word, typename T> requires requires (writer<word> &w, const T &x) {
        { w << x };
    } message_writer<message, word> &operator << (message_writer<message, word> &w, const T &x) {
        static_cast<writer<word> &> (w) << x;
        return w;
    }

    // a message writer has the concept of an end to a message.
    template <std::integral word>
    struct session : virtual writer<word> {
        virtual void complete () = 0;
        virtual ~session () {};
    };

    template <std::integral word, typename T> requires requires (writer<word> &w, const T &x) {
        { w << x };
    } session<word> &operator << (session<word> &w, const T &x) {
        static_cast<writer<word> &> (w) << x;
        return w;
    }

    struct end_message {};

    template <typename message, std::integral word>
    message inline operator << (message_writer<message, word> &w, end_message) {
        return w.complete ();
    }

    template <std::integral word>
    session<word> inline operator << (session<word> &w, end_message) {
        w.complete ();
        return w;
    }

    // you have to know how big the string is going to be in
    // order to use this.
    template <typename word, std::output_iterator<word> it>
    struct iterator_writer final : virtual writer<word> {

        it Begin;
        it End;
        
        iterator_writer (it b, it e) : Begin {b}, End {e} {}
        
        void write (const word *b, size_t size) final override {
            for (int i = 0; i < size; i++) {
                if (Begin == End) throw end_of_stream {};
                *Begin = b[i];
                ++Begin;
            }
        }
        
    };

    template <typename it>
    iterator_writer (it b, it e) -> iterator_writer<unref<decltype (*std::declval<it> ())>, it>;
    
    template <std::input_iterator it>
    struct iterator_reader final : virtual reader<std::remove_const_t<unref<decltype (*std::declval<it> ())>>> {
        using word = std::remove_const_t<unref<decltype (*std::declval<it> ())>>;

        it Begin;
        it End;
        iterator_reader (const it b, const it e) : Begin {b}, End {e} {}
        
        bool empty () const {
            return Begin == End;
        }

        void read (word *b, size_t size) final override {
            for (int i = 0; i < size; i++) {
                if (empty ()) throw end_of_stream {};
                b[i] = *Begin;
                ++Begin;
            }
        }
        
        void skip (size_t size) final override {
            if (Begin + size > End) throw end_of_stream {};
            Begin += size;
        }
    };

    template <std::input_iterator it>
    iterator_reader (it b, it e) -> iterator_reader<it>;

    template <std::integral word>
    writer<word> inline &operator << (writer<word> &w, word x) {
        w.write (&x, 1);
        return w;
    }

    template <std::integral word>
    writer<word> inline &operator << (writer<word> &w, view<word> x) {
        w.write (x.data (), x.size ());
        return w;
    }

    template <std::integral word>
    writer<word> inline &operator << (writer<word> &w, const std::string &x) {
        w.write ((const byte *) (x.data ()), x.size ());
        return w;
    }
    
}

#endif

