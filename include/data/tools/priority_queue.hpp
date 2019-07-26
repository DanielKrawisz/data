// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PRIORITY_QUEUE
#define DATA_TOOLS_PRIORITY_QUEUE

#include <milewski/Leftist/LeftistHeap.hpp>
#include <data/list/linked_list.hpp>
    
namespace data {
    namespace priority {

        template <typename x>
        class queue {
            using heap = milewski::okasaki::Heap<x>;
            heap Heap;
            uint32 Size;
            queue(heap h, uint32 size) : Heap{h}, Size{size} {}
        public:
            uint32 size() const {
                return Size;
            }
            
            bool empty() const {
                return Size == 0;
            }
            
            queue() : Heap{}, Size{0} {}
            queue(std::initializer_list<x> init) : Heap{init}, Size{init.size()} {}
            queue(const queue& q) : Heap{q.Heap}, Size{q.Size} {}
                
            x first() const {
                return Heap.front();
            }

            queue rest() const {
                return {Heap.popped_front(), Size - 1};
            }
            
            queue insert(x elem) const {
                return {Heap.inserted(elem), Size + 1};
            }
            
            template <typename List>
            queue insert(List l) const {
                static list::definition::list<List, x> is_list{};
                if (l.empty()) return *this;
                return insert(l.first()).insert(l.rest());
            }
            
            template <typename List>
            queue(List l) : queue{queue{}.insert(l)} {}
            
            linked_list<x> all() const;
            
            queue& operator=(const queue& q) {
                Heap = q.Heap;
                Size = q.Size;
            }
            
            static list::definition::list<queue, x> is_list{};
        };
        
        template <typename List, typename x> 
        List all(const queue<x> q) {
            static list::definition::list<List, x> is_list{};
            List l{};
            for (queue<x> i = q; !i.empty(); i = i.rest()) l = l + i.first();
            return l;
        }
        
        template <typename x>
        linked_list<x> queue<x>::all() const {
            return priority::all<linked_list<x>, x>(*this);
        }
    
    }
    
    template <typename List> 
    List sort(List x) {
        priority::all<List, list::is_list<List>::element>(priority::queue<typename list::is_list<List>::element>{x});
    }

}

#endif
