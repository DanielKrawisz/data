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

Suitable to be an element in a data structure. References are allowed. 

`template <typename Type> concept Element = std::is_constructible_v<Type, Type>;`

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

### `data/sequence.hpp`

#### `first`

#### `rest`

#### `concept data::Sequence`

A supporting `empty`, `size`, `first`, and `rest`.

#### `concept data::SequenceOf`

`SequnceOf<seq, elem>` is a `Sequence` such that `first` returns a value that can be implicitly converted to `const elem`.

### `data/stack.hpp`

#### `concept data::Stack`

A supporting `empty`, `size`, `first`, `rest`, and `prepend`.

#### `class data::stack`

An implementation of `Stack` supporting `empty`, `size`, `first`, `rest`, and `prepend`.

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
