#ifndef DATA_TOOLS_ITERATOR_LIST_HPP
#define DATA_TOOLS_ITERATOR_LIST_HPP

#include <type_traits>
#include <data/list.hpp>

namespace data {
        
    template <typename it>
    struct iterator_list {
        
        it Next;
        it End;

        iterator_list(it n, it e) : Next{n}, End{e} {}
        using Element = typename it::value_type;
        constexpr static const list::definition::list<iterator_list<it>, Element> is_list{};
            
        bool empty() const {
            return Next == End;
        }
            
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const Element& first() const {
            return *Next;
        }
            
        const iterator_list rest() const {
            if (empty()) return *this;
                
            return iterator_list{End, Next++};
        }
                
        bool contains(Element x) const {
            if (empty()) return false;
                
            Next->contains(x);
        }
            
        bool operator==(const iterator_list<it>& l) {
            if (this == &l) return true;
            return Next == l.Next && End == l.End;
        }
        
        iterator_list<it> from(uint n) const {
            if (empty()) return {};
            if (n == 0) return this;
            return rest().from(n - 1);
        }
            
        it begin() {
            return Next;
        }
            
        it end() {
            return End;
        }
            
    };

    template <typename it>
    inline bool empty(const data::iterator_list<it> l) {
        return l.empty();
    }

    template <typename it>
    inline const typename it::value_type& first(const data::iterator_list<it> l) {
        return l.first();
    }

    template <typename it>
    inline const data::iterator_list<it> rest(const data::iterator_list<it> l) {
        return l.rest();
    }

}

#endif
