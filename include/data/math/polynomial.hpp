// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POLYNOMIAL
#define DATA_MATH_POLYNOMIAL

#include <data/tools/ordered_list.hpp>
#include <data/for_each.hpp>
#include <data/fold.hpp>
#include <data/math/division.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename A, typename N>
    struct polynomial {
        struct term {
            A Coefficient;
            N Power;
            
            bool operator==(const term& t) const {
                return Coefficient == t.Coefficient && Power == t.Power;
            }
            
            bool operator==(const A& a) const {
                return operator==(term{a, 0});
            }
            
            bool operator!=(const term& t) const {
                return !operator==(t);
            }
            
            bool operator!=(const A& a) const {
                return !operator==(term{a, 0});
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
        
    private:
        // for ordering terms in the polynomial by power. 
        struct ordering {
            term Term;
            
            ordering(term t) : Term{t} {}
            
            bool operator==(const ordering& o) {
                return Term.Power == o.Term.Power;
            }
            
            bool operator>(const ordering& o) {
                return Term.Power > o.Term.Power;
            }
            
            bool operator<(const ordering& o) {
                return Term.Power < o.Term.Power;
            }
            
            bool operator<=(const ordering& o) {
                return Term.Power <= o.Term.Power;
            }
            
            bool operator>=(const ordering& o) {
                return Term.Power >= o.Term.Power;
            }
            
            division<ordering> operator/(const ordering& o) {
                if (o.Term == 0) throw division_by_zero{};
                if (Term.Power < o.Term.Power) return division<polynomial>{*this, o};
                return division<ordering>{
                    ordering{term{
                        Term.Coefficient / o.Term.Coefficient, 
                        Term.Power - o.Term.Power}}, 
                    ordering{0}};
            }
        };
        
        using terms = tool::ordered_list<ordering>;
        
        terms Terms;
        
        polynomial(const terms l) : Terms{l} {}
        
    public:
        polynomial() : Terms{} {}
        polynomial(const A a) : Terms{terms{}.insert(ordering{term{a, 0}})} {}
        polynomial(const term t) : Terms{terms{}.insert(ordering{t})} {}
        
        constexpr static polynomial unit() {
            return polynomial{term{1, 0}};
        } 
        
        constexpr static polynomial zero() {
            return polynomial{term{0, 0}};
        }
        
        term first() const {
            if (Terms.empty()) return term{0, 0};
            return Terms.first().Term;
        }
        
        polynomial rest() const {
            return Terms.rest();
        }
        
        uint32 degree() const {
            if (Terms.empty()) return 0;
            return Terms.first().Term.Power;
        }
        
        static bool equal(const polynomial p, const polynomial q) {
            if (p.degree() != q.degree()) return false;
            if (p.Terms.empty()) return true;
            if (p.first() != q.first()) return false;
            return equal(p.rest(), q.rest());
        }
        
        bool operator==(const polynomial p) const {
            return equal(*this, p);
        }
        
        bool operator!=(const polynomial p) const {
            return !equal(*this, p);
        }
        
    private:
        polynomial insert(const term x) const {
            if (Terms.empty()) return x;
            if (x == term{0, 0}) return *this;
            ordering first = Terms.first();
            if (first == ordering{x}) 
                return rest().insert(term{x.Coefficient + first.Term.Coefficient, x.Power});
            return Terms.insert(ordering{x});
        }
        
        functional::stack::linked<term> get_terms() const {
            if (Terms.empty()) return {};
            return rest().get_terms() + first();
        }
        
        polynomial insert(functional::stack::linked<term> t) const {
            if (t.empty()) return *this;
            return insert(t.first()).insert(t.rest());
        }
        
        polynomial insert(const polynomial p) const {
            if (Terms.empty()) return *this;
            return insert(functional::stack::reverse(p.get_terms()));
        }
        
    public:
        static polynomial plus(const polynomial p, const term x) {
            return p.insert(x);
        }
        
        static polynomial plus(const polynomial t, const A a) {
            return plus(t, term{a, 0});
        }
        
        static polynomial plus(const polynomial a, const polynomial b) {
            return a.insert(b);
        }
        
        polynomial operator+(const term t) const {
            return plus(*this, t);
        };
        
        polynomial operator+(const A a) const {
            return plus(*this, a);
        }
        
        polynomial operator+(const polynomial& p) const {
            return plus(*this, p);
        }
        
        polynomial operator*(const A x) const {
            return reduce<polynomial>(data::plus<polynomial>{}, 
                for_each([x](ordering o)->polynomial{return o.Term * x;}, Terms));
        }
        
        polynomial operator*(const term x) const {
            return reduce<polynomial>(data::plus<polynomial>{}, 
                for_each([x](ordering o)->polynomial{return o.Term * x;}, Terms));
        }
        
        polynomial operator*(const polynomial p) const {
            return reduce<polynomial>(data::plus<polynomial>{}, 
                for_each([p](ordering o)->polynomial{
                    return p * o.Term;
                }, Terms));
        }
        
        polynomial operator^(const N n) const {
            if (n == 0) return unit();
            if (n == 1) return *this;
            return operator*(*this)^(n - 1);
        }
        
        polynomial operator()(const A f) const {
            if (empty(Terms)) return 0;
            return Terms.first()(f) + polynomial{Terms.rest()}(f);
        }
        
        // inefficient as it computes powers repeatedly. 
        polynomial operator()(const polynomial p) const {
            return reduce<polynomial>(data::plus<polynomial>{}, 
                for_each([p](ordering o)->polynomial{
                    return o.Term(p);
                }, Terms));
        }
        
        bool operator==(const polynomial& p) {
            return equal(Terms, p.Terms);
        }
        
        bool operator!=(const polynomial& p) {
            return !operator==(p);
        }
        
        division<polynomial> divide(const polynomial Dividend, const polynomial Divisor) {
            if (Divisor == 0) throw division_by_zero{};
            if (Divisor.degree() > Dividend.degree()) return division<polynomial>{Dividend, Divisor};
            term x = Divisor.first() / Dividend.first();
            return polynomial{x} + divide(Dividend - Divisor * x, Divisor);
        }
        
        division<polynomial> operator/(const polynomial p) const {
            return divide(*this, p);
        }
        
        polynomial derivative() const {
            return reduce(data::plus<polynomial>{}, 
                for_each([](ordering o)->polynomial{
                    if (o.Term.Power == 0) return zero();
                    return polynomial{term{o.Term.Coefficient * o.Term.Power, o.Term.Power - 1}};
                }, Terms));
        }
        
        template <typename ... P>
        static polynomial make(P... rest) {
            return build(polynomial{}, rest...);
        }
        
    private: 
        static polynomial build(const polynomial p) {
            return p;
        }
        
        template <typename ... P>
        static polynomial build(const polynomial p, const A x, P... rest) {
            return build(p + x, rest...);
        }
        
        template <typename ... P>
        static polynomial build(const polynomial p, const term x, P... rest) {
            return build(p + x, rest...);
        }
    };

}

#endif
