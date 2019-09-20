# data
Wappers and high-level programming constructs in c++

## Introduction
c++ has been unfairly maligned as a low-level programming language
when in reality it gives the programmer tools to program at both a
low- and a high-level and it is superior as a high-level language
to most which have come after it. It is more challenging to learn
than other popular programming languages, but it also puts more
power at the programmer's fingertips, once he has mastered it. 

Languages which claim to be simple and easy to learn cause problems
down the line because ultimately the biggest challenges of
programming are those caused messes which programmers have created.
No language, no matter how simple, can protect against a
programmer's own inexperience. c++ provides tools that an 
experienced programmer can use to tame the complexity of a large
project as it grows. 

The purpose of this library is to provide high-level tools for
c++ that let me program in ways that I have enjoyed in other
languages. 

This library depends on boost, gmp, crypto++, libbitcoin, and on Bartosz Milewski's implementation of Okasaki
https://github.com/BartoszMilewski/Okasaki

## data structures
  * Definitions of functional lists, queues, and maps. 
  * Implementations based on Okasaki. 
    * linked_list
    * functional_queue
    * rb_map

## mathematics
  * Definitions of mathematical concepts.
    * Natural numbers
    * Integers
    * Rationals
    * Complex rationals
    * Algebraic concepts such as groups, rings, fields, modules, and so on. 
  * Implementatations using gmp. 

## cryptography
  * wrappers for bitcoin cryptography from libbitcoin and crypto++
    * secp256k1
    * sha256
    * sha512
    * ripemd160

## programming
  * for_each applies a function to the elements of data structure and returns a new data structure congruent to the first.
  * fold and reduce

## concurrency
  * golang-style communication channel. 
