// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_FUNCTIONAL_QUEUE
#define DATA_TOOLS_FUNCTIONAL_QUEUE

#include <data/functional/list.hpp>
#include <data/fold.hpp>
    
namespace data::tool {
    
    // functional queue based on Milewski's implementation of Okasaki. 
    // it is built out of any stack. 
    template <typename stack, 
        typename element = std::remove_reference_t<decltype(std::declval<stack>().first())>>
    requires functional::stack<stack, element> 
    struct functional_queue {
        
        functional_queue();
        functional_queue(const element& x);
        functional_queue(stack l);
        
        template <typename X, typename Y, typename ... P>
        functional_queue(X x, Y y, P... p);
        
        bool empty() const;
        
        size_t size() const;
        bool valid() const;
        
        const element& first() const;
        const element& operator[](uint32 i);
        
        functional_queue rest() const;
        const element last() const;
        
        functional_queue append(const element e) const;
        functional_queue prepend(const element e) const;
        functional_queue append(functional_queue q) const;
        
        template <typename X, typename Y, typename ... P>
        functional_queue append(X x, Y y, P... p) const;
        
        functional_queue operator<<(const element e) const;
        functional_queue operator<<(const functional_queue q) const;
        
        functional_queue &operator<<=(const element e) {
            return *this = *this << e;
        }
        
        functional_queue &operator<<=(const functional_queue q) {
            return *this = *this << q;
        }
        
        functional_queue reverse() const;
        
        bool operator==(const functional_queue& q) const;
        bool operator!=(const functional_queue& q) const;
        
        static functional_queue make();
        
        template <typename A, typename ... M>
        static functional_queue make(const A x, M... m);
        
        using const_iterator = typename stack::const_iterator;
        
        const const_iterator begin() const {
            return values().begin();
        }
        
        const const_iterator end() const {
            return values().end();
        }
        
    private:
        stack Left;
        stack Right;

        functional_queue(stack l, stack r);
        
        static functional_queue check(const stack& l, const stack& r);
        
        const stack values() const {
            return data::join(data::reverse(Right), data::reverse(Left));
        }
    
    };

    template <typename X> 
    std::ostream& operator<<(std::ostream& o, const functional_queue<X> n) {
        return functional::write(o, n);
    }

    template <typename stack, typename element>
    inline functional_queue<stack, element>::functional_queue(stack l, stack r) : Left{l}, Right{r} {}
    
    template <typename stack, typename element>
    inline functional_queue<stack, element>::functional_queue() : Left{}, Right{} {}
    
    template <typename stack, typename element>
    inline functional_queue<stack, element>::functional_queue(const element& x) : Left{x}, Right{} {}
    
    template <typename stack, typename element>
    inline functional_queue<stack, element>::functional_queue(stack l) : Left{l}, Right{} {}
    
    template <typename stack, typename element>
    template <typename X, typename Y, typename ... P>
    inline functional_queue<stack, element>::functional_queue(X x, Y y, P... p) : 
        functional_queue{functional_queue{}.append(x, y, p...)} {}
    
    template <typename stack, typename element>
    inline bool functional_queue<stack, element>::empty() const {
        return data::empty(Left);
    }
    
    template <typename stack, typename element>
    inline size_t functional_queue<stack, element>::size() const {
        return data::size(Left) + data::size(Right);
    }
    
    template <typename stack, typename element>
    inline bool functional_queue<stack, element>::valid() const {
        return Left.valid() && Right.valid();
    }
    
    template <typename stack, typename element>
    inline const element& functional_queue<stack, element>::first() const {
        return data::first(Left);
    }
    
    template <typename stack, typename element>
    const element& functional_queue<stack, element>::operator[](uint32 i) {
        if (i >= size()) throw std::out_of_range("queue index");
        uint32 left = Left.size();
        if (i >= left) return Right[Right.size() - (i - left) - 1];
        return Left[i];
    }
    
    template <typename stack, typename element>
    functional_queue<stack, element> functional_queue<stack, element>::check(const stack& l, const stack& r) {
        if (l.empty()) {
            if (!r.empty()) return functional_queue{data::reverse(r), stack{}};
            return functional_queue{};
        } else return functional_queue(l, r);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::rest() const {
        return check(Left.rest(), Right);
    }
    
    template <typename stack, typename element>
    inline const element functional_queue<stack, element>::last() const {
        return check(Right, Left).Left.first();
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::append(const element e) const {
        return check(Left, Right << e);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::prepend(const element e) const {
        return check(Left << e, Right);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::append(functional_queue q) const {
        if (q.empty()) return *this;
        return append(q.first()).append(q.rest());
    }
    
    template <typename stack, typename element>
    template <typename X, typename Y, typename ... P>
    inline functional_queue<stack, element> functional_queue<stack, element>::append(X x, Y y, P... p) const {
        return append(x).append(y, p...);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::operator<<(const element e) const {
        return append(e);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::operator<<(const functional_queue q) const {
        return append(q);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::reverse() const {
        return check(Right, Left);
    }
    
    template <typename stack, typename element>
    bool functional_queue<stack, element>::operator==(const functional_queue& q) const {
        if (this == &q) return true;
        if (size() != q.size()) return false;
        if (size() == 0) return true;
        if (first() != q.first()) return false;
        return rest() == q.rest();
    }
    
    template <typename stack, typename element>
    inline bool functional_queue<stack, element>::operator!=(const functional_queue& q) const {
        return !operator==(q);
    }
    
    template <typename stack, typename element>
    inline functional_queue<stack, element> functional_queue<stack, element>::make() {
        return functional_queue{};
    }
    
    template <typename stack, typename element>
    template <typename A, typename ... M>
    inline functional_queue<stack, element> functional_queue<stack, element>::make(const A x, M... m) {
        return make(m...).prepend(x);
    }
    
}

#endif
