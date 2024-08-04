// Copyright (c) 2019-202[ Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SLICE
#define DATA_SLICE

#include <span>
#include <data/tools/index_iterator.hpp>
#include <data/meta/greater.hpp>
#include <data/meta/unsigned_minus.hpp>

namespace data::meta {

    template <uint32 X, uint32 ceil>
    struct ceiling {
        constexpr static uint32 value = (X % ceil == 0 ? X : X % ceil);
    };
    
}

namespace data {
    
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X, size_t ...> struct slice;
    
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X> struct slice<X> : std::span<X> {
        using std::span<X>::span;
        
        constexpr X& operator [] (size_t i) const {
            if (this->size () == 0) throw std::out_of_range {"slice size 0"};
            if (i < 0 || i >= this->size ()) return this->operator [] ((i + this->size ()) % this->size ());
            return this->data ()[i];
        }
        
        bool operator == (const slice<X> s) const {
            if (this->data () == s.data ()) return true;
            if (this->size () != s.size ()) return false;
            for (int i = 0; i < this->size (); i++)
                if (operator [] (i) != s[i])
                    return false;
            return true;
        }

        operator view<X> () const;

        /// Selects a range from the current slice
        /// \param b range begins from this index inclusive
        /// \param e range ends at this index excluisive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range (int begin, int end) {
            if (end < 0) end = this->size () + end;
            if (begin >= end || begin >= this->size () || end > this->size ()) return slice {};
            return slice {this->data () + begin, this->data () + end};
        }

        /// Selects a range from the current slice up to end of slice
        /// \param b  range begins from this index inclusive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range (int32 b) {
            return range (0, b);
        }

        template <size_t b, size_t e>
        slice<X, e - b> range () const {
            return slice<X, e - b> {this->data () + b, e - b};
        }

        using iterator = std::span<X>::iterator;
        using const_iterator = view<X>::iterator;
        using reverse_iterator = std::span<X>::reverse_iterator;
        using const_reverse_iterator = view<X>::reverse_iterator;

        iterator begin ();
        iterator end ();

        const_iterator begin () const;
        const_iterator end () const;

        reverse_iterator rbegin ();
        reverse_iterator rend ();

        const_reverse_iterator rbegin () const;
        const_reverse_iterator rend () const;

    };
    
    template <typename X, size_t n> struct slice<X, n> : public std::span<X, n> {

        using std::span<X, n>::span;
        slice (X *x): std::span<X, n> {x, n} {}

        constexpr X& operator [] (size_t i) const {
            if (this->size () == 0) throw std::out_of_range {"slice size 0"};
            if (i < 0 || i >= this->size ()) return this->operator [] ((i + this->size ()) % this->size ());
            return this->data ()[i];
        }

        bool operator == (const slice<X, n> s) const {
            if (this->data () == s.data ()) return true;
            if (this->size () != s.size ()) return false;
            for (int i = 0; i < this->size (); i++)
                if (operator [] (i) != s[i])
                    return false;
            return true;
        }

        operator slice<X> () const {
            return slice<X> {this->data (), this->data () + n};
        }

        operator view<X> () const;

        template <size_t b, size_t e> slice<X, e - b> range () const {
            return slice<X, e - b> {this->data () + b, e - b};
        }

    };

    template <typename X> inline slice<X>::operator view<X> () const {
        return view<X> {this->data (), this->size () * sizeof (X)};
    }

    template <typename X> slice<X>::iterator inline slice<X>::begin () {
        return std::span<X>::begin ();
    }

    template <typename X> slice<X>::iterator inline slice<X>::end () {
        return std::span<X>::end ();
    }

    template <typename X> slice<X>::const_iterator inline slice<X>::begin () const {
        return view<X> (*this).begin ();
    }

    template <typename X> slice<X>::const_iterator inline slice<X>::end () const {
        return view<X> (*this).end ();
    }

    template <typename X> slice<X>::reverse_iterator inline slice<X>::rbegin () {
        return std::span<X>::rbegin ();
    }

    template <typename X> slice<X>::reverse_iterator inline slice<X>::rend () {
        return std::span<X>::rend ();
    }

    template <typename X> slice<X>::const_reverse_iterator inline slice<X>::rbegin () const {
        return view<X> (*this).rbegin ();
    }

    template <typename X> slice<X>::const_reverse_iterator inline slice<X>::rend () const {
        return view<X> (*this).rend ();
    }

    template <typename X, size_t n> inline slice<X, n>::operator view<X> () const {
        return view<X> {this->data (), n};
    }

}

#endif
