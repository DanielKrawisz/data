// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MAP_QUEUE
#define DATA_MAP_QUEUE

#include <data/list.hpp>
#include <data/tools/iterator_list.hpp>
#include <data/queue.hpp>
#include <data/fold.hpp>
    
namespace data {
    
    // functional queue based on Milewski's implementation of Okasaki. 
    template <typename X, typename L>
    class functional_queue {
        L Left;
        L Right;

        functional_queue(L l, L r) : Left{l}, Right{r} {}
    public:
        using required = list::is_buildable<L, X>;
        using returned = typename required::returned;
        constexpr static required Satisfied{};
        
        functional_queue() : Left{}, Right{} {}
        functional_queue(L l) : Left{l}, Right{} {}
        functional_queue(const functional_queue& q) : Left{q.Left}, Right{q.Right} {}
        
        bool empty() const {
            return list::empty(Left);
        }
        
        uint32 size() const {
            return list::size(Left) + list::size(Right);
        }
        
        returned first() {
            return list::first(Left);
        }
        
    private:
        static functional_queue check(const L& l, const L& r) {
            if (l.empty()) {
                if (!r.empty()) return functional_queue{list::reverse(r), L{}};
                return functional_queue{};
            } else return functional_queue(l, r);
        }
        
    public:
        functional_queue rest() const {
            return check(Left.rest(), Right);
        }
        
        returned last() const {
            return check(Right, Left).Left.first();
        }
        
        functional_queue append(X e) const {
            return check(Left, Right + e);
        }
        
        functional_queue operator+(X e) const {
            return append(e);
        }
        
        functional_queue append(L list) const {
            if (list.empty()) return *this;
            return append(list.first()).append(list.rest());
        }
        
        functional_queue operator+(L list) const {
            return append(list);
        }
        
        functional_queue append(functional_queue q) const {
            if (q.empty()) return *this;
            return append(q.first()).append(q.rest());
        }
        
        functional_queue operator+(functional_queue q) const {
            return append(q);
        }
        
        functional_queue reverse() const {
            return check(list::reverse(Left), list::reverse(Right));
        }
        
        constexpr static data::queue::definition::queue<functional_queue, returned> require_is_queue{};
        constexpr static data::list::definition::extendable<functional_queue, returned> require_is_buildable{};
    
    };
    
    template <typename X, typename L> 
    inline bool empty(functional_queue<X, L> q) {
        return q.empty();
    }
    
    template <typename X, typename L> 
    inline uint32 size(functional_queue<X, L> q) {
        return q.size();
    }
    
    template <typename X, typename L> 
    inline typename functional_queue<X, L>::returned first(functional_queue<X, L> q) {
        return q.first();
    }
    
    template <typename X, typename L> 
    inline functional_queue<X, L> rest(functional_queue<X, L> q) {
        return q.rest();
    }
    
    template <typename X, typename L> 
    inline functional_queue<X, L> append(functional_queue<X, L> q, X x) {
        return q.append(x);
    }
    
    template <typename X, typename L> 
    inline functional_queue<X, L> reverse(functional_queue<X, L> q) {
        return q.reverse();
    }
       
}

#endif
