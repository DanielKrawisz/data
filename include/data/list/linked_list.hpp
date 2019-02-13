#ifndef DATA_LIST_LINKED_LIST_HPP
#define DATA_LIST_LINKED_LIST_HPP

#include <data/list.hpp>
#include <data/data.hpp>
#include <data/tools/iterator_list.hpp>
#include <data/fold.hpp>
#include <type_traits>
    
namespace data {
        
    template <typename X>
    struct linked_list {
        constexpr static list::definition::complete<linked_list<X>, X, list::iterator<linked_list<X>, X>> is_list{};
        
        using next = ptr<list::node<X, linked_list>>;
            
        next Next;
            
        bool empty() const {
            return Next == nullptr;
        }
            
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const X& first() const {
            return Next->First;
        }
            
        const linked_list rest() const {
            if (empty()) return {};
                
            return Next->Rest;
        }
            
        const list::node<X, linked_list> operator*() {
            return *Next;
        }
            
        list::node<X, linked_list>& operator->() const {
            return Next;
        }
            
        linked_list<X> prepend(X x) const {
            return linked_list<X>{std::make_shared<list::node<X, linked_list>>(list::node<X, linked_list>{x, *this})};
        }

        linked_list<X> operator+(X x) const {
            return prepend(x);
        }

        bool contains(X x) const {
            if (empty()) return false;
                
            Next->contains(x);
        }
            
        bool operator==(const next n) {
            return Next == n;
        }
            
        bool operator==(const linked_list<X>& l) {
            if (this == &l) return true;
            return Next == l.Next;
        }
            
        const linked_list<X> from(uint n) const {
            if (empty()) return nullptr;
            if (n == 0) return this;
            return rest().from(n - 1);
        }
                
        const X operator[](uint n) const {
            auto l = from(n);
            if (l.empty()) return X{};
            return l.first();
        }
        
        linked_list() : Next{nullptr} {}
        linked_list(const linked_list<X>& l) : Next{l.Next} {}
        linked_list(linked_list<X>&& l) : Next{l.Next} {
            l.Next = nullptr;
        }
        
        linked_list(std::initializer_list<X> l);
        
        linked_list<X>& operator=(const linked_list<X>& l) {
            Next = l.Next;
            return *this;
        }

        list::iterator<linked_list<X>, X> begin() {
            return {*this};
        }
            
        list::iterator<linked_list<X>, X> end() {
            return {};
        }
        
    private:
        linked_list(next n) : Next{n} {}
    };

    template <typename X>
    inline bool empty(const data::linked_list<X> l) {
        return l.empty();
    }

    template <typename X>
    inline const X& first(const data::linked_list<X> l) {
        return l.first();
    }

    template <typename X>
    inline const data::linked_list<X> rest(const data::linked_list<X> l) {
        return l.rest();
    }

    template <typename X>
    inline const data::linked_list<X> prepend(const data::linked_list<X> l, const X elem) {
        return l.prepend(elem);
    }
    
    template <typename X>
    linked_list<X>::linked_list(std::initializer_list<X> l) : linked_list(list::reverse(fold(plus<linked_list<X>, X>, linked_list{}, iterator_list<decltype(l.begin()), X>(l.begin(), l.end())))) {}

}

#endif
