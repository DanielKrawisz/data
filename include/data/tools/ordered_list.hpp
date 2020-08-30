// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <concepts>
#include <milewski/OrdList/OrdList.hpp>
#include <data/tools/functional_queue.hpp>
#include <data/tools/linked_stack.hpp>
    
namespace data::tool {
    
    template <std::totally_ordered element>
    class ordered_list {
        uint32 Size;
        milewski::okasaki::OrdList<element> Ordered;
    public:
        ordered_list();
        explicit ordered_list(const functional_queue<linked_stack<element>>& l);
        
        bool empty() const;
        size_t size() const;
        
        bool valid() const;
        
        ordered_list insert(const element& x) const;
        ordered_list operator<<(const element& x) const;
        ordered_list operator<<(const linked_stack<element> l) const;
        
        ordered_list rest() const;
        const element& first() const;
        
        const element& operator[](uint32 n) const;
        
        const element& last() const;
        
        template <typename seq>
        bool operator==(const seq& x) const;
        
        template <typename seq>
        bool operator!=(const seq& x) const;
        
        struct const_iterator {
            const_iterator() : Ordered{} {}
            
            const element& operator*() const;
            
            const_iterator& operator++();
            bool operator==(const const_iterator) const;
            bool operator!=(const const_iterator) const;
        private:
            milewski::okasaki::OrdList<element> Ordered;
            const_iterator(milewski::okasaki::OrdList<element> o) : Ordered{o} {}
            
            friend class ordered_list;
        };
        
        const_iterator begin() const {
            return const_iterator{Ordered};
        }
        
        const_iterator end() const {
            return const_iterator{};
        }
        
    private:
        const element& last_private() const;
        
        ordered_list(uint32 size, milewski::okasaki::OrdList<element> ordered);
    };

    template <typename element>
    std::ostream& operator<<(std::ostream& o, const data::tool::ordered_list<element>& l) {
        o << "ordered_list{";
        if (!l.empty()) {
            data::tool::ordered_list<element> x = l;
            while(true) {
                o << x.first();
                x = x.rest();
                if (x.empty()) break;
                o << ", ";
            }
        }
        return o << "}";
    }
    
    template <typename element>
    bool ordered_list<element>::valid() const {
        if (empty()) return true;
        if (!data::valid(first())) return false;
        return rest().valid();
    }
    
    template <typename element>
    inline ordered_list<element>::ordered_list() : Size{0}, Ordered{} {}
        
    template <typename element>
    ordered_list<element>::ordered_list(const functional_queue<linked_stack<element>>& l) : ordered_list{} {
        functional_queue<linked_stack<element>> g = l;
        while(!data::empty(g)) {
            *this << g.first();
            g = g.rest();
        }
    }
        
    template <typename element>
    template <typename seq>
    bool ordered_list<element>::operator==(const seq& x) const {
        if (Size != x.size()) return false;
        if (Size == 0) return true;
        if (first() != x.first()) return false;
        return rest() == x.rest();
    }
    
    template <typename element>
    template <typename seq>
    bool ordered_list<element>::operator!=(const seq& x) const {
        return !operator==(x);
    }
    
    template <typename element>
    bool ordered_list<element>::empty() const {
        return Size == 0;
    }
    
    template <typename element>
    size_t ordered_list<element>::size() const {
        return Size;
    }
    
    template <typename element>
    ordered_list<element> ordered_list<element>::insert(const element& x) const {
        return {Size + 1, Ordered.inserted(x)};
    }
    
    template <typename element>
    ordered_list<element> ordered_list<element>::operator<<(const element& x) const {
        return {Size + 1, Ordered.inserted(x)};
    }
    
    template <typename element>
    ordered_list<element> ordered_list<element>::operator<<(const linked_stack<element> l) const {
        if (l.empty()) return *this;
        return *this << l.first() << l.rest();
    }
    
    template <typename element>
    ordered_list<element> ordered_list<element>::rest() const {
        if (Size == 0) return {};
        return {Size - 1, Ordered.popped_front()};
    }
    
    template <typename element>
    const element& ordered_list<element>::first() const {
        return Ordered.front();
    }
    
    template <typename element>
    const element& ordered_list<element>::operator[](uint32 n) const {
        if (n >= Size) throw std::out_of_range{"ordered list"};
        if (n == 0) return first();
        return rest()[n - 1];
    }
    
    template <typename element>
    const element& ordered_list<element>::last() const {
        if (Size == 0) throw std::out_of_range{"ordered list"};
        return last_private();
    }
    
    template <typename element>
    const element& ordered_list<element>::last_private() const {
        if (Size == 1) return first();
        return rest().last_private();
    }
    
    template <typename element>
    inline const element& ordered_list<element>::const_iterator::operator*() const {
        return Ordered.front();
    }
    
    template <typename element>
    inline bool ordered_list<element>::const_iterator::operator==(const const_iterator i) const {
        return Ordered == i.Ordered;
    }
    
    template <typename element>
    inline bool ordered_list<element>::const_iterator::operator!=(const const_iterator i) const {
        return !(*this == i);
    }
    
    template <typename element>
    typename ordered_list<element>::const_iterator& ordered_list<element>::const_iterator::operator++() {
        if (!Ordered.isEmpty()) Ordered = Ordered.popped_front();
        return *this;
    }
    
    template <typename element>
    ordered_list<element>::ordered_list(uint32 size, milewski::okasaki::OrdList<element> ordered) : Size{size}, Ordered{ordered} {}
}

#endif

