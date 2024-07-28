// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_DIGITS
#define DATA_ENCODING_DIGITS

#include <data/tools/linked_stack.hpp>
#include <data/math/number/natural.hpp>

namespace data::encoding {
    
    template <typename N>
    std::string write_base (const N &n, std::string digits) {
        
        N base {digits.size ()};
        if (base < 2) return "";
        
        linked_stack<char> dig {};
        N x = n;
        while (x > 0u) {
            math::division<N> d = math::number::natural_divide (x, base);
            dig = dig << digits[(uint64) (d.Remainder)];
            x = d.Quotient;
        }
        
        std::string o;
        o.resize (size (dig));
        int i = 0;
        while (dig.size () > 0) {
            o[i] = first (dig);
            dig = rest (dig);
            i++;
        }
        
        return o;
    }
    
    template <typename N, typename f>
    N read_base (string_view s, uint32 base, f inverse_digits) {
        N n {0};
        N pow {1};
        //std::cout << "  () read base " << base << " \"" << s << "\"" << std::endl;
        //std::cout << "   () n is " << std::hex << n << std::endl;
        for (auto x = s.rbegin (); x != s.rend (); ++x) {
            n += pow * inverse_digits (*x);
            //std::cout << "   () digit is " << *x << "; n is " << std::hex << n << std::endl;
            pow *= base;
        }
        //std::cout << "  () returning " << std::hex << n << std::endl;
        return n;
    }
}

#endif

