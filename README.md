# data
Tools and high-level programming constructs in c++.

## Features 

`#include<data.hpp>` to gain all usable features. See `data.hpp` for 
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
    
# Dependencies
  * Boost 1.72
  * GMP
  * Crypto++ https://github.com/weidai11/cryptopp 
  * https://github.com/BartoszMilewski/Okasaki (included as needed)
  * Rotella's implementation of AKS. (included)
  * compile-time-regular-expressions https://github.com/hanickadot/compile-time-regular-expressions 
  * Google test

# Version 1 progress

This library is an alpha product. This is what is planned for version 1. 
  * use c++ 20 concepts to define interfaces. (See c++20 branch)
  * everything needs to be properly tested. There are tests now but they are incomplete. 
  * documentation. 
  * new features to be included. 
    * finite size signed and unsigned numbers for every size. There are versions of these types
      already but not everything about them works. See c++20 branch for overhauled versions. 
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
    * permutations
    * merge sort
    * Rabin, ElGamal, and RSA signatures. 
  * optimizatons
    * map.remove should use matt.might.net/articles/red-black-delete/
    * use NTL to hide GMP. 
    * use NTL integer exponentiation. 
    * secp256k1 should use libsecp256k1. 
  * required fixes
    * encoding/ascii.hpp, encoding/unicode.hpp, and encoding utf8.hpp are out of date
      and don't work. They should either be deleted or brought up to date and working.
