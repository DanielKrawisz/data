// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_PRIORITY_QUEUE
#define DATA_TOOLS_PRIORITY_QUEUE

#include <milewski/Leftist/LeftistHeap.hpp>
#include <data/list/linked.hpp>
    
namespace data {
    namespace priority {

        template <typename x>
        class queue {
            using heap = milewski::okasaki::Heap<x>;
            heap Heap;
            uint32 Size;
            queue(heap h, uint32 size) : Heap{h}, Size{size} {}
        public:
            size_t size() const {
                return Size;
            }
            
            bool empty() const {
                return Size == 0;
            }
            
            queue() : Heap{}, Size{0} {}
            queue(std::initializer_list<x> init) : Heap{init}, Size{init.size()} {}
            queue(const queue& q) : Heap{q.Heap}, Size{q.Size} {}
                
            const x& first() const {
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
                static interface::sequence<List> is_list{};
                if (l.empty()) return *this;
                return insert(l.first()).insert(l.rest());
            }
            
            template <typename List>
            queue(List l) : queue{queue{}.insert(l)} {}
            
            list::linked<x> all() const;
            
            queue& operator=(const queue& q) {
                Heap = q.Heap;
                Size = q.Size;
                return *this;
            }
        };
    
    }

}

#endif
