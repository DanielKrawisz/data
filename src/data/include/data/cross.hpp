// Copyright (c) 2019-2022 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CROSS
#define DATA_CROSS

#include <data/sequence.hpp>
#include <data/slice.hpp>
#include <data/valid.hpp>
#include <data/arithmetic/words.hpp>
#include <data/stream.hpp>
#include <data/math/abs.hpp>
#include <data/math/associative.hpp>
#include <data/math/commutative.hpp>

namespace data {
    
    // The cartesian product. 
    // it is the same as a vector with some slight improvements. 
    template <typename X> struct cross : std::vector<X> {
        constexpr cross ();
        explicit cross (size_t size);
        explicit cross (size_t size, X fill);
        
        cross (std::initializer_list<X> x);
        
        template<sequence list>
        explicit cross (list l);
        
        bool valid () const;
        
        X &operator [] (int i);
        
        const X &operator [] (int i) const;
        
        explicit operator slice<X> ();
        explicit operator slice<const X> () const;
        
        slice<X> range (int);
        slice<X> range (int, int);

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;
        
        cross (std::vector<X> &&v) : std::vector<X> {v} {}
        
    protected:
        void fill (const X &x) {
            for (X& z : *this) z = x;
        }
        
    };
    
    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s);

    template <typename X, size_t... > struct array;
    
    template <typename X, size_t size> struct array<X, size> : public std::array<X, size> {
        constexpr static size_t Size = size;
        using std::array<X, size>::array;

        constexpr array (std::initializer_list<X> x);

        // make an array filled with a particular value.
        static array filled (const X &x);

        bool valid () const;

        X &operator [] (size_t i);
        const X &operator [] (size_t i) const;

        explicit operator slice<X, size> ();

        slice<X> range (int);
        slice<X> range (int, int);

        explicit operator slice<const X, size> () const;

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;
    };

    template <typename X, size_t size, size_t... sizes> struct array<X, size, sizes...> : public array<X, size * array<X, sizes...>::Size> {
        constexpr static size_t Size = size * array<X, sizes...>::Size;

        array ();
        array (std::initializer_list<array<X, sizes...>>);

        slice<X, sizes...> &operator [] (size_t i);
        slice<const X, sizes...> &operator [] (size_t i) const;

    };

    template <typename X> struct array<X> {
        constexpr static size_t Size = 1;
        array (const X &);
        array (X &&);
        operator X () const;

        X Value;

        bool valid () const;

        explicit operator slice<X, 1> ();

        slice<X> range (int);
        slice<X> range (int, int);

        explicit operator slice<const X, 1> () const;

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;

        slice<X>::iterator begin ();
        slice<X>::iterator end ();

        slice<const X>::iterator begin () const;
        slice<const X>::iterator end () const;

    };

    // vector addition and subtraction.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator + (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator - (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x) {
        {-x} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator - (const array<X, sizes...> &);

    // scalar multiplication and division.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x * y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator * (const array<X, sizes...> &, const X &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator / (const array<X, sizes...> &, const X &);

    // multiplication operation good enough for an inner product and matrix multiplication
    template <typename X, size_t... A, size_t C, size_t... B> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
        {x * inner (x, y)} -> implicitly_convertible_to<X>;
    } && requires () {
        {X {}};
    } array<X, A..., B...> operator * (const array<X, A..., C> &a, const array<X, C, B...> &b);
}

namespace data::math {
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } struct is_associative<plus<array<X, sizes...>>, array<X, sizes...>> : is_associative<plus<X>, X> {};

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } struct is_commutative<plus<array<X, sizes...>>, array<X, sizes...>> : is_commutative<plus<X>, X> {};
}

namespace data {
    template <typename X>
    bool cross<X>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

    template <typename X>
    X &cross<X>::operator [] (int i) {
        size_t size = this->size ();
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::vector<X>::operator [] (i);
    }

    template <typename X>
    const X &cross<X>::operator [] (int i) const {
        size_t size = this->size ();
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::vector<X>::operator [] (i);
    }

    template <typename X, size_t size>
    constexpr inline array<X, size>::array (std::initializer_list<X> x) : std::array<X, size> {} {
        if (x.size () != size) throw exception {} << "out of range";
        size_t index = 0;
        for (const X &xx : x) std::array<X, size>::operator [] (index++) = xx;
    }

    template <typename X, size_t size>
    array<X, size> inline array<X, size>::filled (const X &x) {
        array n {};
        n.fill (x);
        return n;
    }

    template <typename X, size_t size>
    bool inline array<X, size>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

    template <typename X, size_t size>
    X inline &array<X, size>::operator [] (size_t i) {
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::array<X, size>::operator [] (i);
    }

    template <typename X, size_t size>
    const X inline &array<X, size>::operator [] (size_t i) const {
        if (size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= size) return this->operator [] ((i + size) % size);
        return std::array<X, size>::operator [] (i);
    }
    
    template <typename X>
    std::ostream &operator << (std::ostream &o, const cross<X> &s) {
        auto b = s.begin ();
        o << "[";
        while (true) {
            if (b == s.end ()) return o << "]";
            else if (b != s.begin ()) o << ", ";
            o << *b;
            b++;
        }
    }
    
    template <typename X>
    constexpr inline cross<X>::cross () : std::vector<X> {} {}
    
    template <typename X>
    inline cross<X>::cross (size_t size) : std::vector<X> (size) {}
    
    template <typename X>
    inline cross<X>::cross (size_t size, X fill) : std::vector<X> (size) {
        for (auto it = std::vector<X>::begin (); it < std::vector<X>::end (); it++) *it = fill;
    }
    
    template <typename X>
    inline cross<X>::cross (std::initializer_list<X> x) : std::vector<X> {x} {}
    
    template <typename X>
    template<sequence list>
    cross<X>::cross (list l) : cross {} {
        std::vector<X>::resize (data::size (l));
        auto b = std::vector<X>::begin ();
        while (!l.empty ()) {
            *b = l.first ();
            l = l.rest ();
            b++;
        }
    }
    
    template <typename X>
    inline cross<X>::operator slice<X> () {
        return slice<X> (static_cast<std::vector<X> &> (*this));
    }
    
    template <typename X>
    inline slice<X> cross<X>::range (int e) {
        return operator slice<X> ().range (e);
    }
    
    template <typename X>
    inline slice<X> cross<X>::range (int b, int e) {
        return operator slice<X> ().range (e);
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator + (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        array<X, sizes...> x {};
        auto ai = a.begin ();
        auto bi = b.begin ();

        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai + *bi;
            ai++;
            bi++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x - y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator - (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        array<X, sizes...> x {};
        auto ai = a.begin ();
        auto bi = b.begin ();

        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai - *bi;
            ai++;
            bi++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x * y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator * (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai * b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> implicitly_convertible_to<X>;
    } array<X, sizes...> operator / (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai / b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> implicitly_convertible_to<X>;
        {x * y} -> implicitly_convertible_to<X>;
    } && requires () {
        {X {0}};
    } X operator * (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        X x {0};
        auto bi = b.begin ();

        for (auto ai = a.begin (); ai != a.end (); ai++) {
            x += *ai * *bi;
            bi++;
        }

        return x;
    }
    
}

#endif


