#ifndef DATA_MAP_QUEUE_HPP
#define DATA_MAP_QUEUE_HPP

#include <data/list.hpp>
#include <data/tools/iterator_list.hpp>
#include <data/queue.hpp>
#include <data/fold.hpp>
    
namespace data {
    
    // functional queue based on Milewski's implementation of Okasaki. 
    template <typename L>
    class functional_queue {
        L Left;
        L Right;
        
        using requirement = list::is_buildable<L>;
        constexpr static requirement Satisfied{};

        functional_queue(L l, L r) : Left{l}, Right{r} {}
    public:
        using element = typename requirement::element;
        
        functional_queue() : Left{}, Right{} {}

        functional_queue(L l) : Left{l}, Right{} {}
        
        bool empty() const {
            return list::empty(Left);
        }
        
        uint size() const {
            return list::size(Left) + list::size(Right);
        }
        
        inline element& first() {
            return list::first(Left);
        }
        
        functional_queue(const std::initializer_list<element> l) : functional_queue(reverse(fold(plus, L{}, make_iterator_list(l)))) {}
        
    private:
        
        static functional_queue check(L const & l, L const & r) {
            if (l.empty()) {
                if (!r.empty()) return functional_queue{list::reverse(r), L{}};
            } else return functional_queue(l, r);
        }
        
    public:
        functional_queue rest() const {
            return check(Left.rest(), Right);
        }
            
        functional_queue append(element e) const {
            return check(Left, Right + e);
        }
            
        functional_queue operator+(element e) const {
            return append(e);
        }

        functional_queue reverse() const {
            return check(functional_queue{list::reverse(Left), list::reverse(Right)});
        }

        constexpr static data::queue::definition::queue<functional_queue, element> require_is_queue{};
        constexpr static data::list::definition::extendable<functional_queue, element> require_is_buildable{};

    };
            
    template <typename L> 
    inline bool empty(functional_queue<L> q) {
        return q.empty();
    }

    template <typename L> 
    inline uint size(functional_queue<L> q) {
        return q.size();
    }

    template <typename L> 
    inline typename functional_queue<L>::element& first(functional_queue<L> q) {
        return q.first();
    }
            
    template <typename L> 
    inline L rest(functional_queue<L> q) {
        return q.rest();
    }
            
    template <typename L, typename X> 
    inline functional_queue<L> append(functional_queue<L> q, X x) {
        return q.append(x);
    }
            
    template <typename L> 
    inline functional_queue<L> reverse(functional_queue<L> q) {
        return q.reverse();
    }
    
}

#endif
