# Data

Daniel Krawisz

## General Considerations

* Concepts and public member variables are written `LikeThis`. 
* Types, functions, and member functions are written `like_this`.
* Namespaces may have any format style.

We use `c++23`.

## Functional Programming

### `data/concepts.hpp`

#### `concept Same`

Equivalant to `std::same_as`.

#### `concept ImplicitlyConvertible`

Eqivalent to `std::convertible_to`.

#### `concept Convertible`

Implicitly or explicitly convertable.

#### `concept ExplicitlyConvertible`

Explicitly convertible but not implicitly convertable.

### `data/ordered.hpp`

#### `concept Sortable`

A type `X`, is sortable if for values `const X a` and `const X b`, `a > b -> bool`

#### `concept Ordered`

Equivalent to `std::totally_ordered`. 

### `data/valid.hpp`

#### `valid`

### `data/sequence.hpp`

#### `concept Sequence`

#### `concept SequenceOf`

### `data/stack.hpp`

### `concept Stack`

### `class stack`

## Mathematics

### `data/math/infinite.hpp`

