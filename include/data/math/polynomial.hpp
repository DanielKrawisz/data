// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POLYNOMIAL
#define DATA_MATH_POLYNOMIAL

#include <data/tools.hpp>
#include <data/math/division.hpp>
#include <data/math/arithmetic.hpp>

namespace data::math {
    
    template <typename A, typename N, char x = 'x'> struct polynomial;

    template <typename A, typename N, char x>
    std::ostream& operator<<(std::ostream& o, const polynomial<A, N, x>& p);
    
    template <typename A, typename N, char x>
    struct polynomial {
        
        polynomial();
        polynomial(const A a);
        
        constexpr static polynomial zero();
        constexpr static polynomial unit();
        constexpr static polynomial variable();
        
        uint32 degree() const;
        
        static bool equal(const polynomial p, const polynomial q);
        
        bool operator==(const polynomial p) const;
        
        static polynomial plus(const polynomial t, const A a);
        static polynomial plus(const polynomial a, const polynomial b);
        
        polynomial operator+(const A a) const;
        polynomial operator+(const polynomial& p) const;
        
        polynomial operator+=(const A a);
        polynomial operator+=(const polynomial& p);
        
        polynomial operator*(const A a) const;
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
        struct term {
            A Coefficient;
            N Power;
            
            term(A a, N p);
            
            bool operator==(const term& t) const;
            bool operator==(const A& a) const;
            
            A operator()(const A f) const;
            polynomial operator()(const polynomial p) const;
            
            term operator*(const term& t) const;
            term operator*(const A a) const;
            polynomial operator*(const polynomial p) const;
            
            std::weak_ordering operator<=>(const term& o) const;
            
            division<term> operator/(const term& o) const;
        };
        
        polynomial(const term t);
        
        static polynomial plus(const polynomial p, const term t);
        
        polynomial operator+(const term t) const;
        
        polynomial operator+=(const term t);
        
        polynomial operator*(const term t) const;
        
        using terms = ordered_list<term>;
        
        terms Terms;
        
        term first() const;
        
        polynomial rest() const;
        
        friend std::ostream &operator<<<A, N, x>(std::ostream &o, const polynomial &p);
        
        polynomial insert(const term x) const;
        polynomial insert(const terms x) const;
        polynomial insert(const polynomial p) const;
        
        polynomial(const terms l);
        polynomial(const list<term> l);
        
        static polynomial build(const polynomial p);
        
        template <typename ... P>
        static polynomial build(const polynomial p, const A a, P... rest);
        
        template <typename ... P>
        static polynomial build(const polynomial p, const term t, P... rest);
    };

    template <typename A, typename N, char x>
    struct commutative<plus<polynomial<A, N, x>>, polynomial<A, N, x>> : commutative<plus<A>, A> {};
    
    template <typename A, typename N, char x>
    struct associative<plus<polynomial<A, N, x>>, polynomial<A, N, x>> : associative<plus<A>, A> {};
    
    template <typename A, typename N, char x>
    struct commutative<times<polynomial<A, N, x>>, polynomial<A, N, x>> : commutative<times<A>, A>{};
    
    template <typename A, typename N, char x>
    struct associative<times<polynomial<A, N, x>>, polynomial<A, N, x>> : associative<times<A>, A> {};
    
    template <typename A, typename N, char x>
    struct identity<plus<polynomial<A, N, x>>, polynomial<A, N, x>> : identity<plus<A>, A> {
        static const polynomial<A, N, x> value() {
            return identity<plus<A>, A>::value();
        }
    };
    
    template <typename A, typename N, char x>
    struct identity<times<polynomial<A, N, x>>, polynomial<A, N, x>> : identity<times<A>, A> {
        static const polynomial<A, N, x> value() {
            return identity<times<A>, A>::value();
        }
    };

    template <typename A, typename N, char x>
    std::ostream& operator<<(std::ostream& o, const polynomial<A, N, x>& p) {
        o << "polynomial{";
        if (!p.Terms.empty()) {
            typename polynomial<A, N, x>::terms y = p.Terms;
            while(true) {
                typename polynomial<A, N, x>::term t = y.first().Term;
                o << "(" << t.Coefficient << " " << x << "^" << t.Power << ")";
                y = y.rest();
                if (y.empty()) break;
                o << " + ";
            }
        }
        return o << "}";
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::term::operator==(const term& t) const {
        return Coefficient == t.Coefficient && Power == t.Power;
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::term::operator==(const A& a) const {
        return operator==(term{a, 0});
    }
    
    template <typename A, typename N, char x>
    inline A polynomial<A, N, x>::term::operator()(const A f) const{
        return (f ^ Power) * Coefficient;
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::term::operator()(const polynomial p) const {
        return (p ^ Power) * Coefficient;
    }
    
    template <typename A, typename N, char x>
    inline typename polynomial<A, N, x>::term polynomial<A, N, x>::term::operator*(const term& t) const {
        return term{Coefficient * t.Coefficient, Power + t.Power};
    }
    
    template <typename A, typename N, char x>
    inline typename polynomial<A, N, x>::term polynomial<A, N, x>::term::operator*(const A a) const {
        return term{Coefficient * a, Power};
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::term::operator*(const polynomial p) const {
        return polynomial{for_each([this](term o) -> term {
            return this->operator*(o);
        }, p.Terms)};
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::term::term(A a, N p) : Coefficient{a}, Power{p} {}
    
    template <typename A, typename N, char x>
    division<typename polynomial<A, N, x>::term> 
    polynomial<A, N, x>::term::operator/(const term& o) const {
        if (o == 0) throw division_by_zero{};
        if (Power < o.Power) return division<polynomial>{*this, o};
        return division<term>{term{Coefficient / o.Coefficient, Power - o.Power}, term{0}};
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial(const terms l) : Terms{l} {}
    
    template <typename A, typename N, char x>
    polynomial<A, N, x>::polynomial(const list<term> l) : Terms{} {
        list<term> t = l;
        while (!t.empty()) {
            operator+=(t.first());
            t = t.rest();
        }
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial() : Terms{} {}
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial(const A a) : Terms{terms{}.insert(term{a, 0})} {}
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial(const term t) : Terms{terms{}.insert(t)} {}
    
    template <typename A, typename N, char x>
    inline typename polynomial<A, N, x>::term polynomial<A, N, x>::first() const {
        return Terms.first();
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::rest() const {
        return Terms.rest();
    }
    
    template <typename A, typename N, char x>
    inline constexpr polynomial<A, N, x> polynomial<A, N, x>::unit() {
        return polynomial{term{A{1}, N{0}}};
    } 
    
    template <typename A, typename N, char x>
    inline constexpr polynomial<A, N, x> polynomial<A, N, x>::zero() {
        return polynomial{term{A{0}, N{0}}};
    }
    
    template <typename A, typename N, char x>
    inline constexpr polynomial<A, N, x> polynomial<A, N, x>::variable() {
        return polynomial{term{A{1}, N{1}}};
    }
    
    template <typename A, typename N, char x>
    inline uint32 polynomial<A, N, x>::degree() const {
        if (Terms.empty()) return 0;
        return Terms.first().Power;
    }
    
    template <typename A, typename N, char x>
    bool polynomial<A, N, x>::equal(const polynomial p, const polynomial q) {
        if (p.degree() != q.degree()) return false;
        if (p.Terms.empty()) return true;
        if (p.first() != q.first()) return false;
        return equal(p.rest(), q.rest());
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::operator==(const polynomial p) const {
        return equal(*this, p);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::insert(const term t) const {
        if (Terms.empty()) return t;
        if (t == term{A{0}, N{0}}) return *this;
        term first = Terms.first();
        if (first == t) return rest().insert(term{t.Coefficient + first.Coefficient, t.Power});
        return Terms.insert(t);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> 
    polynomial<A, N, x>::insert(terms t) const {
        if (Terms.empty()) return {t};
        if (t.empty()) return *this;
        return insert(t.first()).insert(t.rest());
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::insert(const polynomial p) const {
        if (Terms.empty()) return p;
        return insert(p.Terms);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::plus(const polynomial p, const term t) {
        return p.insert(t);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::plus(const polynomial t, const A a) {
        return plus(t, term{a, 0});
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::plus(const polynomial a, const polynomial b) {
        return a.insert(b.Terms);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator+(const term t) const {
        return plus(*this, t);
    };
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator+(const A a) const {
        return plus(*this, a);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator+(const polynomial& p) const {
        return plus(*this, p);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator+=(const term t) {
        return *this = *this + t;
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator+=(const A a) { 
        return *this = *this + a;
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator+=(const polynomial& p) {
        return *this = *this + p;
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator*(const A a) const {
        return reduce<polynomial>(math::plus<polynomial>{}, 
            for_each([a](term o) -> polynomial{ return o * a; }, Terms));
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator*(const term t) const {
        return reduce<polynomial>(math::plus<polynomial>{}, 
            for_each([t](term o) -> polynomial{ return o * t; }, Terms));
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator*(const polynomial p) const {
        return reduce<polynomial>(math::plus<polynomial>{}, 
            for_each([p](term o) -> polynomial {
                return o * p;
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator^(const N n) const {
        if (n == 0) return unit();
        if (n == 1) return *this;
        return operator*(*this)^(n - 1);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator()(const A f) const {
        if (empty(Terms)) return 0;
        return Terms.first()(f) + polynomial{Terms.rest()}(f);
    }
    
    // inefficient as it computes powers repeatedly. 
    template <typename A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::operator()(const polynomial p) const {
        return reduce<polynomial>(math::plus<polynomial>{}, 
            for_each([p](term o) -> polynomial {
                return o(p);
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::operator==(const polynomial& p) {
        return equal(Terms, p.Terms);
    }
    
    template <typename A, typename N, char x>
    inline division<polynomial<A, N, x>> polynomial<A, N, x>::divide(const polynomial Dividend, const polynomial Divisor) {
        if (Divisor == 0) throw division_by_zero{};
        if (Divisor.degree() > Dividend.degree()) return division<polynomial>{Dividend, Divisor};
        term t = Divisor.first() / Dividend.first();
        return polynomial{t} + divide(Dividend - Divisor * t, Divisor);
    }
    
    template <typename A, typename N, char x>
    inline division<polynomial<A, N, x>> polynomial<A, N, x>::operator/(const polynomial p) const {
        return divide(*this, p);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::derivative() const {
        return reduce(math::plus<polynomial>{}, 
            for_each([](term o)->polynomial{
                if (o.Power == 0) return zero();
                return polynomial{term{o.Coefficient * o.Power, o.Power - 1}};
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    template <typename ... P>
    inline polynomial<A, N, x> polynomial<A, N, x>::make(P... rest) {
        return build(polynomial{}, rest...);
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::build(const polynomial p) {
        return p;
    }
    
    template <typename A, typename N, char x>
    template <typename ... P>
    inline polynomial<A, N, x> polynomial<A, N, x>::build(const polynomial p, const A a, P... rest) {
        return build(p + a, rest...);
    }
    
    template <typename A, typename N, char x>
    template <typename ... P>
    inline polynomial<A, N, x> polynomial<A, N, x>::build(const polynomial p, const term t, P... rest) {
        return build(p + t, rest...);
    }
    
}

#endif
