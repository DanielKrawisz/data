# Data

Daniel Krawisz

## General Considerations

* Concepts and public member variables are written `LikeThis`. 
* Types, functions, and member functions are written `like_this`.
* Namespaces may have any format style.

We use `c++23`.

## Functional Programming

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

### `data/ordered.hpp`

#### `concept data::Sortable`

A type `X`, is sortable if for values `const X a` and `const X b`, `a > b -> bool`

#### `concept data::Ordered`

Equivalent to `std::totally_ordered`. 

### `data/valid.hpp`

#### `data::valid`

### `data/sequence.hpp`

#### `concept data::Sequence`

#### `concept data::SequenceOf`

### `data/stack.hpp`

#### `concept data::Stack`

#### `class data::stack`

A stack of values supporting `size`, `first`, `rest`, and `prepend`.

For a value `z` of type `stack<X>`.: 

`for (X &x : z)` or `for (const X &x z)` will iterate over the stack. 

`z.valid ()` is true iff for every element `x` of `z`, `valid (x)` is true. 

#### `==`

`stack<X>` supports an equality operator iff `X` does. 

#### `empty`

`empty (x) -> ImplicitlyConvertible<bool>`

#### `size`

Return the size of the list.

`size (z) -> ImplicitlyConvertible<size_t>`

#### `first`

Return the first element of `z`. 

#### `rest`

Return the rest of the sack after the first element.

#### `prepend`

#### `take`

For a `size_t n`, take the first `n` elements of `z`. 

If `n > size (z)` return the whole stack. 

#### `drop`

For a `size_t n`, drop the first `n` elements of `z`. 

If `n > size (z)` return an empty stack. 

#### `reverse`

### `data/list.hpp`

#### `concept data::Queue`

#### `concept data::List`

#### `class data::list`

## Mathematics

### `data/math/infinite.hpp`
