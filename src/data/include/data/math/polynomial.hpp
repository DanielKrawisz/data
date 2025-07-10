// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_POLYNOMIAL
#define DATA_MATH_POLYNOMIAL

#include <data/tools.hpp>
#include <data/divide.hpp>
#include <data/arithmetic.hpp>
#include <data/math/field.hpp>

namespace data::math {

    template <typename A, typename Exp = A>
    struct power {
        A Base;
        Exp Exponent;

        bool operator == (const power &x) const {
            return Base == x.Base && Exponent == x.Exponent;
        }
    };

    template <typename A, typename Mod>
    std::ostream inline &operator << (std::ostream &o, const power<A, Mod> &p) {
        o << p.Base;
        if (p.Exponent != 1) o << " ^ " << p.Exponent;
        return o;
    }

    template <ring A, typename N, char x> struct polynomial;

    template <ring A, typename N, char x>
    bool operator == (const polynomial<A, N, x>, const polynomial<A, N, x>);

    template <ring A, typename N, char x>
    polynomial<A, N, x> operator - (const polynomial<A, N, x>);

    template <ring A, typename N, char x>
    polynomial<A, N, x> operator + (const polynomial<A, N, x>, const polynomial<A, N, x>);

    template <ring A, typename N, char x>
    polynomial<A, N, x> operator - (const polynomial<A, N, x>, const polynomial<A, N, x>);

    template <ring A, typename N, char x>
    polynomial<A, N, x> operator * (const polynomial<A, N, x>, const polynomial<A, N, x>);

    // the other operations work if A is a ring, but for division we need A to be a field.
    template <field A, typename N, char x>
    struct divide<polynomial<A, N, x>> {
        division<polynomial<A, N, x>> operator () (const polynomial<A, N, x>, const nonzero<polynomial<A, N, x>>);
    };

    template <typename A, typename N, char x>
    std::ostream &operator << (std::ostream &o, const polynomial<A, N, x> &p);

    template <typename A, typename N, char x>
    struct identity<plus<polynomial<A, N, x>>, polynomial<A, N, x>> : identity<plus<A>, A> {
        polynomial<A, N, x> operator () ();
    };

    template <typename A, typename N, char x>
    struct identity<times<polynomial<A, N, x>>, polynomial<A, N, x>> : identity<times<A>, A> {
        polynomial<A, N, x> operator () ();
    };

    template <char name> struct variable {};

    template <char name>
    bool operator == (variable<name>, variable<name>);

    template <char name>
    std::ostream &operator << (std::ostream &o, const variable<name> &x);

    template <ring A, typename N, char x>
    struct polynomial {
        
        polynomial ();
        polynomial (const A a);

        bool valid () const;
        
        constexpr static polynomial zero ();
        constexpr static polynomial unit ();
        constexpr static polynomial var ();
        
        uint32 degree () const;
        
        static bool equal (const polynomial, const polynomial);
        static polynomial negative (const polynomial);
        static polynomial plus (const polynomial, const polynomial);
        static polynomial minus (const polynomial, const polynomial);
        static polynomial times (const polynomial, const polynomial);
        static polynomial pow (const polynomial, const N &);
        
        bool operator == (const A &) const;
        
        polynomial operator + (const A) const;
        
        polynomial operator += (const A);
        polynomial operator += (const polynomial &);
        
        polynomial operator * (const A) const;
        
        polynomial operator *= (const A) const;
        polynomial operator *= (const polynomial) const;

        polynomial operator () (const A) const;
        polynomial operator () (const polynomial) const;

        polynomial operator ^ (const N &n) const;
        
        bool operator > (const polynomial) const;
        bool operator < (const polynomial) const;
        bool operator <= (const polynomial) const;
        bool operator >= (const polynomial) const;

        polynomial derivative () const;

        static division<polynomial> divide (const polynomial, const polynomial);
        
    private:
        // remove any terms that are equal to zero.
        polynomial normalize () const;

        using power = math::power<variable<x>, N>;

        // a term in a polynomial is the part like A (x ^ N).
        struct term {
            A Coefficient;
            power Power;
            
            explicit term () : Coefficient {0}, Power {{}, 1} {}
            explicit term (A, const N &n);

            // cannot have 0 ^ 0.
            bool valid () const {
                return Coefficient != 0 || Power.Exponent != 0u;
            }
            
            bool operator == (const term &) const;
            bool operator == (const A &) const;
            
            A operator () (const A) const;
            polynomial operator () (const polynomial) const;
            
            term operator - () const {
                return term {-Coefficient, Power.Exponent};
            }

            term operator * (const term &) const;
            term operator * (const A) const;
            polynomial operator * (const polynomial) const;
            
            // used for ordering terms so the fact that one term < another
            // does not mean that it is actually numerically bigger.
            std::weak_ordering operator <=> (const term &o) const {
                return Coefficient == 0 && o.Coefficient == 0 || Power.Exponent == 0u && o.Power.Exponent == 0u ? std::weak_ordering::equivalent :
                    Power.Exponent == o.Power.Exponent ? Coefficient <=> o.Coefficient : o.Power.Exponent <=> Power.Exponent;
            } 

            term derivative () const {
                if (Power.Exponent == 0) return term {};
                return term {Coefficient * Power.Exponent, Power.Exponent - 1};
            }
        };
        
        polynomial (const term t);
        
        polynomial operator + (const term t) const;
        
        polynomial operator += (const term t);
        
        polynomial operator * (const term t) const;

        using terms = ordered_sequence<term>;
        
        terms Terms;
        
        term first () const;
        
        polynomial rest () const;
        
        friend std::ostream &operator << <A, N, x> (std::ostream &, const polynomial &);
        
        polynomial insert (const term) const;
        polynomial insert (const terms) const;
        
        explicit polynomial (const terms t, void*) : Terms {t} {}
    };
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline identity<plus<polynomial<A, N, x>>, polynomial<A, N, x>>::operator () () {
        return identity<plus<A>, A>::value ();
    }
    
    template <typename A, typename N, char x>
    polynomial<A, N, x> inline identity<times<polynomial<A, N, x>>, polynomial<A, N, x>>::operator () () {
        return identity<times<A>, A>::value ();
    }

    template <char name>
    bool inline operator == (variable<name>, variable<name>) {
        return true;
    }

    template <char name>
    std::ostream inline &operator << (std::ostream &o, const variable<name> &x) {
        return o << name;
    }

    template <field A, typename N, char x>
    division<polynomial<A, N, x>> inline divide<polynomial<A, N, x>>::operator ()
    (const polynomial<A, N, x> a, const nonzero<polynomial<A, N, x>> b) {
        return polynomial<A, N, x>::divide (a, b.Value);
    }

    template <typename A, typename N, char x>
    std::ostream &operator << (std::ostream &o, const polynomial<A, N, x> &p) {
        o << "polynomial {";
        if (!p.Terms.empty ()) {
            typename polynomial<A, N, x>::terms z = p.Terms;
            while (true) {
                typename polynomial<A, N, x>::term t = z.first ();
                if (t.Power.Exponent == 0) o << t.Coefficient;
                else {
                    if (t.Coefficient != 1) o << t.Coefficient << " ";
                    o << t.Power;
                }
                z = z.rest ();
                if (z.empty ()) break;
                o << " + ";
            }
        } else o << "0";
        return o << "}";
    }

    template <ring A, typename N, char x>
    bool inline operator == (const polynomial<A, N, x> a, const polynomial<A, N, x> b) {
        return polynomial<A, N, x>::equal (a, b);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline operator - (const polynomial<A, N, x> p) {
        return polynomial<A, N, x>::negative (p);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline operator + (const polynomial<A, N, x> a, const polynomial<A, N, x> b) {
        return polynomial<A, N, x>::plus (a, b);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline operator - (const polynomial<A, N, x> a, const polynomial<A, N, x> b) {
        return polynomial<A, N, x>::minus (a, b);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline operator * (const polynomial<A, N, x> a, const polynomial<A, N, x> b) {
        return polynomial<A, N, x>::times (a, b);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator ^ (const N &n) const {
        return polynomial<A, N, x>::pow (*this, n);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::pow (const polynomial p, const N &n) {
        return data::pow (p, n);
        //throw 0;
    }

    template <ring A, typename N, char x>
    bool polynomial<A, N, x>::valid () const {
        return Terms.valid ();
    }
    
    template <ring A, typename N, char x>
    bool inline polynomial<A, N, x>::term::operator == (const term &t) const {
        return *this <=> t == 0;
    }
    
    template <ring A, typename N, char x>
    bool inline polynomial<A, N, x>::term::operator == (const A &a) const {
        return operator == (term {a, 0});
    }
    
    template <ring A, typename N, char x>
    A inline polynomial<A, N, x>::term::operator () (const A f) const{
        return (f ^ Power) * Coefficient;
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::term::operator () (const polynomial p) const {
        return (p ^ Power.Exponent) * Coefficient;
    }
    
    template <ring A, typename N, char x>
    typename polynomial<A, N, x>::term inline polynomial<A, N, x>::term::operator * (const term &t) const {
        return term {Coefficient * t.Coefficient, Power.Exponent + t.Power.Exponent};
    }

    template <ring A, typename N, char x>
    typename polynomial<A, N, x>::term inline polynomial<A, N, x>::term::operator * (const A a) const {
        return term {Coefficient * a, Power.Exponent};
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::term::operator * (const polynomial p) const {
        return fold ([this] (const polynomial &p, const term &t) -> polynomial {
            return p + (*this * t);
        }, polynomial {}, reverse (p.Terms));
    }

    template <ring A, typename N, char x>
    inline polynomial<A, N, x>::term::term (A a, const N& n) : Coefficient {a}, Power {{}, n} {}
    
    template <ring A, typename N, char x>
    inline polynomial<A, N, x>::polynomial () : Terms {} {}
    
    template <ring A, typename N, char x>
    inline polynomial<A, N, x>::polynomial (const A a) : Terms {terms {}.insert (term {a, 0})} {}
    
    template <ring A, typename N, char x>
    inline polynomial<A, N, x>::polynomial (const term t) : Terms {terms {}.insert (t)} {}
    
    template <ring A, typename N, char x>
    typename polynomial<A, N, x>::term inline polynomial<A, N, x>::first () const {
        return Terms.first ();
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::rest () const {
        return polynomial {Terms.rest (), nullptr};
    }
    
    template <ring A, typename N, char x>
    constexpr polynomial<A, N, x> inline polynomial<A, N, x>::unit () {
        return polynomial {term {A {1}, 0}};
    } 
    
    template <ring A, typename N, char x>
    constexpr polynomial<A, N, x> inline polynomial<A, N, x>::zero () {
        return polynomial {term {A {0}, 0}};
    }
    
    template <ring A, typename N, char x>
    constexpr polynomial<A, N, x> inline polynomial<A, N, x>::var () {
        return polynomial (term {A (1), 1});
    }
    
    template <ring A, typename N, char x>
    uint32 inline polynomial<A, N, x>::degree () const {
        auto p = normalize ();
        if (p.Terms.empty ()) return 0;
        return p.Terms.first ().Power.Exponent;
    }
    
    template <ring A, typename N, char x>
    bool inline polynomial<A, N, x>::equal (const polynomial p, const polynomial q) {
        return p.normalize ().Terms == q.normalize ().Terms;
    }

    template <ring A, typename N, char x>
    bool inline polynomial<A, N, x>::operator == (const A &p) const {
        return equal (*this, polynomial {p});
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::insert (const term t) const {
        if (t == term {}) return *this;
        if (Terms.empty ()) return {t};
        term first = Terms.first ();
        if (t.Power.Exponent > first.Power.Exponent) return polynomial {Terms.insert (t), nullptr};
        if (first.Power.Exponent == t.Power.Exponent) return rest ().insert (term {t.Coefficient + first.Coefficient, t.Power.Exponent});
        return rest ().insert (t).insert (first);
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::insert (terms t) const {
        if (Terms.empty ()) return polynomial {t, nullptr};
        if (t.empty ()) return *this;
        return polynomial {insert (t.first ()).insert (t.rest ())};
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator + (const A a) const {
        return insert (term {a, 0});
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::plus (const polynomial a, const polynomial b) {
        return a.insert (b.Terms).normalize ();
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::minus (const polynomial a, const polynomial b) {
        return a + -b;
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::negative (const polynomial a) {
        return fold ([] (const polynomial p, const term &t) -> polynomial {
            return p + -t;
        }, polynomial {}, reverse (a.Terms));
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator + (const term t) const {
        return insert (t);
    };
    
    template <ring A, typename N, char x>
    inline polynomial<A, N, x> polynomial<A, N, x>::operator += (const term t) {
        return *this = *this + t;
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator += (const A a) {
        return *this = *this + a;
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator += (const polynomial &p) {
        return *this = *this + p;
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator * (const A z) const {
        return fold ([z] (const polynomial p, const term &t) -> polynomial {
            return p + t * z;
        }, polynomial {}, reverse (Terms));
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator * (const term z) const {
        return fold ([z] (const polynomial p, const term &t) -> polynomial {
            return p + t * z;
        }, polynomial {}, reverse (Terms));
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::times (const polynomial a, const polynomial b) {
        return fold ([b] (const polynomial p, const term &t) -> polynomial {
            return p + t * b;
        }, polynomial {}, reverse (a.Terms));
    }
    
    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::operator () (const A f) const {
        if (empty (Terms)) return 0;
        return Terms.first () (f) + polynomial {Terms.rest ()} (f);
    }
    
    // inefficient as it computes powers repeatedly. 
    template <ring A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::operator () (const polynomial p) const {
        return fold ([p] (const polynomial n, const term &t) -> polynomial {
            return n + t (p);
        }, polynomial {}, reverse (Terms));
    }

    template <ring A, typename N, char x>
    division<polynomial<A, N, x>> polynomial<A, N, x>::divide (const polynomial Dividend, const polynomial Divisor) {
        if (Divisor == 0) throw division_by_zero {};

        function<division<polynomial> (const polynomial, const polynomial)> f = [&f] (const polynomial d, const polynomial s) {
            if (s.degree () > d.degree ()) return division<polynomial> {polynomial::zero (), d};
            auto d1 = d.first ();
            auto s1 = s.first ();
            term t {d1.Coefficient / s1.Coefficient, d1.Power.Exponent - s1.Power.Exponent};
            division<polynomial<A, N, x>> div = f (d - s * t, s);
            return division<polynomial<A, N, x>> {div.Quotient + t, div.Remainder};
        };

        return f (Dividend.normalize (), Divisor.normalize ());
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> polynomial<A, N, x>::normalize () const {
        return polynomial {select (Terms, function<bool (const term &)> {[] (const term &t) -> bool {
            return t.Coefficient != 0;
        }}), nullptr};
    }

    template <ring A, typename N, char x>
    polynomial<A, N, x> inline polynomial<A, N, x>::derivative () const {
        return fold ([] (const polynomial p, const term &t) -> polynomial {
            return p + t.derivative ();
        }, polynomial {}, reverse (Terms)).normalize ();
    }
    
}

#endif
