// Copyright (c) 2019-202[ Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_SLICE
#define DATA_SLICE

#include <span>
#include <data/tools/index_iterator.hpp>
#include <data/io/exception.hpp>

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

    using byte_slice = slice<const byte>;
    
    // Slice is an indexed section of an array which
    // can create smaller slices. 
    template <typename X> struct slice<X> : std::span<X> {
        using std::span<X>::span;
        
        constexpr X &operator [] (size_t i) const {
            if (this->size () == 0) throw out_of_range {"slice size 0"};
            if (i < 0 || i >= this->size ()) return this->operator [] ((i + this->size ()) % this->size ());
            return this->data ()[i];
        }
        
        constexpr bool operator == (const slice<X> s) const {
            if (this->data () == s.data ()) return true;
            if (this->size () != s.size ()) return false;
            for (int i = 0; i < this->size (); i++)
                if (operator [] (i) != s[i])
                    return false;
            return true;
        }

        /// Selects a range from the current slice
        /// \param b range begins from this index inclusive
        /// \param e range ends at this index excluisive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range (int begin, int end) const {
            if (end < 0) end = this->size () + end;
            if (begin >= end || begin >= this->size () || end > this->size ()) return slice {};
            return slice {this->data () + begin, this->data () + end};
        }

        /// Selects a range from the current slice up to end of slice
        /// \param b  range begins from this index inclusive
        /// \return a slice containing the requested range
        [[nodiscard]] slice<X> range (int32 b) const {
            return range (0, b);
        }

        template <size_t b, size_t e>
        slice<X, e - b> range () const {
            return slice<X, e - b> {this->data () + b, e - b};
        }

        slice<X> drop (size_t b) const {
            return range (b, this->size ());
        }

        using iterator = decltype (std::declval<std::span<X>> ().begin ());
        using const_iterator = decltype (std::declval<const std::span<X>> ().begin ());
        using reverse_iterator = decltype (std::declval<std::span<X>> ().rbegin ());
        using const_reverse_iterator = decltype (std::declval<const std::span<X>> ().rbegin ());

        constexpr iterator begin ();
        constexpr iterator end ();

        constexpr const_iterator begin () const;
        constexpr const_iterator end () const;

        constexpr reverse_iterator rbegin ();
        constexpr reverse_iterator rend ();

        constexpr const_reverse_iterator rbegin () const;
        constexpr const_reverse_iterator rend () const;

    };
    
    template <typename X, size_t n> struct slice<X, n> : public std::span<X, n> {
        //constexpr const static size_t size = n;

        using std::span<X, n>::span;
        slice (X *x): std::span<X, n> {x, n} {}

        constexpr X &operator [] (size_t i) const {
            if (this->size () == 0) throw out_of_range {"slice size 0"};
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

        template <size_t b, size_t e> slice<X, e - b> range () const {
            return slice<X, e - b> {this->data () + b, e - b};
        }

    };

    template <typename X> constexpr slice<X>::iterator inline slice<X>::begin () {
        return std::span<X>::begin ();
    }

    template <typename X> constexpr slice<X>::iterator inline slice<X>::end () {
        return std::span<X>::end ();
    }

    template <typename X> constexpr slice<X>::const_iterator inline slice<X>::begin () const {
        return std::span<X>::begin ();
    }

    template <typename X> constexpr slice<X>::const_iterator inline slice<X>::end () const {
        return std::span<X>::end ();
    }

    template <typename X> constexpr slice<X>::reverse_iterator inline slice<X>::rbegin () {
        return std::span<X>::rbegin ();
    }

    template <typename X> constexpr slice<X>::reverse_iterator inline slice<X>::rend () {
        return std::span<X>::rend ();
    }

    template <typename X> constexpr slice<X>::const_reverse_iterator inline slice<X>::rbegin () const {
        return std::span<X>::rbegin ();
    }

    template <typename X> constexpr slice<X>::const_reverse_iterator inline slice<X>::rend () const {
        return std::span<X>::rend ();
    }

}

#endif
