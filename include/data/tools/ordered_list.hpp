// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <milewski/OrdList/OrdList.hpp>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_stack.hpp>
    
namespace data::tool {
    
    template <typename value> 
    class ordered_list {
        uint32 Size;
        milewski::okasaki::OrdList<value> Ordered;
    public:
        ordered_list();
        explicit ordered_list(const functional_queue<linked_stack<value>>& l);
        
        bool operator==(const ordered_list x) const;
        bool operator!=(const ordered_list x) const;
        
        bool empty() const;
        size_t size() const;
        
        ordered_list insert(const value& x) const;
        ordered_list operator<<(const value& x) const;
        
        ordered_list rest() const;
        const value& first() const;
        
        const value& operator[](uint32 n) const;
        
        const value& last() const;
        
        struct const_iterator {
            const_iterator() : Ordered{} {}
            
            const value& operator*() const;
            
            const_iterator& operator++();
            bool operator==(const const_iterator) const;
            bool operator!=(const const_iterator) const;
        private:
            milewski::okasaki::OrdList<value>* Ordered;
            const_iterator(milewski::okasaki::OrdList<value>* o) : Ordered{o} {}
            
            friend struct ordered_list;
        };
        
        const_iterator begin() const {
            return const_iterator{Ordered};
        }
        
        const_iterator end() const {
            return const_iterator{};
        }
        
    private:
        const value& last_private() const;
        
        ordered_list(uint32 size, milewski::okasaki::OrdList<value> ordered);
    };

    template <typename value>
    std::ostream& operator<<(std::ostream& o, const data::tool::ordered_list<value>& l) {
        o << "ordered_list{";
        if (!l.empty()) {
            data::tool::ordered_list<value> x = l;
            while(true) {
                o << x.first();
                x = x.rest();
                if (x.empty()) break;
                o << ", ";
            }
        }
        return o << "}";
    }
    
    template <typename value>
    inline ordered_list<value>::ordered_list() : Size{0}, Ordered{} {}
        
    template <typename value>
    ordered_list<value>::ordered_list(const functional_queue<linked_stack<value>>& l) : ordered_list{} {
        functional_queue<linked_stack<value>> g = l;
        while(!data::empty(g)) {
            *this << g.first();
            g = g.rest();
        }
    }
        
    template <typename value>
    bool ordered_list<value>::operator==(const ordered_list x) const {
        if (Size != x.Size) return false;
        if (Size == 0) return true;
        if (first() != x.first()) return false;
        return rest() == x.rest();
    }
    
    template <typename value>
    bool ordered_list<value>::operator!=(const ordered_list x) const {
        return !operator==(x);
    }
    
    template <typename value>
    bool ordered_list<value>::empty() const {
        return Size == 0;
    }
    
    template <typename value>
    size_t ordered_list<value>::size() const {
        return Size;
    }
    
    template <typename value>
    ordered_list<value> ordered_list<value>::insert(const value& x) const {
        return {Size + 1, Ordered.inserted(x)};
    }
    
    template <typename value>
    ordered_list<value> ordered_list<value>::operator<<(const value& x) const {
        return {Size + 1, Ordered.inserted(x)};
    }
    
    template <typename value>
    ordered_list<value> ordered_list<value>::rest() const {
        if (Size == 0) return {};
        return {Size - 1, Ordered.popped_front()};
    }
    
    template <typename value>
    const value& ordered_list<value>::first() const {
        return Ordered.front();
    }
    
    template <typename value>
    const value& ordered_list<value>::operator[](uint32 n) const {
        if (n >= Size) throw std::out_of_range{"ordered list"};
        if (n == 0) return first();
        return rest()[n - 1];
    }
    
    template <typename value>
    const value& ordered_list<value>::last() const {
        if (Size == 0) throw std::out_of_range{"ordered list"};
        return last_private();
    }
    
    template <typename value>
    const value& ordered_list<value>::last_private() const {
        if (Size == 1) return first();
        return rest().last_private();
    }
    
    template <typename value>
    inline const value& ordered_list<value>::const_iterator::operator*() const {
        return Ordered.front();
    }
    
    template <typename value>
    inline bool ordered_list<value>::const_iterator::operator==(const const_iterator i) const {
        return Ordered == i.Ordered;
    }
    
    template <typename value>
    inline bool ordered_list<value>::const_iterator::operator!=(const const_iterator i) const {
        return !(*this == i);
    }
    
    template <typename value>
    typename ordered_list<value>::const_iterator& ordered_list<value>::const_iterator::operator++() {
        if (!Ordered.isEmpty()) Ordered = Ordered.popped_front();
        return *this;
    }
    
    template <typename value>
    ordered_list<value>::ordered_list(uint32 size, milewski::okasaki::OrdList<value> ordered) : Size{size}, Ordered{ordered} {}
}

#endif

