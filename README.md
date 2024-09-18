# data
Tools and high-level programming constructs in c++.

## Features 

`#include <data\data.hpp>` to gain all usable features. See `data.hpp` for 
how to use all features. 

  * Functional data structures
    * stack
    * queue
    * list
    * tree
    * map
    * set
  * Functional programming
    * reduce
    * fold
    * for_each
    * map_thread
  * Mathematical types
    * natural numbers
    * integers
    * rational numbers
    * complex rationals
    * rational quaternions
    * rational octonions
    * prime fields
    * polynomials
    * permutations
    * AKS algorithm
  * Encodings
    * hex
    * base58
    * base64
  * cryptography 
    * AES
    * SHA
    * RIPEMD160
    * NIST_DRBG 
  * concurrency
    * golang-style communication channel. 
  * networking in a functional style.
    * URLs
    * TCP
    * HTTP
    * Websockets
    
# Dependencies
  * Boost 1.80
  * GMP
  * PEGTL
  * Crypto++ https://github.com/weidai11/cryptopp 
  * OpenSSL
  * secp256k1
  * https://github.com/BartoszMilewski/Okasaki (included as needed)
  * Rotella's implementation of AKS. (included)
  * compile-time-regular-expressions https://github.com/hanickadot/compile-time-regular-expressions (included)
  * Google test

# Requirements
  * GCC v13
  * Boost 
  * OpenSSL 
  * Crypto++
  * GMP 
  * SECP256k1

# Build instructions

  * `cmake -B build -S .`
  * `cmake --build build`
  * `cmake --install build`

# Version 1 progress

This library is an alpha product. This is what is planned for version 1.
  * documentation. 
  * new features to be included. 
    * list works more like Mathematica's List. 
      * inner
      * outer
      * flatten
      * riffle
      * partition
    * minimal polynomials using NTL. 
    * finite fields. Right now we do prime fields but we need to be able to generate minimal
      polynomials for arbitrary finite fields. 
    * elliptic curves using NTL. 
    * Rabin, ElGamal, and RSA signatures. 
  * optimizatons
    * map.remove should use matt.might.net/articles/red-black-delete/
    * use NTL to hide GMP. 
    * use NTL integer exponentiation. 
  * networking
    * websockets
    * run external programs
