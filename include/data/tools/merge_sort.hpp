// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_MERGE_SORT
#define DATA_TOOLS_MERGE_SORT

#include <data/types.hpp>
#include <data/queue/functional_queue.hpp>
#include <data/empty.hpp>
    
namespace data {
    
    namespace sort::merge {
        template <typename X> 
        list::linked<X> merge(list::linked<X> a, list::linked<X> b) {
            list::linked<X> r{};
            while (!a.empty() && !b.empty()) {
                if (a.first() > b.first()) {
                    r = r + a.first();
                    b = b.rest();
                } else {
                    r = r + b.first();
                    b = b.rest();
                }
            }
            
            if (a.empty()) r = r.prepend(b);
            else if (b.empty()) r = r.prepend(a);
            return reverse(r);
        }
        
        template <typename X, typename L>
        list::linked<X> merge(functional_queue<X, L> a, functional_queue<X, L> b) {
            list::linked<X> r{};
            while (!a.empty() && !b.empty()) {
                if (a.first() > b.first()) {
                    r = r + a.first();
                    b = b.rest();
                } else {
                    r = r + b.first();
                    b = b.rest();
                }
            }
            
            if (a.empty()) return r.append(b);
            else if (b.empty()) return r.append(a);
        }
        
        template <typename X>
        void step(X& out, X& in) {
            if (in.size() == 0) return;
            if (in.size() == 1) {
                out = out + in.first();
                in = in.rest();
                return;
            }
            out = out + merge(in.first(), in.rest().first());
            in = in.rest().rest();
        }
        
        template <typename X>
        X round(X in) {
            X out{};
            while (in.size() > 0) step(out, in);
            return out;
        }
    }
    
    struct merge_sort {
        template <typename X>
        list::linked<X> operator()(list::linked<X> x) {
            list::linked<list::linked<X>> z = for_each([](const X x)->list::linked<X>{
                return list::linked<X>{x};
            }, x);
            while (z.size() > 1) z = sort::merge::round(z);
            return z.first();
        }
        
        template <typename X, typename L>
        list::linked<X> operator()(functional_queue<X, L> x) {
            functional_queue<functional_queue<X, L>, L> z = for_each([](const X x)->functional_queue<X, L>{
                return functional_queue<X, L>{x};
            }, x);
            while (z.size() > 1) z = sort::merge::round(z);
            return z.first();
        }
    };

}

#endif
