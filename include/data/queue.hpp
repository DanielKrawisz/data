// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_QUEUE
#define DATA_QUEUE

#include <data/list.hpp>

namespace data {
        
    namespace queue {

        namespace definition {
                
            template <typename Q, typename X>
            struct queue : public list::definition::list<Q, X> {
                Q make_empty() const {
                    return {};
                }
                
                Q append(const Q l, const X x) const {
                    return l + x;
                }
                
                Q append(const Q l, const Q q) const {
                    return l + q;
                }
                
                X last(const Q l) const {
                    return l.last();
                }
            };

            template <typename Q, typename X>
            struct queue<Q*, X> : public list::definition::list<Q*, X> {
                Q append(const Q l, const X x) const {
                    if (l == nullptr) return x;
                    return l->append(x);
                }
                
                Q append(const Q l, const Q q) const {
                    if (l == nullptr) return q;
                    return l->append(q);
                }
                
                X last(const Q l) const {
                    return l->last();
                }
            };
            
            template <typename Q, typename X>
            struct queue<ptr<Q>, X> : public list::definition::list<ptr<Q>, X>, public queue<Q*, X> {};
            
            template <typename Q, typename X, typename it, typename const_it>
            struct iterable : public queue<Q, X>, public container::iterable<Q, it, const_it> {};
            
        }
            
        template <typename Q>
        struct is_queue {
            using element = typename std::remove_reference<typename std::invoke_result<decltype(&Q::first), Q>::type>::type;
            constexpr static definition::queue<Q, element> IsQueue{};
        };
        
        template <typename Q>
        struct is_queue<Q*> {
            using element = typename std::remove_reference<typename std::invoke_result<decltype(&Q::first), Q>::type>::type;
            constexpr static definition::queue<Q, element> IsQueue{};
        };
        
        template <typename Q>
        struct is_queue<ptr<Q>> : public is_queue<Q*> {
            using element = typename is_queue<Q*>::Element;
        };
            
        template <typename Q, typename X> 
        inline Q append(Q q, X x) {
            return definition::queue<Q, X>{}.append(q, x);
        }
            
        template <typename Q> 
        inline bool empty(Q q) {
            return list::empty(q);
        }
            
        template <typename Q> 
        inline bool size(Q q) {
            return list::size(q);
        }
            
        template <typename Q, typename X> 
        inline X& first(Q q) {
            return list::first(q);
        }
            
        template <typename Q, typename X> 
        inline Q rest(Q q) {
            return list::rest(q);
        }
        
        template <typename Q, typename X> 
        inline Q prepend(Q q, X value) {
            return list::rest(q, value);
        }
    
    }

}

#endif

