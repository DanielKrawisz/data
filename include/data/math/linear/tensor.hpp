// Copyright (c) 2019-2020 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_LINEAR_FINITE
#define DATA_MATH_LINEAR_FINITE

#include <data/iterable.hpp>

namespace data::math::linear {
    template <typename X, uint32 ...> struct tensor;
    
    template <typename X, uint32 size> using vector = tensor<X, size>;
    
    template <typename X, uint32 size> struct tensor<X, size> {
        static constexpr uint32 Dimension = 1;
        
        section<X, 1> Data;
        
        tensor() : Data(size) {}
        
        const X operator[](uint32 x) const {
            return Data[x];
        }
        
        X operator[](uint32 x) {
            return Data[x];
        }
        
        tensor operator+(const tensor& t) const;
        
        tensor operator*(const X& x) const;
        
        X operator*(const tensor& t) const;
        
    private:
        tensor(section<X, 1> data) : Data{data} {}
        
        template <uint32> struct eliminate;
        
        template <> struct eliminate<size> {
            using eliminated = X;
        };
    };
    
    template <typename X, uint32 size, uint32 ... sizes> struct tensor<X, size, sizes...> {
        static constexpr uint32 Dimension = tensor<X, sizes...>::Dimension + 1;
        
        section<X, Dimension> Data;
        
        const tensor operator[](uint32 x) const {
            return {Data[x]};
        } 
        
        tensor operator[](uint32 x) {
            return {Data[x]};
        }
        
        tensor operator+(const tensor& t) const;
        
        tensor operator*(const X& x) const;
        
    private:
        tensor(section<X, 1> data) : Data{data} {}
        
        template <uint32 ...> struct eliminate;
        
        template <> struct eliminate<size> {
            using eliminated = tensor<X, sizes...>;
        };
        
        template <uint32 ... other_sizes>
        struct eliminate<size, other_sizes...> {
            using eliminated = typename tensor<X, sizes...>::eliminate<other_sizes...>::eliminated;
        }
        
    public:
        
        template <uint32 ... other_sizes>
        typename eliminated<other_sizes>::eliminated operator*(const tensor<size, other_sizes>& t) const;
    };
    
}

template <typename X, data::uint32 ... sizes>
inline data::math::linear::tensor<X, sizes...> operator*(const X& x, const data::math::linear::tensor<X, sizes...>& v) {
    return v * x;
}

#endif

