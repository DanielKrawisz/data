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

We use conan 2 for dependency management. All dependencies are included in the repo or are
available through conan 2 other than PEGTL, which must be installed separately.

secp256k1 must be added to a local conan repo using https://github.com/Gigamonkey-BSV/secp256k1-conan-recipe

# Requirements
  * GCC v13
  * Boost 
  * OpenSSL 
  * Crypto++
  * GMP 
  * SECP256k1

# Build instructions

  * `mkdir build`
  * `cd build`
  * `cmake .. -DCMAKE_BUILD_TYPE=Release`
  * `cmake --build .`

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
