// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_TOOLS_EXPRESSION
#define DATA_TOOLS_EXPRESSION

#include <data/tools.hpp>
    
namespace data {
    
    template <typename element>
    struct expression {
        expression(element &);
        expression operator<<(expression &);
        
        element Head;
        stack<expression> Body;
    };
    
}
    
namespace Diophant {
    
    struct value {
        expression(element &);
        expression operator<<(expression &);
        
        element Head;
        stack<expression> Body;
    };
    
}

#endif 
