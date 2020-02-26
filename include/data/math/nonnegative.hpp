// Copyright (c) 2018-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NONNEGATIVE
#define DATA_MATH_NONNEGATIVE

#include <data/math/ordered.hpp>

namespace data::math {
    
    template <typename R> struct nonnegative : interface::ordered<R> {
        R Number;
        
        nonnegative(const R& n) : Number{n} {}
        
        bool valid() {
            return data::valid(Number) && Number >= R{0};
        }
        
        operator R() const {
            return Number;
        }
        
        nonnegative operator+(const nonnegative& n) const {
            return Number + n.Number;
        }
        
        nonnegative operator*(const nonnegative& n) const {
            return Number * n.Number;
        }
        
        bool operator==(const nonnegative& n) const {
            return Number == n.Number;
        }
        
        bool operator!=(const nonnegative& n) const {
            return !operator==(n);
        }
        
        bool operator>(const nonnegative& n) const {
            return Number > n.Number;
        }
        
        bool operator>=(const nonnegative& n) const {
            return Number >= n.Number;
        }
        
        bool operator<(const nonnegative& n) const {
            return Number < n.Number;
        }
        
        bool operator<=(const nonnegative& n) const {
            return Number <= n.Number;
        }
    };
    
    template <typename R> struct nonzero {
        R Number;
        
        nonzero(const R& n) : Number{n} {}
        
        bool valid() {
            return data::valid(Number) && Number != 0;
        }
        
        operator R() const {
            return Number;
        }
        
        nonzero operator*(const nonzero& n) const {
            return Number * n.Number;
        }
        
        bool operator==(const nonzero& n) const {
            return Number == n.Number;
        }
        
        bool operator!=(const nonzero& n) const {
            return !operator==(n);
        }
    };

}



#endif
