// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_ORDERED_LIST
#define DATA_TOOLS_ORDERED_LIST

#include <milewski/OrdList/OrdList.hpp>
    
namespace data::tool {
        
    template <typename X>
    struct ordered_list {
        uint32 Size;
        milewski::okasaki::OrdList<X> Ordered;
        
        ordered_list() : Size{0}, Ordered{} {}
        
        bool operator==(const ordered_list x) const {
            if (Size != x.Size) return false;
            if (Size == 0) return true;
            if (first() != x.first()) return false;
            return rest() == x.rest();
        }
        
        bool operator!=(const ordered_list x) const {
            return !operator==(x);
        }
        
        bool empty() const {
            return Size == 0;
        }
        
        size_t size() const {
            return Size;
        }
        
        ordered_list insert(const X& x) const {
            return {Size + 1, Ordered.inserted(x)};
        };
        
        ordered_list operator<<(const X& x) const {
            return {Size + 1, Ordered.inserted(x)};
        };
        
        ordered_list rest() const {
            if (Size == 0) return {};
            return {Size - 1, Ordered.popped_front()};
        };
        
        const X& first() const {
            return Ordered.front();
        }
        
        const X& operator[](uint32 n) const {
            if (n >= Size) throw std::out_of_range{"ordered list"};
            if (n == 0) return first();
            return rest()[n - 1];
        }
        
        const X& last() const {
            if (Size == 0) throw std::out_of_range{"ordered list"};
            return last_private();
        }
        
    private:
        const X& last_private() const {
            if (Size == 1) return first();
            return rest().last_private();
        }
        ordered_list(uint32 size, milewski::okasaki::OrdList<X> ordered) : Size{size}, Ordered{ordered} {}
    };

}

template <typename X>
std::ostream& operator<<(std::ostream& o, const data::tool::ordered_list<X>& l) {
    o << "ordered_list{";
    if (!l.empty()) {
        data::tool::ordered_list<X> x = l;
        while(true) {
            o << x.first();
            x = x.rest();
            if (x.empty()) break;
            o << ", ";
        }
    }
    return o << "}";
}

#endif

