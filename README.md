# data

Tools and high-level programming constructs in c++, including 
functional data structures, higher-order functions, big numbers, and mathematics. 

See [the standard](doc/standard.md) for all supported features and how to use them. 
The standard will be gradually improved as we work toward a beta version.

Current version is 0.1 beta.

## Features 

  * Functional data structures
    * stack
    * queue
    * list
    * tree
    * map
    * set
    * priority queue
  * Functional programming
    * reduce
    * fold
    * for_each
    * lift
  * Mathematical types
    * signed and unsigned int types of every size.
    * natural numbers
    * integers
    * rational numbers
    * Caylay-Dickson rationals (complex, quaternion, and octonion)
    * prime fields
    * polynomials
    * permutations
    * AKS algorithm
  * Encodings
    * hex
    * base58
    * base64
    
# Dependencies
  * Boost 1.86
  * GMP
  * compile-time-regular-expressions https://github.com/hanickadot/compile-time-regular-expressions (included)
  * Google test (only for tests)

# Requirements
  * GCC v13+
  * Boost 
  * OpenSSL 
  * Crypto++
  * GMP 
  * SECP256k1

# Build instructions

  * `cmale -B build -S .`
  * `cmake --build build`
  * `cmake --install build`

# For version 1:

This library is an alpha product. This is what is planned for version 1.
  * functional RB map algorithms
    * remove
    * merge
    * interction
    * complements
  * list works more like Mathematica's List. 
    * inner
    * outer
    * partition
  * use size_in_base in the division algorithm.
  * Polynomial algorithms
    * minimal polynomials.
    * irreducible polynomial.
  * finite fields. 
    * Generate irreducible polynomials for arbitrary finite fields. 
  * geometry
    * Affine space using real, complex, and quaternionic numbers.
    * Euclidian space
    * Projective space
    * Hyperbolic space
    * infinite classes of compact Lie Groups (no exceptionals for now)
  * Async: golang style channel
  * crypto
    * Better definitions of cryptographic algorithms, like hash functions, MACs, and block ciphers. 
    * More block cipher modes
    * Stream ciphers, including block cyphers as stream ciphers.
    * Elliptic curves (SEC and other standard curves)
    * Rabin, ElGamal, and RSA signatures. 
  * networking
    * websockets
    * run external programs
    * thread safe (multithreaded async lock)
  * separate crypto and networking into separate libraries. 
