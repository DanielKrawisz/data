// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST_LINKED
#define DATA_LIST_LINKED

#include <data/tools/iterator_list.hpp>
#include <data/fold.hpp>
#include <type_traits>
    
namespace data::list {
    
    template <typename elem>
    struct linked {
        using returned = typename container::returned<elem>::type;
        using requirement = data::list::definition::buildable<linked<elem>, elem, returned>;
        constexpr static requirement Satisfied{};
        
        using node = list::node<elem, linked, returned>;
        using next = ptr<node>;
        
        next Next;
        
        // if the list is empty, then this function
        // will dereference a nullptr. It is your
        // responsibility to check. 
        const returned first() const {
            return Next->first();
        }
        
        returned first() {
            return Next->first();
        }
        
        bool empty() const {
            return Next == nullptr;
        }
        
        const linked rest() const {
            if (empty()) return {};
            
            return Next->rest();
        }
        
        bool contains(elem x) const {
            if (empty()) return false;
                
            Next->contains(x);
        }
        
        uint32 size() const {
            if (empty()) return 0;
                
            return Next->size();
        }
        
        linked prepend(elem x) const {
            return linked{std::make_shared<node>(node{x, *this})};
        }
        
        linked operator+(elem x) const {
            return prepend(x);
        }
        
        bool operator==(const next n) {
            return Next == n;
        }
        
        bool operator==(const linked& l) const {
            if (this == &l) return true;
            return Next == l.Next;
        }
        
        bool operator!=(const linked& l) const {
            return !(*this==l);
        }
        
        const linked from(uint32 n) const {
            if (empty()) return {};
            if (n == 0) return *this;
            return rest().from(n - 1);
        }
        
        const returned operator[](uint32 n) const {
            return from(n).first();
        }
        
        linked() : Next{nullptr} {}
        linked(const linked& l) : Next{l.Next} {}
        linked(linked&& l) : Next{l.Next} {
            l.Next = nullptr;
        }
        
        linked(std::initializer_list<elem> l);
        linked(std::vector<elem> v);
        
        linked& operator=(const linked& l) {
            Next = l.Next;
            return *this;
        } 
        
    private:
        linked(next n) : Next{n} {}
    };
}

namespace data {
    
    template <typename X>
    inline bool empty(const list::linked<X> l) {
        return l.empty();
    }
    
    template <typename X>
    inline typename list::linked<X>::returned first(const list::linked<X> l) {
        return l.first();
    }
    
    template <typename X>
    inline const list::linked<X> rest(const list::linked<X> l) {
        return l.rest();
    }
    
    template <typename X>
    inline const list::linked<X> prepend(const list::linked<X> l, const X elem) {
        return l.prepend(elem);
    }
    
    template <typename X>
    list::linked<X>::linked(std::initializer_list<X> l) : linked(
        list::reverse(fold(plus<linked, X>, linked{}, iterator_list<decltype(l.begin()), X>(l.begin(), l.end())))) {}

}

#endif
