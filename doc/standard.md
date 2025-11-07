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

## Functional Data Structures

A headers-only library focused on functional data structures.

The data library supports the following data structures:

* stack
* list
* ordered_sequence
* tree
* map
* set
* dispatch
* permutation

A set of member functions is provided to work with them, as well as functions
with the same name that can be found by argument dependent lookup. 

The functional data structures library has the following includes:

* `data/concepts.hpp`
* `data/ordered.hpp`
* `data/valid.hpp`
* `data/maybe.hpp`
* `data/sequence.hpp`
* `data/stack.hpp`
* `data/list.hpp`
* `data/ordered_sequence.hpp`
* `data/tree.hpp`
* `data/set.hpp`
* `data/map.hpp`
* `data/dispatch.hpp`
* `data/math/permutation.hpp`
<!--
* `data/transpose.hpp`
* `data/map_thread`
* `data/cycle.hpp`
* `data/container.hpp`
* `data/remove.hpp`
* `data/erase.hpp`
* `data/select.hpp`
* `data/replace.hpp`
* `data/for_each.hpp`
-->

### `data/concepts.hpp`

#### `concept data::Same`

Equivalant to `std::same_as`.

#### `concept data::ImplicitlyConvertible`

Equivalent to `std::convertible_to`. 
<!--
For types `From` and `To`, `ImplicitlyConvertible<From, To>` if `From` is implicitly convertible to `To` excluding narrowing conversions. 
-->
#### `concept data::Convertible`

Implicitly or explicitly convertable.

#### `concept data::ExplicitlyConvertible`

For types `From` and `To`, `ExplicitlyConvertible<From, To>` is equivalent to `Convertible<From, To> && !ImplicitlyConvertible<From, To>`.

#### `concept data::Element`

Suitable as an element in a data structure. References are allowed. 

`template <typename Type> concept Element = std::is_constructible_v<Type, Type>;`

#### `typename data::unref`

Equivalent to `std::remove_reference_t`.

#### `typename data::unconst`

Equivalent to `std::remove_const_t`.

### `data/ordered.hpp`

#### `concept data::Sortable`

A type `X`, is sortable if for values `const X a` and `const X b`, `a > b -> bool`

#### `concept data::Ordered`

Equivalent to `std::totally_ordered`. 

### `data/valid.hpp`

#### `valid`

#### `data::valid`

`valid` takes a value of any type and returns bool. 

* For pointers, `nullptr` is `false`. Otherwise it dereferences the pointer and calls `valid` on the value. 
* For floating points, `valid` checks for `NaN` and returns `false`. Otherwise returns `true`.
* For a class `X`, `valid` looks for `X::valid const () -> bool` and calls that. Otherwise it returns `true`.

### `data/maybe.hpp`

#### `typename maybe<X>`

### `data/either.hpp`

#### `typename either<X...>`

### `data/sequence.hpp`

#### `concept data::Sequence`

A type `seq` satisfies `data::Sequence<seq>` if it supports `seq::size`, `seq::first`, and `seq::rest`.

A type `elem` satisfies `data::Sequence<seq, elem>` if `data::Sequence<seq>` 
and the type returned by `seq::first` can be implicitly converted to `elem`.

#### `data::drop`

### `data/stack.hpp`

#### `concept data::Stack`

Available from `data/stack.hpp`, `data/list.hpp`, `data/ordered_sequence.hpp`

For a type `X`, `data::Stack<X>` if `X` supports `X::empty`, `X::size`, `X::first`, `X::rest`, and `X::prepend`. 

For a value `x` of `X`, `x.prepend (x.first ()) -> X`.

For a type `elem`, `data::Stack<X, elem>` is true if `data::Stack<X>`, 
the type returned by `X::first` can be converted to `elem` and for a
type `e` of `elem`, `x.prepend (x.first ()) -> X`.

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

Return the rest of the stack after the first element.

`rest (z) -> data::stack<X>`

#### `prepend`

Return the stack with a new element prepended.

For a value `a` of type `elem`, 

`prepend (z, a) -> data::stack<X>`.

#### `>>`

Return the stack with a new element prepended.

`z >> a -> data::stack<X>`

#### `take`

For `size_t n`, take the first `n` elements of `z`. 

If `n > size (z)` return the whole stack. 

#### `drop`

For `size_t n`, drop the first `n` elements of `z`. 

If `n > size (z)` return an empty stack. 

#### `reverse`

Reverse a stack.

#### `join`

Join two stacks.

#### `sort`

Sort a stack. 

#### `sorted`

#### `values`

#### `contains`

#### `remove`

#### `erase`

### `data/list.hpp`

#### `concept data::Queue`

For a type `X`, `data::Queue<X>` if `X` supports `X::empty`, `X::size`, `X::first`, `X::rest`, and `X::append`. 

For a value `x` of `X`, `x.append (x.first ()) -> X`.

For a type `elem`, `data::Queue<X, elem>` is true if `data::Queue<X>`, 
the type returned by `X::first` can be converted to `elem` and for a
type `e` of `elem`, `x.append (x.first ()) -> X`.

#### `concept data::List`

For a type `X`, `data::List<X>` if `data::Stack<X> && data::Queue<X>`. 

#### `data::take`

A `List` is both a `Stack` and a `Queue`. 

#### `data::reverse`

#### `class data::list`

An implementation of `data::List` supporting `empty`, `size`, `first`, `rest`, `append`, and `prepend`.

For a value `z` of type `data::list<X>`: 

`for (X &x : z)` or `for (const X &x z)` will iterate over the stack. 

`z.valid ()` is true iff for every element `x` of `z`, `valid (x)` is true. 

#### `<<`

Iff `X` has a function to write to an `std::ostream` via `<<` than so does `list<X>`.

#### `empty`

#### `size`

#### `first`

#### `rest`

#### `prepend`

#### `append`

#### `take`

#### `drop`

#### `reverse`

#### `rotate_right`

#### `rotate_left`

#### `join`

#### `sort`

#### `sorted`

#### `values`

#### `contains`

#### `remove`

#### `erase`

### `data/ordered_sequence.hpp`

#### `class data::ordered_sequence`

#### `<<`

Iff `X` has a function to write to an `std::ostream` via `<<` than so does `list<X>`.

#### `empty`

#### `size`

#### `first`

#### `rest`

#### `insert`

#### `take`

#### `drop`

#### `merge`

#### `sort`

#### `sorted`

#### `values`

#### `contains`

#### `remove`

#### `erase`

### `data/priority_queue.hpp`

#### `class data::priority_queue`

#### `<<`

#### `empty`

#### `size`

#### `first`

#### `rest`

#### `insert`

#### `merge`

#### `sort`

#### `sorted`

#### `values`

#### `contains`

#### `remove`

#### `erase`

### `data/tree.hpp`

#### `class data::tree`

#### `root`

#### `right`

#### `left`

#### `contains`

### `data/map.hpp`

#### `class data::map`

#### `insert`

#### `contains`

### `data/set.hpp`

#### `class data::set`

### `data/math/permutation.hpp`

## Mathematics

A headers-only library for mathematical structures.

* `data/math/infinite.hpp`
* `data/math/nonzero.hpp`
* `data/math/figurate.hpp`
* `data/math/combinatorics.hpp`
* `data/integral.hpp`
* `data/math/extended_euclidian.hpp`
* `data/math/fraction.hpp`
* `data/math/polynomial.hpp`
* `data/math/complex.hpp`
* `data/math/quaternion.hpp`
* `data/math/octonion.hpp`
* `data/math/prime.hpp`
* `data/math/primitive_root.hpp`
* `data/math/modular.hpp`
* `data/math/algebra/finite_field.hpp`

### `data/math/infinite.hpp`

#### `struct data::math::signed_limit`

#### `struct data::math::unsigned_limit`

#### `struct data::math::numeric_limits`

### `data/math/nonzero.hpp`

#### `struct data::math::nonzero`

#### `struct data::math::nonnegative`

### `data/math/figurate.hpp`

### `data/math/combinatorics.hpp`

### `data/integral.hpp`

### `data/math/extended_euclidian.hpp`

### `data/math/fraction.hpp`

### `data/math/polynomial.hpp`

### `data/math/complex.hpp`

### `data/math/quaternion.hpp`

### `data/math/octonion.hpp`

### `data/math/prime.hpp`

### `data/math/primitive_root.hpp`

### `data/math/modular.hpp`

### `data/math/algebra/finite_field.hpp`

## String

* `data/stream.hpp`
* `data/encoding/endian.hpp`
* `data/cross.hpp`
* `data/array.hpp`
* `data/bytes.hpp`

### `data/cross.hpp`

### `data/array.hpp`

### `data/bytes.hpp`

#### `typename data::bytes`

### `data/string.hpp`

#### `typename data::string`

Just like `std::string` except that when you print it, `"` are included as delimiters. 
<!--
### `data/integral.hpp`

Numbers that work just like built-in types but with more sizes. 
#### `typename data::byte`

#### `typename data::uint8`

#### `typename data::int8`

#### `typename data::uint16`

#### `typename data::int16`

#### `typename data::uint16_big`

#### `typename data::int16_big`

#### `typename data::uint16_little`

#### `typename data::int16_little`

#### `typename data::uint32`

#### `typename data::int32`

#### `typename data::uint32_big`

#### `typename data::int32_big`

#### `typename data::uint32_little`

#### `typename data::int32_little`

#### `typename data::uint64`

#### `typename data::int64`

#### `typename data::uint64_big`

#### `typename data::int64_big`

#### `typename data::uint64_little`

#### `typename data::int64_little`
#### `typename data::uint128`

#### `typename data::int128`

#### `typename data::uint128_big`

#### `typename data::int128_big`

#### `typename data::uint128_little`

#### `typename data::int128_little`

#### `typename data::uint160`

#### `typename data::int160`

#### `typename data::uint160_big`

#### `typename data::int160_big`

#### `typename data::uint160_little`

#### `typename data::int160_little`

-->
## Hash

A library providing a number of cryptographic hash functions. 

### data/hash.hpp

Provides a set of hash functions from crypto++. 

## Numbers

Big numbers library that uses GMP and crypto++

* `data/random.hpp`
* `data/numbers.hpp`
* `data/math/factor.hpp`
* `data/math/random_prime.hpp`

## Crypto

## Net

### `data/async.hpp`

#### `typename awaitable<X>`

Same as `asio::awaitable`.

#### `concept Awaitable<X>`

Satisfied by any awaitable type.

#### `synced`

Run a coroutine, wait for it to complete, and return the result.
