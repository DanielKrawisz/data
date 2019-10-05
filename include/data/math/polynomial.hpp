// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POLYNOMIAL
#define DATA_MATH_POLYNOMIAL

#include <data/queue/functional_queue.hpp>
#include <data/list/linked.hpp>
#include <data/for_each.hpp>
#include <data/fold.hpp>
#include <data/plus.hpp>

namespace data::math {
    template <typename A, typename N>
    struct polynomial {
        struct term {
            A Coefficient;
            N Power;
            
            bool operator==(const term& t) const {
                return Coefficient == t.Coefficient && Power == t.Power;
            }
            
            A operator()(const A f) const{
                return (f ^ Power) * Coefficient;
            }
            
            polynomial operator()(const polynomial p) const {
                return (p ^ Power) * Coefficient;
            }
            
            term operator*(const term& x) const {
                return term{Coefficient * x.Coefficient, Power + x.Power};
            }
            
            term operator*(const A x) const {
                return term{Coefficient * x, Power};
            }
            
            term(A a, N p) : Coefficient{a}, Power{p} {}
        };
        
        using queue = functional_queue<term, list::linked<term>>;
        queue Terms;
        
        polynomial() : Terms{} {}
        polynomial(const A a) : polynomial{{term{a, 0}}} {}
        polynomial(const term t) : Terms{{t}} {}
        polynomial(const queue q) : Terms{q} {}
        
        bool operator==(const polynomial& p) {
            return Terms == p.Terms;
        }
        
        bool operator!=(const polynomial& p) {
            return !operator==(p);
        }
        
        polynomial operator+(const A a) const {
            return operator+(term{a, 0});
        }
        
        polynomial operator+(const term t) const {
            return polynomial{plus(Terms, t)};
        };
        
        static polynomial add(const polynomial a, const polynomial b) {
            return polynomial{merge(a.Terms, b.Terms)};
        }
        
        polynomial operator+(const polynomial p) const {
            return add(*this, p);
        }
        
        polynomial operator*(const A x) const {
            return polynomial{for_each([x](term t)->term{return t * x;}, Terms)};
        }
        
        polynomial operator*(const term x) const {
            return polynomial{for_each([x](term t)->term{return t * x;}, Terms)};
        }
        
        polynomial operator*(const polynomial p) const {
            return reduce(data::plus<polynomial>{}, 
                for_each([p](term t)->polynomial{
                    return p * t;
                }, Terms));
        }
        
        polynomial operator^(N n) const {
            if (n == 0) return 1;
            if (n == 1) return *this;
            return operator*(*this)^(n - 1);
        }
        
        polynomial operator()(A f) const {
            if (empty(Terms)) return 0;
            return Terms.first()(f) + polynomial{Terms.rest()}(f);
        }
        
        // inefficient as it computes powers repeatedly. 
        polynomial operator()(const polynomial p) const {
            return reduce(data::plus<polynomial>{}, 
                for_each([p](term t)->polynomial{
                    return t(p);
                }, Terms));
        }
        
        template <typename ... P>
        static polynomial make(P... rest) {
            return build(polynomial{}, rest...);
        }
        
    private:
        static queue plus(queue l, term t) {
            if (empty(l)) return queue{{t}};
            if (t.Coefficient == 0) return l;
            term f = l.first();
            if (t.Power == f.Power) return append(l.rest(), term{t.Coefficient + f.Coefficient, t.Power});
            return append(plus(l, t), f);
        }
        
        static queue merge(queue a, queue b) {
            queue r{}; 
            while (!a.empty() && !b.empty()) {
                term ta = a.first();
                term tb = b.first();
                if (ta.Power < tb.Power) {
                    r = r + ta;
                    a = a.rest();
                } else if (ta.Power > tb.Power) {
                    r = r + tb;
                    b = b.rest();
                } else {
                    r = r + term{ta.Coefficient + tb.Coefficient, ta.Power};
                    a = a.rest();
                    b = b.rest();
                }
            }
            if (!a.empty()) r = r.append(a);
            if (!b.empty()) r = r.append(b);
            return r;
        }
        
        static polynomial build(polynomial p) {
            return p;
        }
        
        template <typename ... P>
        static polynomial build(polynomial p, const A x, P... rest) {
            build(p + x, rest...);
        }
        
        template <typename ... P>
        static polynomial build(polynomial p, const term x, P... rest) {
            build(p + x, rest...);
        }
    };

}

#endif

