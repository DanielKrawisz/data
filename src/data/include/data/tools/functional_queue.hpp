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
    template <typename stack, typename element = element_of<stack>>
    requires functional::stack<stack, element> 
    struct functional_queue {
        
        functional_queue ();
        explicit functional_queue (const element &x);
        explicit functional_queue (const functional_queue &l, const element &e);
        explicit functional_queue (const element &e, const functional_queue &l);
        functional_queue (stack l);

        functional_queue (std::initializer_list<wrapped<element>> init) {
            for (int i = 0; i < init.size (); i++) *this = append (*(init.begin () + i));
        }
        
        bool empty () const;
        
        size_t size () const;
        bool valid () const;
        
        const element &first () const;
        const element &operator [] (uint32 i) const;
        
        functional_queue rest () const;
        const element last () const;
        
        functional_queue append (const element &e) const;
        functional_queue prepend (const element &e) const;
        functional_queue append (functional_queue q) const;
        
        functional_queue operator << (const element &e) const;
        
        functional_queue &operator <<= (const element &e) {
            return *this = *this << e;
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
            return data::reverse (Right).prepend (data::reverse (Left));
        }

        template <typename X, typename Y, typename ... P>
        functional_queue append (X x, Y y, P... p) const;

        template <typename X, typename ... P>
        functional_queue append (functional_queue q, X x, P... p) const {
            return append (q).append (x, p...);
        }

        template <data::sequence X> requires std::equality_comparable_with<element, data::element_of<X>>
        bool operator == (const X &x) const {
            return sequence_equal (*this, x);
        }

        template <typename Z> requires implicitly_convertible_to<stack, Z>
        operator functional_queue<Z> () const {
            return functional_queue<Z> {Z (Left), Z (Right)};
        }

        // explicit c
        template <typename Z> requires explicitly_convertible_to<stack, Z>
        explicit operator functional_queue<Z> () const {
            return functional_queue<Z> {Z (Left), Z (Right)};
        }

    private:
        stack Left;
        stack Right;

        functional_queue (stack l, stack r);
        
        static functional_queue check (const stack &l, const stack &r);
        
        template <typename Z, typename E> requires functional::stack<Z, E> friend struct functional_queue;
    
    };

    template <typename X>
    functional_queue<X> inline operator + (const functional_queue<X> a, const functional_queue<X> b) {
        return a.append (b);
    }

    template <typename stack, typename element = element_of<stack>>
    requires requires (std::ostream &o, const element &e) {
        { o << e } -> std::same_as<std::ostream &>;
    } std::ostream inline &operator << (std::ostream &o, const functional_queue<stack, element> n) {
        return functional::write (o, n);
    }

    template <typename stack, typename element>
    auto inline last (functional_queue<stack, element> q) {
        return q.last ();
    }

    template <typename stack, typename element> requires functional::stack<stack, element>
    inline functional_queue<stack, element>::functional_queue (stack l, stack r) : Left {l}, Right {r} {}
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    inline functional_queue<stack, element>::functional_queue () : Left {}, Right {} {}
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    inline functional_queue<stack, element>::functional_queue (const element &x) : Left {x}, Right {} {}
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    inline functional_queue<stack, element>::functional_queue (stack l) : Left {l}, Right {} {}
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    bool inline functional_queue<stack, element>::empty () const {
        return data::empty (Left);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    size_t inline functional_queue<stack, element>::size () const {
        return data::size (Left) + data::size (Right);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    bool inline functional_queue<stack, element>::valid () const {
        return Left.valid () && Right.valid ();
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    const element inline &functional_queue<stack, element>::first () const {
        return data::first (Left);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    const element &functional_queue<stack, element>::operator [] (uint32 i) const {
        if (i >= size ()) throw exception {} << "index " << i << " called on queue of size " << size ();
        uint32 left = Left.size ();
        if (i >= left) return Right[Right.size () - (i - left) - 1];
        return Left[i];
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> functional_queue<stack, element>::check (const stack &l, const stack &r) {
        if (l.empty ()) {
            if (!r.empty ()) return functional_queue {data::reverse (r), stack {}};
            return functional_queue {};
        } else return functional_queue (l, r);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::rest () const {
        return check (Left.rest (), Right);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    const element inline functional_queue<stack, element>::last () const {
        return check (Right, Left).Left.first ();
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::append (const element &e) const {
        return check (Left, Right << e);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::prepend (const element &e) const {
        return check (Left << e, Right);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::append (functional_queue q) const {
        if (q.empty ()) return *this;
        return append (q.first ()).append (q.rest ());
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    template <typename X, typename Y, typename ... P>
    functional_queue<stack, element> inline functional_queue<stack, element>::append (X x, Y y, P... p) const {
        return append (element (x)).append (y, p...);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::operator << (const element &e) const {
        return append (e);
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    functional_queue<stack, element> inline functional_queue<stack, element>::make () {
        return functional_queue {};
    }
    
    template <typename stack, typename element> requires functional::stack<stack, element>
    template <typename A, typename ... M>
    functional_queue<stack, element> inline functional_queue<stack, element>::make (const A x, M... m) {
        return make (m...).prepend (x);
    }
    
}

#endif
