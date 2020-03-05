// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP_QUEUE
#define DATA_MAP_QUEUE

#include <data/stack.hpp>
#include <data/list.hpp>
#include <data/fold.hpp>
    
namespace data::tool {
    
    // functional queue based on Milewski's implementation of Okasaki. 
    // it is built out of any stack. 
    template <typename stack>
    class functional_queue {
        
        // construction of functional_queue will fail if L 
        // does not satisfy the stack interface. 
        using is_stack = typename interface::stack<stack>;
        constexpr static is_stack Required{};
        
        stack Left;
        stack Right;

        functional_queue(stack l, stack r) : Left{l}, Right{r} {}
    public:
        using element = typename interface::stack<stack>::element;
        
        functional_queue() : Left{}, Right{} {}
        functional_queue(stack l) : Left{l}, Right{} {}
        
        template <typename X, typename Y, typename ... P>
        functional_queue(X x, Y y, P... p) : functional_queue{} {
            append(x, y, p...);
        }
        
        bool empty() const {
            return data::empty(Left);
        }
        
        size_t size() const {
            return data::size(Left) + data::size(Right);
        }
        
        bool valid() const {
            return Left.valid() && Right.valid();
        }
        
        const element first() const {
            return data::first(Left);
        }
        
        const element operator[](uint32 i) {
            if (i >= size()) throw std::out_of_range("queue index");
            uint32 left = Left.size();
            if (i >= left) return Right[Right.size() - (i - left) - 1];
            return Left[i];
        }
        
    private:
        static functional_queue check(const stack& l, const stack& r) {
            if (l.empty()) {
                if (!r.empty()) return functional_queue{functional::stack::reverse(r), stack{}};
                return functional_queue{};
            } else return functional_queue(l, r);
        }
        
    public:
        functional_queue rest() const {
            return check(Left.rest(), Right);
        }
        
        const element last() const {
            return check(Right, Left).Left.first();
        }
        
        functional_queue append(const element e) const {
            return check(Left, Right << e);
        }
        
        functional_queue prepend(const element e) const {
            return check(Left << e, Right);
        }
        
        functional_queue operator<<(const element e) const {
            return append(e);
        }
        
        functional_queue append(functional_queue q) const {
            if (q.empty()) return *this;
            return append(q.first()).append(q.rest());
        }
        
        template <typename X, typename Y, typename ... P>
        functional_queue append(X x, Y y, P... p) const {
            return append(x).append(y, p...);
        }
        
        functional_queue operator + (functional_queue q) const {
            return append(q);
        }
        
        functional_queue reverse() const {
            return check(functional::stack::reverse(Left), functional::stack::reverse(Right));
        }
        
        bool operator==(const functional_queue& q) const {
            if (this == &q) return true;
            if (size() != q.size()) return false;
            if (first() != q.first()) return false;
            return rest() == q.rest();
        }
        
        bool operator!=(const functional_queue& q) const {
            return !operator=(q);
        }
        
        static functional_queue make() {
            return functional_queue{};
        }
        
        template <typename A, typename ... M>
        static functional_queue make(const A x, M... m) {
            return make(m...).prepend(x);
        }
    
    };
       
}

template <typename X, typename L> 
std::ostream& operator<<(std::ostream& o, const data::tool::functional_queue<X> n) {
    return data::functional::stack::write(o, n);
}

#endif
