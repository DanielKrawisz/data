// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POLYNOMIAL
#define DATA_MATH_POLYNOMIAL

#include <data/tools.hpp>
#include <data/math/division.hpp>
#include <data/math/arithmetic.hpp>
#include <data/math/power.hpp>

namespace data::math {

    template <typename A, typename N, char x> struct polynomial;

    template <typename A, typename N, char x>
    std::ostream &operator << (std::ostream &o, const polynomial<A, N, x> &p);

    template <char name> struct variable {};

    template <char name>
    std::ostream inline &operator << (std::ostream &o, const variable<name> &x) {
        return o << name;
    }
    
    template <typename A, typename N, char x>
    struct polynomial {
        struct term {
            A Coefficient;
            power<variable<x>, N> Power;
            
            term (A, N);
            
            bool operator == (const term &) const;
            bool operator == (const A &) const;
            
            bool operator != (const term &) const;
            bool operator != (const A &) const;
            
            A operator () (const A) const;
            polynomial operator () (const polynomial) const;
            
            term operator * (const term &) const;
            term operator * (const A) const;
            polynomial operator * (const polynomial) const;
        };
        
        polynomial ();
        polynomial (const A a);
        polynomial (const term t);
        
        constexpr static polynomial unit ();
        
        constexpr static polynomial zero ();
        
        uint32 degree () const;
        
        static bool equal (const polynomial, const polynomial);
        
        bool operator == (const polynomial) const;
        bool operator != (const polynomial) const;
        
        static polynomial plus (const polynomial, const term);
        static polynomial plus (const polynomial, const A);
        static polynomial plus (const polynomial, const polynomial);
        
        polynomial operator + (const term) const;
        polynomial operator + (const A) const;
        polynomial operator + (const polynomial &) const;
        
        polynomial operator += (const term);
        polynomial operator += (const A);
        polynomial operator += (const polynomial &);
        
        polynomial operator * (const A) const;
        polynomial operator * (const term) const;
        polynomial operator * (const polynomial) const;
        
        polynomial operator ^ (const N) const;
        
        polynomial operator () (const A) const;
        polynomial operator () (const polynomial) const;
        
        bool operator == (const polynomial &);
        
        bool operator != (const polynomial &);
        
        division<polynomial> divide (const polynomial Dividend, const polynomial Divisor);
        
        division<polynomial> operator / (const polynomial) const;
        
        polynomial derivative () const;
        
        bool operator > (const polynomial) const;
        bool operator < (const polynomial) const;
        bool operator <= (const polynomial) const;
        bool operator >= (const polynomial) const;
        
        template <typename ... P>
        static polynomial make (P... rest);
        
    private:
        // for ordering terms in the polynomial by power. 
        struct ordering {
            term Term;
            
            ordering (term t);
            
            bool operator == (const ordering &) const;
            bool operator > (const ordering &) const;
            bool operator < (const ordering &) const;
            bool operator <= (const ordering &) const;
            bool operator >= (const ordering &) const;
            
            division<ordering> operator / (const ordering &) const;
        };
        
        using terms = ordered_list<ordering>;
        
        terms Terms;
        
        term first () const;
        
        polynomial rest () const;
        
        polynomial (const terms l);
        polynomial (const list<term> l);
        
        friend std::ostream &operator << <A, N> (std::ostream &, const polynomial &);
        
        polynomial insert (const term) const;
        polynomial insert (const terms) const;
        polynomial insert (const polynomial p) const;
        
        static polynomial build (const polynomial p);
        
        template <typename ... P>
        static polynomial build (const polynomial p, const A, P... rest);
        
        template <typename ... P>
        static polynomial build (const polynomial p, const term, P... rest);
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
        static const polynomial<A, N, x> value () {
            return identity<plus<A>, A>::value ();
        }
    };
    
    template <typename A, typename N, char x>
    struct identity<times<polynomial<A, N, x>>, polynomial<A, N, x>> : identity<times<A>, A> {
        static const polynomial<A, N, x> value () {
            return identity<times<A>, A>::value ();
        }
    };

    template <typename A, typename N, char x>
    std::ostream &operator << (std::ostream &o, const polynomial<A, N, x> &p) {
        o << "polynomial {";
        if (!p.Terms.empty ()) {
            typename polynomial<A, N, x>::terms z = p.Terms;
            while (true) {
                typename polynomial<A, N, x>::term t = z.first ().Term;
                o << t.Coefficient << " " << t.Power;
                z = z.rest ();
                if (z.empty ()) break;
                o << " + ";
            }
        }
        return o << "}";
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::term::operator == (const term &t) const {
        return Coefficient == t.Coefficient && Power.Exponent == t.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::term::operator == (const A &a) const {
        return operator == (term {a, 0});
    }
    
    template <typename A, typename N, char x>
    bool inline polynomial<A, N, x>::term::operator != (const term &t) const {
        return !operator == (t);
    }
    
    template <typename A, typename N, char x>
    bool inline polynomial<A, N, x>::term::operator != (const A &a) const {
        return !operator == (term {a, 0});
    }
    
    template <typename A, typename N, char x>
    A inline polynomial<A, N, x>::term::operator () (const A f) const{
        return (f ^ Power) * Coefficient;
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::term::operator () (const polynomial p) const {
        return (p ^ Power.Exponent) * Coefficient;
    }
    
    template <typename A, typename N, char x>
    typename polynomial<A, N, x>::term inline polynomial<A, N, x>::term::operator * (const term &z) const {
        return term {Coefficient * z.Coefficient, Power.Exponent + z.Power.Exponent};
    }
    
    template <typename A, typename N, char x>
    typename polynomial<A, N, x>::term inline polynomial<A, N, x>::term::operator * (const A z) const {
        return term {Coefficient * z, Power.Exponent};
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::term::operator * (const polynomial p) const {
        return polynomial {for_each ([this] (ordering o) -> term {
            return this->operator * (o.Term);
        }, p.Terms)};
    }

    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::term::term (A a, N p) : Coefficient {a}, Power {variable<x> {}, p} {}
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::ordering::ordering (term t) : Term{t} {}
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::ordering::operator == (const ordering &o) const {
        return Term.Power.Exponent == o.Term.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::ordering::operator > (const ordering &o) const {
        return Term.Power.Exponent > o.Term.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::ordering::operator < (const ordering &o) const {
        return Term.Power.Exponent < o.Term.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::ordering::operator <= (const ordering &o) const {
        return Term.Power.Exponent <= o.Term.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    inline bool polynomial<A, N, x>::ordering::operator >= (const ordering &o) const {
        return Term.Power.Exponent >= o.Term.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    division<typename polynomial<A, N, x>::ordering>
    polynomial<A, N, x>::ordering::operator / (const ordering &o) const {
        if (o.Term == 0) throw division_by_zero {};
        if (Term.Power < o.Term.Power) return division<polynomial> {*this, o};
        return division<ordering> {
            ordering {term {
                Term.Coefficient / o.Term.Coefficient, 
                Term.Power - o.Term.Power}}, 
            ordering {0}};
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial (const terms l) : Terms {l} {}
    
    template <typename A, typename N, char x>
    polynomial<A, N, x>::polynomial (const list<term> l) : Terms {} {
        list<term> t = l;
        while (!t.empty ()) {
            operator += (t.first ());
            t = t.rest ();
        }
    }
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial () : Terms {} {}
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial (const A a) : Terms {terms {}.insert (ordering {term {a, 0}})} {}
    
    template <typename A, typename N, char x>
    inline polynomial<A, N, x>::polynomial (const term t) : Terms {terms {}.insert (ordering {t})} {}
    
    template <typename A, typename N, char x>
    typename polynomial<A, N, x>::term inline polynomial<A, N, x>::first () const {
        return Terms.first ().Term;
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::rest () const {
        return Terms.rest ();
    }
    
    template <typename A, typename N, char x>
    constexpr polynomial<A, N, x> inline polynomial<A, N, x>::unit () {
        return polynomial {term {A {1}, N {0}}};
    } 
    
    template <typename A, typename N, char x>
    constexpr polynomial<A, N, x> inline polynomial<A, N, x>::zero () {
        return polynomial {term {A {0}, N {0}}};
    }
    
    template <typename A, typename N, char x>
    uint32 inline polynomial<A, N, x>::degree () const {
        if (Terms.empty ()) return 0;
        return Terms.first ().Term.Power.Exponent;
    }
    
    template <typename A, typename N, char x>
    bool polynomial<A, N, x>::equal (const polynomial p, const polynomial q) {
        if (p.degree () != q.degree ()) return false;
        if (p.Terms.empty ()) return true;
        if (p.first () != q.first ()) return false;
        return equal (p.rest (), q.rest ());
    }
    
    template <typename A, typename N, char x>
    bool inline polynomial<A, N, x>::operator == (const polynomial p) const {
        return equal (*this, p);
    }
    
    template <typename A, typename N, char x>
    bool inline polynomial<A, N, x>::operator != (const polynomial p) const {
        return !equal (*this, p);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::insert (const term z) const {
        if (Terms.empty ()) return z;
        if (z == term {A {0}, N {0}}) return *this;
        ordering first = Terms.first ();
        if (first == ordering {z})
            return rest ().insert (term {z.Coefficient + first.Term.Coefficient, z.Power.Exponent});
        return Terms.insert (ordering {z});
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::insert (terms t) const {
        if (Terms.empty ()) return {t};
        if (t.empty ()) return *this;
        return insert (t.first ().Term).insert (t.rest ());
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::insert (const polynomial p) const {
        if (Terms.empty ()) return p;
        return insert (p.Terms);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::plus (const polynomial p, const term z) {
        return p.insert (z);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::plus (const polynomial t, const A a) {
        return plus (t, term {a, 0});
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::plus (const polynomial a, const polynomial b) {
        return a.insert (b.Terms);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator + (const term t) const {
        return plus (*this, t);
    };
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator + (const A a) const {
        return plus (*this, a);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator + (const polynomial& p) const {
        return plus (*this, p);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator += (const term t) {
        return *this = *this + t;
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator += (const A a) {
        return *this = *this + a;
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator += (const polynomial &p) {
        return *this = *this + p;
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator * (const A z) const {
        return reduce<polynomial> (math::plus<polynomial> {},
            for_each ([z] (ordering o) -> polynomial {
                return o.Term * z;
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator * (const term z) const {
        return reduce<polynomial> (math::plus<polynomial> {},
            for_each ([z] (ordering o) -> polynomial {
                return o.Term * z;
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator * (const polynomial p) const {
        return reduce<polynomial> (math::plus<polynomial> {},
            for_each ([p] (ordering o) -> polynomial {
                return o.Term * p;
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator ^ (const N n) const {
        if (n == 0) return unit ();
        if (n == 1) return *this;
        return operator * (*this) ^ (n - 1);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator () (const A f) const {
        if (empty (Terms)) return 0;
        return Terms.first () (f) + polynomial {Terms.rest ()} (f);
    }
    
    // inefficient as it computes powers repeatedly. 
    template <typename A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::operator () (const polynomial p) const {
        return reduce<polynomial> (math::plus<polynomial> {},
            for_each ([p] (ordering o) -> polynomial {
                return o.Term (p);
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    bool inline polynomial<A, N, x>::operator == (const polynomial &p) {
        return equal (Terms, p.Terms);
    }
    
    template <typename A, typename N, char x>
    bool inline polynomial<A, N, x>::operator != (const polynomial &p) {
        return !operator == (p);
    }
    
    template <typename A, typename N, char x>
    division<polynomial<A, N, x>> inline polynomial<A, N, x>::divide (const polynomial Dividend, const polynomial Divisor) {
        if (Divisor == 0) throw division_by_zero {};
        if (Divisor.degree () > Dividend.degree ()) return division<polynomial> {Dividend, Divisor};
        term z = Divisor.first () / Dividend.first ();
        return polynomial {z} + divide (Dividend - Divisor * x, Divisor);
    }
    
    template <typename A, typename N, char x>
    division<polynomial<A, N, x>> inline polynomial<A, N, x>::operator / (const polynomial p) const {
        return divide (*this, p);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::derivative () const {
        return reduce (math::plus<polynomial> {},
            for_each([] (ordering o)->polynomial {
                if (o.Term.Power == 0) return zero ();
                return polynomial {term {o.Term.Coefficient * o.Term.Power, o.Term.Power - 1}};
            }, Terms));
    }
    
    template <typename A, typename N, char x>
    template <typename ... P>
    polynomial<A, N, x> inline polynomial<A, N, x>::make (P... rest) {
        return build (polynomial {}, rest...);
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::build (const polynomial p) {
        return p;
    }
    
    template <typename A, typename N, char x>
    template <typename ... P>
    polynomial<A, N, x> inline polynomial<A, N, x>::build (const polynomial p, const A z, P... rest) {
        return build (p + z, rest...);
    }
    
    template <typename A, typename N, char x>
    template <typename ... P>
    polynomial<A, N, x> inline polynomial<A, N, x>::build (const polynomial p, const term z, P... rest) {
        return build (p + z, rest...);
    }
    
}

#endif
