#ifndef DATA_TOOLS_ITERATOR_LIST_HPP
#define DATA_TOOLS_ITERATOR_LIST_HPP

#include <data/list.hpp>

namespace data {
        
    template <typename X, typename it>
    struct iterator_list {
        constexpr static const list::definition::list<iterator_list<X, it>, X> is_list{};
        
        it Next;
        it End;
            
        bool empty() const {
            return Next == End;
        }
            
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const X& first() const {
            return *Next;
        }
            
        const iterator_list rest() const {
            if (empty()) return *this;
                
            return iterator_list{End, Next++};
        }
                
        bool contains(X x) const {
            if (empty()) return false;
                
            Next->contains(x);
        }
            
        bool operator==(const iterator_list<X, it>& l) {
            if (this == &l) return true;
            return Next == l.Next && End == l.End;
        }
            
        iterator_list<X, it> from(uint n) const {
            if (empty()) return nullptr;
            if (n == 0) return this;
            return rest().from(n - 1);
        }
            
        iterator_list(it n, it e) : Next{n}, End{e} {}
            
        it begin() {
            return Next;
        }
            
        it end() {
            return End;
        }
            
    };

    template <typename X, typename it>
    inline bool empty(const data::iterator_list<X, it> l) {
        return l.empty();
    }

    template <typename X, typename it>
    inline const X& first(const data::iterator_list<X, it> l) {
        return l.first();
    }

    template <typename X, typename it>
    inline const data::iterator_list<X, it> rest(const data::iterator_list<X, it> l) {
        return l.rest();
    }

}

#endif
