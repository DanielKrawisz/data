// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POLYNOMIAL
#define DATA_MATH_POLYNOMIAL

#include <data/tools.hpp>
#include <data/math/division.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename A, typename N> struct polynomial;

    template <typename A, typename N>
    std::ostream& operator<<(std::ostream& o, const polynomial<A, N>& p);
    
    template <typename A, typename N>
    struct polynomial {
        struct term {
            A Coefficient;
            N Power;
            
            term(A a, N p);
            
            bool operator==(const term& t) const;
            bool operator==(const A& a) const;
            
            bool operator!=(const term& t) const;
            bool operator!=(const A& a) const;
            
            A operator()(const A f) const;
            polynomial operator()(const polynomial p) const;
            
            term operator*(const term& x) const;
            term operator*(const A x) const;
            polynomial operator*(const polynomial p) const;
        };
        
        polynomial();
        polynomial(const A a);
        polynomial(const term t);
        
        constexpr static polynomial unit();
        
        constexpr static polynomial zero();
        
        uint32 degree() const;
        
        static bool equal(const polynomial p, const polynomial q);
        
        bool operator==(const polynomial p) const;
        bool operator!=(const polynomial p) const;
        
        static polynomial plus(const polynomial p, const term x);
        static polynomial plus(const polynomial t, const A a);
        static polynomial plus(const polynomial a, const polynomial b);
        
        polynomial operator+(const term t) const;
        polynomial operator+(const A a) const;
        polynomial operator+(const polynomial& p) const;
        
        polynomial operator+=(const term t);
        polynomial operator+=(const A a);
        polynomial operator+=(const polynomial& p);
        
        polynomial operator*(const A x) const;
        polynomial operator*(const term x) const;
        polynomial operator*(const polynomial p) const;
        
        polynomial operator^(const N n) const;
        
        polynomial operator()(const A f) const;
        polynomial operator()(const polynomial p) const;
        
        bool operator==(const polynomial& p);
        
        bool operator!=(const polynomial& p);
        
        division<polynomial> divide(const polynomial Dividend, const polynomial Divisor);
        
        division<polynomial> operator/(const polynomial p) const;
        
        polynomial derivative() const;
        
        bool operator>(const polynomial p) const;
        bool operator<(const polynomial p) const;
        bool operator<=(const polynomial p) const;
        bool operator>=(const polynomial p) const;
        
        template <typename ... P>
        static polynomial make(P... rest);
        
    private:
        // for ordering terms in the polynomial by power. 
        struct ordering {
            term Term;
            
            ordering(term t);
            
            bool operator==(const ordering& o) const;
            bool operator>(const ordering& o) const;
            bool operator<(const ordering& o) const;
            bool operator<=(const ordering& o) const;
            bool operator>=(const ordering& o) const;
            
            division<ordering> operator/(const ordering& o) const;
        };
        
        using terms = ordered_list<ordering>;
        
        terms Terms;
        
        term first() const;
        
        polynomial rest() const;
        
        polynomial(const terms l);
        polynomial(const list<term> l);
        
        friend std::ostream& operator<<<A, N>(std::ostream& o, const polynomial& p);
        
        polynomial insert(const term x) const;
        polynomial insert(const terms x) const;
        polynomial insert(const polynomial p) const;
        
        static polynomial build(const polynomial p);
        
        template <typename ... P>
        static polynomial build(const polynomial p, const A x, P... rest);
        
        template <typename ... P>
        static polynomial build(const polynomial p, const term x, P... rest);
    };

    template <typename A, typename N>
    struct commutative<data::plus<polynomial<A, N>>, polynomial<A, N>> : commutative<data::plus<A>, A> {};
    
    template <typename A, typename N>
    struct associative<data::plus<polynomial<A, N>>, polynomial<A, N>> : associative<data::plus<A>, A> {};
    
    template <typename A, typename N>
    struct commutative<data::times<polynomial<A, N>>, polynomial<A, N>> : commutative<data::times<A>, A>{};
    
    template <typename A, typename N>
    struct associative<data::times<polynomial<A, N>>, polynomial<A, N>> : associative<data::times<A>, A> {};
    
    template <typename A, typename N>
    struct identity<data::plus<polynomial<A, N>>, polynomial<A, N>> : identity<data::plus<A>, A> {
        static const polynomial<A, N> value() {
            return identity<data::plus<A>, A>::value();
        }
    };
    
    template <typename A, typename N>
    struct identity<data::times<polynomial<A, N>>, polynomial<A, N>> : identity<data::times<A>, A> {
        static const polynomial<A, N> value() {
            return identity<data::times<A>, A>::value();
        }
    };

    template <typename A, typename N>
    std::ostream& operator<<(std::ostream& o, const polynomial<A, N>& p) {
        o << "polynomial{";
        if (!p.Terms.empty()) {
            typename polynomial<A, N>::terms x = p.Terms;
            while(true) {
                typename polynomial<A, N>::term t = x.first().Term;
                o << "(" << t.Coefficient << " x^" << t.Power << ")";
                x = x.rest();
                if (x.empty()) break;
                o << " + ";
            }
        }
        return o << "}";
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::term::operator==(const term& t) const {
        return Coefficient == t.Coefficient && Power == t.Power;
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::term::operator==(const A& a) const {
        return operator==(term{a, 0});
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::term::operator!=(const term& t) const {
        return !operator==(t);
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::term::operator!=(const A& a) const {
        return !operator==(term{a, 0});
    }
    
    template <typename A, typename N>
    inline A polynomial<A, N>::term::operator()(const A f) const{
        return (f ^ Power) * Coefficient;
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::term::operator()(const polynomial p) const {
        return (p ^ Power) * Coefficient;
    }
    
    template <typename A, typename N>
    inline typename polynomial<A, N>::term polynomial<A, N>::term::operator*(const term& x) const {
        return term{Coefficient * x.Coefficient, Power + x.Power};
    }
    
    template <typename A, typename N>
    inline typename polynomial<A, N>::term polynomial<A, N>::term::operator*(const A x) const {
        return term{Coefficient * x, Power};
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::term::operator*(const polynomial p) const {
        return polynomial{for_each([this](ordering o) -> term {
            return this->operator*(o.Term);
        }, p.Terms)};
    }
    
    template <typename A, typename N>
    inline polynomial<A, N>::term::term(A a, N p) : Coefficient{a}, Power{p} {}
    
    template <typename A, typename N>
    inline polynomial<A, N>::ordering::ordering(term t) : Term{t} {}
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::ordering::operator==(const ordering& o) const {
        return Term.Power == o.Term.Power;
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::ordering::operator>(const ordering& o) const {
        return Term.Power > o.Term.Power;
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::ordering::operator<(const ordering& o) const {
        return Term.Power < o.Term.Power;
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::ordering::operator<=(const polynomial::ordering& o) const {
        return Term.Power <= o.Term.Power;
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::ordering::operator>=(const polynomial::ordering& o) const {
        return Term.Power >= o.Term.Power;
    }
    
    template <typename A, typename N>
    division<typename polynomial<A, N>::ordering> 
    polynomial<A, N>::ordering::operator/(const ordering& o) const {
        if (o.Term == 0) throw division_by_zero{};
        if (Term.Power < o.Term.Power) return division<polynomial>{*this, o};
        return division<ordering>{
            ordering{term{
                Term.Coefficient / o.Term.Coefficient, 
                Term.Power - o.Term.Power}}, 
            ordering{0}};
    }
    
    template <typename A, typename N>
    inline polynomial<A, N>::polynomial(const terms l) : Terms{l} {}
    
    template <typename A, typename N>
    polynomial<A, N>::polynomial(const list<term> l) : Terms{} {
        list<term> t = l;
        while (!t.empty()) {
            operator+=(t.first());
            t = t.rest();
        }
    }
    
    template <typename A, typename N>
    inline polynomial<A, N>::polynomial() : Terms{} {}
    
    template <typename A, typename N>
    inline polynomial<A, N>::polynomial(const A a) : Terms{terms{}.insert(ordering{term{a, 0}})} {}
    
    template <typename A, typename N>
    inline polynomial<A, N>::polynomial(const term t) : Terms{terms{}.insert(ordering{t})} {}
    
    template <typename A, typename N>
    inline typename polynomial<A, N>::term polynomial<A, N>::first() const {
        return Terms.first().Term;
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::rest() const {
        return Terms.rest();
    }
    
    template <typename A, typename N>
    inline constexpr polynomial<A, N> polynomial<A, N>::unit() {
        return polynomial{term{A{1}, N{0}}};
    } 
    
    template <typename A, typename N>
    inline constexpr polynomial<A, N> polynomial<A, N>::zero() {
        return polynomial{term{A{0}, N{0}}};
    }
    
    template <typename A, typename N>
    inline uint32 polynomial<A, N>::degree() const {
        if (Terms.empty()) return 0;
        return Terms.first().Term.Power;
    }
    
    template <typename A, typename N>
    bool polynomial<A, N>::equal(const polynomial p, const polynomial q) {
        if (p.degree() != q.degree()) return false;
        if (p.Terms.empty()) return true;
        if (p.first() != q.first()) return false;
        return equal(p.rest(), q.rest());
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::operator==(const polynomial p) const {
        return equal(*this, p);
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::operator!=(const polynomial p) const {
        return !equal(*this, p);
    }
    
    template <typename A, typename N>
    polynomial<A, N> polynomial<A, N>::insert(const term x) const {
        if (Terms.empty()) return x;
        if (x == term{A{0}, N{0}}) return *this;
        ordering first = Terms.first();
        if (first == ordering{x}) 
            return rest().insert(term{x.Coefficient + first.Term.Coefficient, x.Power});
        return Terms.insert(ordering{x});
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> 
    polynomial<A, N>::insert(terms t) const {
        if (Terms.empty()) return {t};
        if (t.empty()) return *this;
        return insert(t.first().Term).insert(t.rest());
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::insert(const polynomial p) const {
        if (Terms.empty()) return p;
        return insert(p.Terms);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::plus(const polynomial p, const term x) {
        return p.insert(x);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::plus(const polynomial t, const A a) {
        return plus(t, term{a, 0});
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::plus(const polynomial a, const polynomial b) {
        return a.insert(b.Terms);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator+(const term t) const {
        return plus(*this, t);
    };
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator+(const A a) const {
        return plus(*this, a);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator+(const polynomial& p) const {
        return plus(*this, p);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator+=(const term t) {
        return *this = *this + t;
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator+=(const A a) {
        return *this = *this + a;
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator+=(const polynomial& p) {
        return *this = *this + p;
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator*(const A x) const {
        return reduce<polynomial>(data::plus<polynomial>{}, 
            for_each([x](ordering o) -> polynomial{ return o.Term * x; }, Terms));
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator*(const term x) const {
        return reduce<polynomial>(data::plus<polynomial>{}, 
            for_each([x](ordering o) -> polynomial{ return o.Term * x; }, Terms));
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator*(const polynomial p) const {
        return reduce<polynomial>(data::plus<polynomial>{}, 
            for_each([p](ordering o) -> polynomial {
                return o.Term * p;
            }, Terms));
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator^(const N n) const {
        if (n == 0) return unit();
        if (n == 1) return *this;
        return operator*(*this)^(n - 1);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::operator()(const A f) const {
        if (empty(Terms)) return 0;
        return Terms.first()(f) + polynomial{Terms.rest()}(f);
    }
    
    // inefficient as it computes powers repeatedly. 
    template <typename A, typename N>
    polynomial<A, N> polynomial<A, N>::operator()(const polynomial p) const {
        return reduce<polynomial>(data::plus<polynomial>{}, 
            for_each([p](ordering o) -> polynomial {
                return o.Term(p);
            }, Terms));
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::operator==(const polynomial& p) {
        return equal(Terms, p.Terms);
    }
    
    template <typename A, typename N>
    inline bool polynomial<A, N>::operator!=(const polynomial& p) {
        return !operator==(p);
    }
    
    template <typename A, typename N>
    inline division<polynomial<A, N>> polynomial<A, N>::divide(const polynomial Dividend, const polynomial Divisor) {
        if (Divisor == 0) throw division_by_zero{};
        if (Divisor.degree() > Dividend.degree()) return division<polynomial>{Dividend, Divisor};
        term x = Divisor.first() / Dividend.first();
        return polynomial{x} + divide(Dividend - Divisor * x, Divisor);
    }
    
    template <typename A, typename N>
    inline division<polynomial<A, N>> polynomial<A, N>::operator/(const polynomial p) const {
        return divide(*this, p);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::derivative() const {
        return reduce(data::plus<polynomial>{}, 
            for_each([](ordering o)->polynomial{
                if (o.Term.Power == 0) return zero();
                return polynomial{term{o.Term.Coefficient * o.Term.Power, o.Term.Power - 1}};
            }, Terms));
    }
    
    template <typename A, typename N>
    template <typename ... P>
    inline polynomial<A, N> polynomial<A, N>::make(P... rest) {
        return build(polynomial{}, rest...);
    }
    
    template <typename A, typename N>
    inline polynomial<A, N> polynomial<A, N>::build(const polynomial p) {
        return p;
    }
    
    template <typename A, typename N>
    template <typename ... P>
    inline polynomial<A, N> polynomial<A, N>::build(const polynomial p, const A x, P... rest) {
        return build(p + x, rest...);
    }
    
    template <typename A, typename N>
    template <typename ... P>
    inline polynomial<A, N> polynomial<A, N>::build(const polynomial p, const term x, P... rest) {
        return build(p + x, rest...);
    }
    
}

#endif
