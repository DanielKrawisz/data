// Copyright (c) 2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_DIGITS
#define DATA_ENCODING_DIGITS

#include <data/list/linked.hpp>
#include <data/math/number/natural.hpp>

namespace data::encoding {
    template <typename N>
    std::string write_base(const N& n, std::string digits) {
        uint32 base = digits.size();
        if (base < 2) return "";
        functional::stack::linked<char> dig{};
        N x = n;
        while(x > 0) {
            math::division<N> d = x.divide(base);
            dig = dig << digits[(uint64)(d.Remainder)];
            x = d.Quotient;
        }
        
        std::string o;
        o.resize(dig.size());
        int i = 0;
        while(dig.size() > 0) {
            o[i] = dig.first();
            dig = dig.rest();
            i++;
        }
        
        return o;
    }
    
    template <typename N, typename f>
    N read_base(string_view s, uint32 base, f inverse_digits);
}

#endif

