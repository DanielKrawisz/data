// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIST_LINKED
#define DATA_LIST_LINKED

#include <data/list.hpp>
#include <type_traits>
    
namespace data::list {
    template <typename elem> struct linked;
    
    namespace base {
        // base class of a linked list. 
        // I want to support linked lists that can act as std::containers, 
        // meaning that they support iterators as well as linked list
        // that contain references, which makes a lot of sense with 
        // functional programming. 
        template <typename elem, typename derived>
        struct linked {
            using returned = typename container::returned<elem>::type;
            using node = list::node<elem, derived, returned>;
            using next = ptr<node>;
            
            next Next;
        
            // if the list is empty, then this function
            // will dereference a nullptr. It is your
            // responsibility to check. 
            const returned first() const {
                return Next->first();
            }
        
            bool empty() const {
                return Next == nullptr;
            }
            
            const derived rest() const {
                if (empty()) return {};
                
                return Next->rest();
            }
            
            bool valid() const {
                if (empty()) return true;
                return Next->valid();
            }
            
            bool contains(elem x) const {
                if (empty()) return false;
                    
                Next->contains(x);
            }
            
            uint32 size() const {
                if (empty()) return 0;
                    
                return Next->size();
            }
            
            bool operator==(const derived& l) const {
                if (this == &l) return true;
                if (size() != l.size()) return false;
                if (Next == l.Next) return true;
                if (Next == nullptr || l.Next == nullptr) return false;
                return *Next == *l.Next;
            }
            
            bool operator!=(const derived& l) const {
                return !operator==(l);
            }
        
            linked() : Next{nullptr} {}
            linked(linked&& l) : Next{l.Next} {
                l.Next = nullptr;
            }
        
            static derived make() {
                return derived{};
            }
            
            template <typename first>
            static derived make(first x) {
                return derived{}.prepend(x);
            }
            
            template <typename first, typename ... rest>
            static derived make(first x, rest... r) {
                return make(r...).prepend(x);
            }
        
        protected:
            linked(next n) : Next{n} {}
            // ensure the base class can't be constructed. 
            virtual ~linked() = 0; 
            
            friend struct list::linked<elem>;
            
        };
        
        template <typename elem, typename derived> linked<elem, derived>::~linked() {}
    }
    
    template <typename elem>
    struct linked : public base::linked<elem, linked<elem>> {
        using parent = base::linked<elem, linked<elem>>;
        using returned = typename parent::returned;
        using requirement = data::list::definition::complete<
            linked<elem>, elem, returned, iterator<linked>, const iterator<linked>>;
        constexpr static requirement Satisfied{};
        
        linked() : parent{} {}
        linked(const linked& l) : parent{l.Next} {}
        
        linked& operator=(const linked& l) {
            parent::Next = l.Next;
            return *this;
        } 
        
        linked prepend(elem x) const {
            return linked{std::make_shared<typename parent::node>(typename parent::node{x, *this})};
        }
        
        linked operator+(elem x) const {
            return prepend(x);
        }
        
        linked& operator+=(elem x) {
            return operator=(prepend(x));
        }
        
        linked prepend(linked l) const {
            linked x = *this;
            while (!l.empty()) {
                x = x + l.first();
                l = l.rest();
            }
            return x;
        }
        
        linked operator+(linked l) const {
            return prepend(l);
        }
        
        linked from(uint32 n) const {
            if (parent::empty()) return {};
            if (n == 0) return *this;
            return parent::rest().from(n - 1);
        }
        
        returned operator[](uint32 n) const {
            return from(n).first();
        }
        
        iterator<linked> begin();
        iterator<linked> end();
        const iterator<linked> begin() const;
        const iterator<linked> end() const;
        
    private:
        linked(typename parent::next n) : parent{n} {}
    };
    
    template <typename elem>
    struct linked<elem&> : public base::linked<const elem&, linked<elem&>> {
        using parent = base::linked<const elem&, linked<elem&>>;
        using requirement = data::list::definition::buildable<linked<elem&>, elem&, elem&>;
        constexpr static requirement Satisfied{};
        
        linked() : parent{} {}
        linked(const linked& l) : parent{l.Next} {}
        linked(linked&& l) : parent{static_cast<parent&&>(l)} {
            l.Next = nullptr;
        }
        
        linked& operator=(const linked& l) {
            parent::Next = l.Next;
            return *this;
        } 
        
        linked prepend(const elem& x) const {
            return linked{std::make_shared<typename parent::node>(typename parent::node{x, *this})};
        }
        
        linked operator+(const elem& x) const {
            return prepend(x);
        }
        
        linked& operator+=(elem x) {
            return operator=(prepend(x));
        }
        
    private:
        linked(typename parent::next n) : parent{n} {}
    };
}

#endif
