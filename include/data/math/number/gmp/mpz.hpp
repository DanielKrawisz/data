// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_MATH_NUMBER_GMP_MPZ
#define DATA_MATH_NUMBER_GMP_MPZ

#include <gmp.h>
#include <data/types.hpp>
#include <data/math/sign.hpp>
#include <data/math/number/natural.hpp>
#include <data/io/unimplemented.hpp>

namespace data::math::number::gmp {
                
                typedef mp_limb_t gmp_uint;
                typedef mp_limb_signed_t gmp_int;
                
                const __mpz_struct MPZInvalid = __mpz_struct{0, 0, nullptr};
                
                inline bool equal(const __mpz_struct& a, const __mpz_struct& b) {
                    return a._mp_alloc == b._mp_alloc && a._mp_size == b._mp_size && a._mp_d == b._mp_d;
                }
                
                inline uint32 size(const __mpz_struct& a) {
                    return a._mp_alloc;
                }
                
                inline bool valid(const __mpz_struct& mpz) {
                    return mpz._mp_d != nullptr;
                }
                
                inline math::sign sign(const __mpz_struct& mpz) {
                    return !valid(mpz) ? zero : mpz._mp_size < 0 ? negative : positive;
                }
                
                void swap(__mpz_struct& a, __mpz_struct& b);
                
                __mpz_struct read_decimal(const string&);
                
                __mpz_struct read_hexidecimal(const string&);
                
                inline __mpz_struct read_string(const string& s) {
                    __mpz_struct x = read_decimal(s);
                    if (!valid(x)) return read_hexidecimal(s);
                }
                
                struct mpz {
                    __mpz_struct MPZ;
                    
                    mpz() : MPZ{MPZInvalid} {}
                    
                    bool valid() const {
                        return gmp::valid(MPZ);
                    }
                    
                    virtual ~mpz() {
                        if (valid()) mpz_clear(&MPZ);
                    }
                    
                    mpz(gmp_uint n) {
                        mpz_init_set_ui(&MPZ, n);
                    }
                    
                    mpz(gmp_int n) {
                        mpz_init_set_si(&MPZ, n);
                    }
                    
                    mpz(uint32 n) : mpz{} {
                        if (n <= std::numeric_limits<uint32>::max()) mpz_init_set_ui(&MPZ, n);
                    }
                    
                    mpz(int32 z) : mpz{} {
                        if (z <= std::numeric_limits<int32>::max() && z >= std::numeric_limits<int32>::min()) mpz_init_set_si(&MPZ, z);
                    }
                    
                    mpz(const __mpz_struct& n) {
                        mpz_init(&MPZ);
                        mpz_set(&MPZ, &n);
                    }
                    
                    mpz(__mpz_struct&& n) {
                        swap(MPZ, n);
                    }
                    
                    mpz(const mpz& n) {
                        mpz_init(&MPZ);
                        mpz_set(&MPZ, &n.MPZ);
                    }
                    
                    mpz(mpz&& n) {
                        swap(MPZ, n.MPZ);
                    }
                    
                    mpz& operator=(mpz& n) {
                        mpz_set(&MPZ, &n.MPZ);
                        return *this;
                    }
                    
                    mpz(string& x) : MPZ{read_string(x)} {}
                    
                    math::sign sign() const {
                        return gmp::sign(MPZ);
                    }
                    
                    size_t size() const {
                        return gmp::size(MPZ);
                    }
                    
                    mp_limb_t* begin() {
                        return MPZ._mp_d;
                    }
                    
                    mp_limb_t* end() {
                        return MPZ._mp_d + MPZ._mp_alloc;
                    }
                    
                    const mp_limb_t* begin() const {
                        return MPZ._mp_d;
                    }
                    
                    const mp_limb_t* end() const {
                        return MPZ._mp_d + MPZ._mp_alloc;
                    };
                    
                    bool operator==(gmp_uint n) const;
                    
                    bool operator==(mpz& n) const;
                    
                    bool operator<(gmp_uint n) const;
                    
                    bool operator<(mpz& n) const;
                    
                    bool operator>(gmp_uint n) const;
                    
                    bool operator>(mpz& n) const;
                    
                    bool operator<=(gmp_uint n) const;
                    
                    bool operator<=(mpz& n) const;
                    
                    bool operator>=(gmp_uint n) const;
                    
                    bool operator>=(mpz& n) const;
                    
                    // TODO
                    bool operator<(uint32 n) const;
                    
                    bool operator>(uint32 n) const;
                    
                    bool operator<=(uint32 n) const;
                    
                    bool operator>=(uint32 n) const;
                };
                
            }

#endif
