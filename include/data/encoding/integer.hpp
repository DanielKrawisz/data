// Copyright (c) 2019 Daniel Krawisz
// Copyright (c) 2019 Katrina Swales
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ENCODING_INTEGER
#define DATA_ENCODING_INTEGER

#include <data/types.hpp>
#include <data/encoding/hex.hpp>
#include <data/encoding/invalid.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/algorithm/string.hpp>
#include <ctre.hpp>

namespace data::encoding {
    struct decimal {
        static constexpr auto pattern = ctll::fixed_string{"0|([1-9][0-9]*)"};
        
        static bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        static bool nonzero(string_view s) {
            return valid(s) && s[0] != '0';
        }
        
        static uint32 digits(string_view s) {
            return valid(s) ? s.size() : 0;
        }
    };
    
    struct hexidecimal {
        static constexpr auto pattern = ctll::fixed_string{"0x(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*)"};
        
        static constexpr auto zero_pattern = ctll::fixed_string{"0x(00)*"};
        
        static bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        static bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        static uint32 digits(string_view s) {
            return valid(s) ? s.size() - 2 : 0;
        }
    };
    
    struct natural {
        static constexpr auto pattern = ctll::fixed_string{"0|([1-9][0-9]*)|(0x(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        
        static constexpr auto zero_pattern = ctll::fixed_string{"0|0x(00)*"};
        
        static bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        static bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        static uint32 digits(string_view s) {
            return std::max(decimal::digits(s), hexidecimal::digits(s));
        }
    };
    
    struct integer {
        static constexpr auto pattern = ctll::fixed_string{
            "0|(-?[1-9][0-9]*)|(-?0x(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        
        static constexpr auto zero_pattern = ctll::fixed_string{
            "0|0x(00)*"};
        
        static constexpr auto negative_pattern = ctll::fixed_string{
            "-([1-9][0-9]*)|(0x(([0-9a-f][0-9a-f])*)|(([0-9A-F][0-9A-F])*))"};
        
        static bool valid(string_view s) {
            return ctre::match<pattern>(s);
        } 
        
        static bool negative(string_view s) {
            return ctre::match<negative_pattern>(s);
        }
        
        static bool nonzero(string_view s) {
            return valid(s) && !ctre::match<zero_pattern>(s);
        }
        
        static uint32 digits(string_view s) {
            return negative(s) ? natural::digits(s.substr(1, s.size() - 1)) : natural::digits(s);
        }
    };
    
}

#endif

