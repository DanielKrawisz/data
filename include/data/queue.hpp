#ifndef DATA_QUEUE_HPP
#define DATA_QUEUE_HPP

#include <data/list.hpp>

namespace data {
        
    namespace queue {

        namespace definition {
                
            template <typename Q, typename X>
            struct queue : public list::definition::list<Q, X> {                
                Q append(const Q l, const X x) const {
                    return l + x;
                }
            };
                
            template <typename Q, typename X>
            struct queue<Q*, X> : public list::definition::list<Q*, X> {
                Q append(const Q l, const X x) const {
                    if (l == nullptr) return x;
                    return l->append(x);
                }
            };
                
            template <typename Q, typename X>
            struct queue<ptr<Q>, X> : public list::definition::list<ptr<Q>, X>, public queue<Q*, X> {};
            
        }
            
        template <typename Q>
        struct is_queue {
            using Element = std::remove_reference<std::__invoke_result<typename Q::first>>;
            constexpr static definition::queue<Q, Element> IsQueue{};
        };
        
        template <typename Q>
        struct is_queue<Q*> {
            using Element = std::remove_reference<std::__invoke_result<typename std::remove_pointer<Q>::first>>;
            constexpr static definition::queue<Q, Element> IsQueue{};
        };
        
        template <typename Q>
        struct is_queue<ptr<Q>> : public is_queue<Q*> {
            using Element = typename is_queue<Q*>::Element;
        };
            
        template <typename Q, typename X> 
        inline Q append(Q q, X x) {
            return definition::queue<Q, X>{}.append(q, x);
        }
    
    }

}

#endif

