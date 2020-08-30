// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PRIORITY_QUEUE
#define DATA_TOOLS_PRIORITY_QUEUE

#include <milewski/Leftist/LeftistHeap.hpp>
#include <data/tools/linked_stack.hpp>
    
namespace data::tool {
    
    template <typename x, typename stack = linked_stack<x>> requires sequence<stack, x>
    class priority_queue {
        using heap = milewski::okasaki::Heap<x>;
        heap Heap;
        uint32 Size;
        priority_queue(heap h, uint32 size) : Heap{h}, Size{size} {}
    public:
        size_t size() const {
            return Size;
        }
        
        bool empty() const {
            return Size == 0;
        }
        
        priority_queue() : Heap{}, Size{0} {}
        //priority_queue(std::initializer_list<x> init) : Heap{init}, Size{init.size()} {}
        
        const x& first() const {
            return Heap.front();
        }
        
        priority_queue rest() const {
            return {Heap.popped_front(), Size - 1};
        }
        
        priority_queue insert(x elem) const {
            return {Heap.inserted(elem), Size + 1};
        }
        
        template <typename List> requires sequence<List, x>
        priority_queue insert(List l) const {
            if (l.empty()) return *this;
            return insert(l.first()).insert(l.rest());
        }
        
        template <typename List> requires sequence<List, x> 
        priority_queue(List l) : priority_queue{priority_queue{}.insert(l)} {}
        
        stack values() const {
            stack vals;
            auto p = *this;
            while (!p.empty()) {
                vals = vals << p.first();
                p = p.rest();
            }
            return reverse(vals);
        }
        
        priority_queue& operator=(const priority_queue& q) {
            Heap = q.Heap;
            Size = q.Size;
            return *this;
        }
        
    };
    
}

#endif
