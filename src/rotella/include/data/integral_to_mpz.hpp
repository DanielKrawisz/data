
#ifndef DATA_INTEGRAL_TO_MPZ
#define DATA_INTEGRAL_TO_MPZ

#include <concepts>
#include <gmp.h>
#include <gmpxx.h>

// solution to the problem that size_t is bigger than
// any of the built-in constructors for mpz_class.
template <std::unsigned_integral X>
requires (sizeof (std::declval<X> ()) <= sizeof (std::declval<unsigned long> ()))
mpz_class inline integral_to_mpz (X x) {
    return mpz_class (x);
}

// warning: input number may be too big. If this becomes a more general problem
// rather than something that just came up this one time, it may be
// necessary to improve the function to ensure that it uses the full number.
// right now it just chops off the biggest digits.
template <std::unsigned_integral X>
requires (sizeof (std::declval<X> ()) > sizeof (std::declval<unsigned long> ()))
mpz_class inline integral_to_mpz (X x) {
    return mpz_class (static_cast<unsigned long> (x));
}

template <std::signed_integral X>
requires (sizeof (std::declval<X> ()) <= sizeof (std::declval<long> ()))
mpz_class inline integral_to_mpz (X x) {
    return mpz_class (x);
}

// warning: input number may be too big.
template <std::signed_integral X>
requires (sizeof (std::declval<X> ()) > sizeof (std::declval<long> ()))
mpz_class inline integral_to_mpz (X x) {
    return mpz_class (static_cast<long> (x));
}

#endif
