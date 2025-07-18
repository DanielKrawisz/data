# Data

Daniel Krawisz

## General Considerations

This document describes a standard for the `data` library. 

Only the symbols specifically described in this document are 
required to remain consistent between minor versions and obey
naming convensiontions. 

We use `c++23`.

All symbols standardized in this document are within namespace 
`data`. If the namespace is not written out explicitly, that 
indicates that the symbol can be found by argument-dependent
lookup. 

Naming conventions:
* Concepts and public member variables are written `LikeThis`. 
* Types, functions, and member functions are written `like_this`.
* Namespaces may have any format style.

## Basics

### `data/concepts.hpp`

#### `concept data::Same`

Equivalant to `std::same_as`.

#### `concept ImplicitlyConvertible`

Eqivalent to `std::convertible_to`.

#### `concept data::Convertible`

Implicitly or explicitly convertable.

#### `concept data::ExplicitlyConvertible`

Explicitly convertible but not implicitly convertable.

#### `concept data::Element`

Suitable to be an element in a data structure. References are allowed. 

`template <typename Type> concept Element = std::is_constructible_v<Type, Type>;`

#### `typename unref`

Equivalent to `std::remove_reference_t`.

#### `typename unconst`

Equivalent to `std::remove_const_t`.

### `data/ordered.hpp`

#### `concept data::Sortable`

A type `X`, is sortable if for values `const X a` and `const X b`, `a > b -> bool`

#### `concept data::Ordered`

Equivalent to `std::totally_ordered`. 

### `data/valid.hpp`

#### `data::valid`

`valid` takes a value of any type and returns bool. 

* For pointers, `nullptr` is `false`. Otherwise it dereferences the pointer and calls `valid` on the value. 
* For a class `X`, `valid` looks for `X::valid const () -> bool` and calls that. Otherwise it returns `true`.
* For floating points, `valid` checks for `NaN` and returns `false`. Otherwise returns `true`.

## Functional Data Structures

The data library supports three data structures:

* stack
* list
* ordered_sequence

A set of member functions is provided to work with them, as well as functions
with the same name that can be found by argument dependent lookup. 

The functional data structures library has the following includes:

* `data/sequence.hpp`
* `data/stack.hpp`
* `data/list.hpp`
* `data/ordered_sequence.hpp`
<!--
* `data/transpose.hpp`
* `data/map_thread`
* `data/cycle.hpp`
* `data/tree.hpp`
* `data/container.hpp`
* `data/set.hpp`
* `data/map.hpp`
* `data/remove.hpp`
* `data/erase.hpp`
* `data/select.hpp`
* `data/replace.hpp`
* `data/for_each.hpp`
-->

### `data/sequence.hpp`

#### `concept data::Sequence`

A type `seq` is a `data::Sequence` if it supports `seq::size`, `seq::first`, and `seq::rest`.

#### `concept data::SequenceOf`

#### `concept data::Stack`

Defined in `data/stack.hpp`, `data/list.hpp`, `data/ordered_sequence.hpp`

A type `X` supporting `X::empty`, `X::size`, `X::first`, `X::rest`, and `X::prepend`. 

For a value `x` of `X`, `x.prepend (x.first ()) -> X`.

#### `data::drop`

#### `concept data::Queue`

#### `concept data::List`

#### `data::take`

A `List` is both a `Stack` and a `Queue`. 

#### `data::reverse`

### `data/stack.hpp`

A `Stack` is a `Sequence`.

#### `class data::stack`

An implementation of `Stack` supporting `valid`, `empty`, `size`, `first`, `rest`, and `prepend` using argument-dependent lookup. 

For a value `z` of type `const data::stack<X>`: 

`for (X &x : z)` or `for (const X &x z)` will iterate over the stack. 

#### `valid`

`valid (z)` is true iff for every element `x` of `z`, `data::valid (x)` is true. 

`valid (x) -> bool`

#### `==`

Iff `X` has an equality operator, so does `data::stack<X>`

#### `<<`

Iff `X` has a function to write to an `std::ostream` via `<<` than so does `stack<X>`.

#### `empty`

Whether the stack is empty. 

`empty (x) -> bool`

#### `size`

Return the size of the list.

`size (z) -> size_t`

#### `first`

Return the first element of `z`. 

If `X` is a reference, `first (z) -> X`, otherwise `first (z) -> X &`.

For an element `zz` of type `data::stack<X>`

If `X` is a reference, `first (zz) -> X`, otherwise `first (z) -> const X &`.

#### `rest`

Return the rest of the sack after the first element.

`rest (z) -> data::stack<X>`

#### `prepend`

Return the stack with a new element prepended.

`prepend (z) -> data::stack<X>`

#### `take`

For a `size_t n`, take the first `n` elements of `z`. 

If `n > size (z)` return the whole stack. 

#### `drop`

For a `size_t n`, drop the first `n` elements of `z`. 

If `n > size (z)` return an empty stack. 

#### `reverse`

#### `sort`

#### `sorted`

### `data/list.hpp`

#### `class data::list`

An implementation of `data::List` supporting `empty`, `size`, `first`, `rest`, `append`, and `prepend`.

For a value `z` of type `data::list<X>`: 

`for (X &x : z)` or `for (const X &x z)` will iterate over the stack. 

`z.valid ()` is true iff for every element `x` of `z`, `valid (x)` is true. 

#### `<<`

Iff `X` has a function to write to an `std::ostream` via `<<` than so does `list<X>`.

### `data/ordered_sequence.hpp`
<!-- 
## Mathematics

### `data/math/infinite.hpp`

### `data/abs.hpp`

### `data/norm.hpp`

### `data/math/integral.hpp`

Numbers that work the same as built-in integral types but with more sizes. 
-->
