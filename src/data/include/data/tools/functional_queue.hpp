// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_FUNCTIONAL_QUEUE
#define DATA_TOOLS_FUNCTIONAL_QUEUE

#include <data/concepts.hpp>
#include <data/functional/list.hpp>
#include <data/reverse.hpp>
#include <data/fold.hpp>
    
namespace data {

    // functional queue based on Milewski's implementation of Okasaki. 
    // it is built out of any stack. 
    // NOTE: it should be possible to get rid of type parameter element. 
    template <Stack stack, typename element> requires Sequence<stack, element>
    struct functional_queue;

    template <typename stack, typename element> 
    bool empty (const functional_queue<stack, element> &x);

    template <typename stack, typename element> 
    size_t size (const functional_queue<stack, element> &x);
    
    template <typename stack, typename element> 
    functional_queue<stack, element> values (const functional_queue<stack, element> &x);

    template <typename X, typename E>
    functional_queue<X, E> inline operator + (const functional_queue<X, E> a, const functional_queue<X, E> b);

    template <typename stack, typename element>
    requires requires (std::ostream &o, const element &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream &operator << (std::ostream &o, const functional_queue<stack, element> n);
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    struct functional_queue {
        
        functional_queue ();
        explicit functional_queue (const element &x);
        explicit functional_queue (const functional_queue &l, const element &e);
        explicit functional_queue (inserted<element> e, const functional_queue &l);
        functional_queue (stack l);

        functional_queue (std::initializer_list<wrapped<element>> init): functional_queue {} {
            for (int i = 0; i < init.size (); i++) *this = append (*(init.begin () + i));
        }
        
        bool empty () const;
        
        size_t size () const;
        bool valid () const;

        // we have to use const stack since we do not provide
        // non-const methods for first and []. 
        using retrieved = decltype (std::declval<const stack> ().first ());
        
        retrieved first () const;
        retrieved operator [] (uint32 i) const;
        
        functional_queue rest () const;
        
        functional_queue append (const element &e) const;
        functional_queue prepend (const element &e) const;
        functional_queue append (functional_queue q) const;
        
        functional_queue operator << (const element &e) const;
        
        functional_queue &operator <<= (const element &e) {
            return *this = *this << e;
        }

        functional_queue operator >> (const element &e) const {
            prepend (e);
        }
        
        functional_queue &operator >>= (const element &e) {
            return *this = *this >> e;
        }
        
        static functional_queue make ();
        
        template <typename A, typename ... M>
        static functional_queue make (const A x, M... m);
        
        using iterator = sequence_iterator<functional_queue>;
        using sentinel = data::sentinel<functional_queue>;
        
        iterator begin () const {
            return iterator {*this};
        }
        
        sentinel end () const {
            return sentinel {*this};
        }

        operator stack () const {
            return join (Left, Right);
        }

        template <typename X, typename Y, typename ... P>
        functional_queue append (X x, Y y, P... p) const;

        template <typename X, typename ... P>
        functional_queue append (functional_queue q, X x, P... p) const {
            return append (q).append (x, p...);
        }

        template <Sequence X> requires std::equality_comparable_with<element, decltype (std::declval<X> ().first ())>
        bool operator == (const X &x) const {
            return sequence_equal (*this, x);
        }

        template <typename Z, typename E> 
        requires ImplicitlyConvertible<stack, Z> && ImplicitlyConvertible<element, E>
        operator functional_queue<Z, E> () const {
            return functional_queue<Z, E> {Z (Left), Z (Right)};
        }

        // explicit c
        template <typename Z, typename E> 
        requires ExplicitlyConvertible<stack, Z> && ExplicitlyConvertible<element, E>
        explicit operator functional_queue<Z, E> () const {
            return functional_queue<Z, E> {Z (Left), Z (Right)};
        }

    private:
        stack Left;
        stack Right;

        functional_queue (stack l, stack r);
        
        static functional_queue check (const stack &l, const stack &r);
        
        template <Stack Z, typename E> requires Sequence<Z, E> friend struct functional_queue;
    
    };

    template <typename X, typename E>
    functional_queue<X, E> inline operator + (const functional_queue<X, E> a, const functional_queue<X, E> b) {
        return a.append (b);
    }

    template <typename stack, typename element>
    requires requires (std::ostream &o, const element &e) {
        { o << e } -> Same<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const functional_queue<stack, element> n) {
        return functional::write (o, n);
    }

    template <typename stack, typename element> 
    bool inline empty (const functional_queue<stack, element> &x) {
        return x.empty ();
    }

    template <typename stack, typename element> 
    size_t inline size (const functional_queue<stack, element> &x) {
        return x.size ();
    }
    
    template <typename stack, typename element> 
    functional_queue<stack, element> inline values (const functional_queue<stack, element> &x) {
        return x;
    }

    template <Stack stack, typename element> requires Sequence<stack, element>
    inline functional_queue<stack, element>::functional_queue (stack l, stack r) : Left {l}, Right {r} {}

    template <Stack stack, typename element> requires Sequence<stack, element>
    inline functional_queue<stack, element>::functional_queue () : Left {}, Right {} {}
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    inline functional_queue<stack, element>::functional_queue (const element &x) : Left {x}, Right {} {}

    template <Stack stack, typename element> requires Sequence<stack, element>
    inline functional_queue<stack, element>::functional_queue (stack l) : Left {l}, Right {} {}
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    bool inline functional_queue<stack, element>::empty () const {
        return data::empty (Left);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    size_t inline functional_queue<stack, element>::size () const {
        return data::size (Left) + data::size (Right);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    bool inline functional_queue<stack, element>::valid () const {
        return Left.valid () && Right.valid ();
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    typename functional_queue<stack, element>::retrieved inline functional_queue<stack, element>::first () const {
        return Left.first ();
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    typename functional_queue<stack, element>::retrieved functional_queue<stack, element>::operator [] (uint32 i) const {
        if (i >= size ()) throw empty_sequence_exception {};
        uint32 left = Left.size ();
        if (i >= left) return Right[Right.size () - (i - left) - 1];
        return Left[i];
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> functional_queue<stack, element>::check (const stack &l, const stack &r) {
        if (l.empty ()) {
            if (!r.empty ()) return functional_queue {data::reverse (r), stack {}};
            return functional_queue {};
        } else return functional_queue (l, r);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::rest () const {
        return check (Left.rest (), Right);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::append (const element &e) const {
        return check (Left, data::prepend (Right, e));
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::prepend (const element &e) const {
        return check (data::prepend (Left, e), Right);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::append (functional_queue q) const {
        if (q.empty ()) return *this;
        return append (q.first ()).append (q.rest ());
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    template <typename X, typename Y, typename ... P>
    functional_queue<stack, element> inline functional_queue<stack, element>::append (X x, Y y, P... p) const {
        return append (element (x)).append (y, p...);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::operator << (const element &e) const {
        return append (e);
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::make () {
        return functional_queue {};
    }
    
    template <Stack stack, typename element> requires Sequence<stack, element>
    template <typename A, typename ... M>
    functional_queue<stack, element> inline functional_queue<stack, element>::make (const A x, M... m) {
        return make (m...).prepend (x);
    }
    
}

#endif
