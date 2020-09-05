# data
Tools and high-level programming constructs in c++.

## Features 

`#include<data.hpp>` to gain all usable features. See `data.hpp` for 
how to use all features. 

  * Functional data structures
    * stack
    * queue
    * tree
    * map
    * set
  * Functional programming
    * reduce
    * fold
    * for_each
  * Mathematical types
    * natural numbers
    * integers
    * rational numbers
    * complex rationals
    * rational quaternions
    * rational octonions
    * prime fields
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
    
# Dependencies
  * Boost 1.72
  * GMP
  * NTL
  * Crypto++ https://github.com/weidai11/cryptopp 
  * https://github.com/BartoszMilewski/Okasaki (included as needed)
  * Rotella's implementation of AKS. (included)
  * compile-time-regular-expressions https://github.com/hanickadot/compile-time-regular-expressions 
  * Google test

# Version 1 progress

This library is an alpha product. This is what is planned for version 1. 
  * New Features
    * finite size signed and unsigned numbers for every size. 
    * minimal polynomials using NTL. 
    * finite fields
    * elliptic curves using NTL. 
    * permutations
    * merge sort
  * optimizatons
    * map.remove should use matt.might.net/articles/red-black-delete/
    * use NTL to hide GMP. 
    * use NTL integer exponentiation. 
    * rational should be template-specialized to use GMP. 
    * secp256k1 should use libsecp256k1. 
  * use c++ 20 concepts to define interfaces. (see branch concepts)
  * everything needs to be properly tested. There are tests now but they are inadequate. 
