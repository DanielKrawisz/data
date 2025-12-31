// Copyright (c) 2019-2021 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_STREAM
#define DATA_STREAM

#include <exception>
#include <data/concepts.hpp>
#include <data/slice.hpp>

namespace data {
    struct end_of_stream : exception {
        end_of_stream () : exception {"end of stream"} {}
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
    writer<word> &operator << (writer<word> &w, slice<const word> x);
    
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

    template <typename W, typename word> concept Writer =
        Same<W, writer<word> &> || std::derived_from<W, writer<word>>;

    template <typename R, typename word> concept Reader =
        Same<R, reader<word> &> || std::derived_from<R, reader<word>>;

    template <typename W, typename d, typename word> concept Builder =
        Writer<W, word> && requires (d &result) {
            W {result};
        };

    template <typename result, typename word, typename builder,
        std::invocable<builder &> F, typename ...X>
    result build_with (F &&f, X &&...x) {
        result r; {
            builder b {r, x...};
            f (b);
        }
        return r;
    }

    namespace {
        template <typename X>
        writer<byte> &write_to_writer (writer<byte> &w, X &&x) {
            return w << x;
        }

        template <typename X, typename... P>
        writer<byte> &write_to_writer (writer<byte> &w, X &&x, P &&...p) {
            return write_to_writer (write_to_writer (w, std::forward<X> (x)), std::forward<P> (p)...);
        }
    }

    template <typename result, typename word, Builder<result, word> builder,
        typename ...X>
    result build (X &&...x) {
        result r; {
            builder b {r};
            write_to_writer (b, std::forward<X> (x)...);
        }
        return r;
    }

    // a message writer has the concept of an end to a message.
    template <typename message, std::integral word>
    struct message_writer : virtual writer<word> {
        virtual message complete () = 0;
        virtual ~message_writer () {};
    };

    // a message writer has the concept of an end to a message.
    template <std::integral word> using out_session = message_writer<void, word>;

    template <std::integral word>
    struct in_session : virtual reader<word> {
        virtual void complete () = 0;
        virtual ~in_session () {};
    };

    // this ensures that every definition made with writer will work with message_reader (and writer).
    template <typename X, std::integral word, typename T> requires requires (writer<word> &w, const T &x) {
        { w << x };
    } message_writer<X, word> inline &operator << (message_writer<X, word> &w, const T &x) {
        static_cast<writer<word> &> (w) << x;
        return w;
    }

    template <std::integral word, typename T> requires requires (reader<word> &w, T &x) {
        { w >> x };
    } in_session<word> inline &operator >> (in_session<word> &w, T &x) {
        static_cast<reader<word> &> (w) >> x;
        return w;
    }

    struct end_message {};

    template <typename message, std::integral word>
    message inline operator << (message_writer<message, word> &w, end_message) {
        return w.complete ();
    }

    template <std::integral word>
    void inline operator >> (in_session<word> &w, end_message) {
        return w.complete ();
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
    writer<word> inline &operator << (writer<word> &w, slice<const word> x) {
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

