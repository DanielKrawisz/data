# data
wrappers and high-level programming constructs in c++

This library depends on boost, gmp and on Bartosz Milewski's implementation of Okasaki
https://github.com/BartoszMilewski/Okasaki

## data structures
  * definitions of functional lists, queues, and maps. 
  * implementations based on Okasaki. 
    * linked_list
    * functional_queue
    * rb_map

## mathematics
  * definitions of mathematical concepts such as complex numbers, rings, and modules and so on. 
  * wrappers for gmp. 

## cryptography
  * wrapper for bitcoin keys. 

## programming
  * for_each applies a function to the elements of data structure and returns a new data structure congruent to the first.
  * fold and reduce
  * simple metaprogramming constructs.
