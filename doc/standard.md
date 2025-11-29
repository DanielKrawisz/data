# Data

Daniel Krawisz

## Introduction

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

## Table of Contents

* Core
* String
* Hash
* Numbers
* Crypto
* Net
* IO

## Core

A headers-only library focused on 
 * extending the functionality of certain std types
 * functional data structures
 * mathematics

The core library supports the following functional data structures:

* stack
* list
* ordered_sequence
* tree
* map
* set
* dispatch
* cycle

A set of member functions is provided to work with them, as well as functions
with the same name that can be found by argument dependent lookup. 

The core library has the following includes:

* `data/concepts.hpp`
* `data/meta.hpp`
* `data/valid.hpp`
* `data/maybe.hpp`
* `data/either.hpp`
* `data/tuple.hpp`
* `data/ordered.hpp`
* `data/sign.hpp`
* `data/abs.hpp`
* `data/math/nonzero.hpp`
* `data/divmod.hpp`
* `data/arithmetic.hpp`
* `data/math/figurate.hpp`
* `data/math/infinite.hpp`
* `data/math/modular.hpp`
* `data/size.hpp`
* `data/empty.hpp`
* `data/sequence.hpp`
* `data/fold.hpp`
* `data/iterable.hpp`
* `data/stack.hpp`
* `data/list.hpp`
* `data/ordered_sequence.hpp`
* `data/tree.hpp`
* `data/set.hpp`
* `data/map.hpp`
* `data/dispatch.hpp`
* `data/cycle.hpp`
* `data/cross.hpp`
* `data/reverse.hpp`
* `data/take.hpp`
* `data/replace.hpp`
* `data/select.hpp`
* `data/lift.hpp`
* `data/math/permutation.hpp`
* `data/math/combinatorics.hpp`
* `data/array.hpp`

### `data/concepts.hpp`

#### `concept data::Same`

`Same<X...>` is true if all types `X...` are identical.

#### `concept data::Unsame`

`Unsame<X...>` is true if all types `X...` are different.

#### `concept data::ImplicitlyConvertible`

Equivalent to `std::convertible_to`. 

#### `concept data::Convertible`

Implicitly or explicitly convertable.

#### `concept data::ExplicitlyConvertible`

For types `From` and `To`, `ExplicitlyConvertible<From, To>` is equivalent to `Convertible<From, To> && !ImplicitlyConvertible<From, To>`.

#### `concept data::Element`

Suitable as an element in a data structure. References are allowed. 

`template <typename Type> concept Element = std::is_constructible_v<Type, Type>;`

### `data/meta.hpp`

Some metaprogramming functions. 

#### `typename data::unref`

Equivalent to `std::remove_reference_t`.

#### `typename data::unconst`

Equivalent to `std::remove_const_t`.

#### `typename data::ref_to_ptr`

For a type `X`, `ref_to_ptr<X>` is a pointer preserving constness if `X` is a 
reference. Otherwise, it is unchanged. 

### `data/valid.hpp`

#### `data::valid`

`valid` takes a value of any type and returns bool. 

* For pointers, `nullptr` is `false`. Otherwise it dereferences the pointer and calls `valid` on the value. 
* For floating points, `valid` checks for `NaN` and returns `false`. Otherwise returns `true`.
* For a class `X`, `valid` looks for `X::valid const () -> bool` and calls that. Otherwise it returns `true`.

### `data/maybe.hpp`

#### `typename maybe<X>`

* Publicly extends `std::optional<X>`;
* Adds no additional members, so pointers can safely be cast back to `std::optional<X>`.
* Can hold references and void.

### `data/either.hpp`

#### `typename either<X...>`

* Publicly extends `std::variant<X...>`;
* Adds no additional members, so pointers can safely be cast back to `std::variant<X...>`.
* Can hold references and void.

### `data/tuple.hpp`

#### `void for_each`

Try to apply a function to every member of a tuple. 

#### `void apply_at`

### `data/ordered.hpp`

#### `concept data::Sortable`

A type `X`, is sortable if for values `const X a` and `const X b`, `a > b -> bool`

#### `concept data::Ordered`

Equivalent to `std::totally_ordered`. 

### `data/sign.hpp`

#### `typename data::math::sign`

#### `data::sign`

#### `data::is_zero`

#### `data::is_negative`

#### `data::is_positive`

#### `concept NumberComparable`

### `data/abs.hpp`

#### `data::abs`

#### `data::negate`

### `data/math/nonzero.hpp`

#### `struct data::math::nonzero`

#### `struct data::math::nonnegative`

### `data/math/infinite.hpp`

#### `struct data::math::signed_limit`

#### `struct data::math::unsigned_limit`

#### `struct data::math::numeric_limits`

#### `bool data::math::is_infinite`

### `data/divmod.hpp`

### `data/arithmetic.hpp`

We have two concepts of numbers, one which closely resembles 
standard c++ number types, and the other is more like what
a mathematician might expect. 

#### `data::plus`

#### `data::minus`

#### `data::times`

#### `concept WholeNumber`

Good enough for sieve of Eratosthenes. It means that we can perform all the
usual mathematical operations up to multiplication not including negation.

#### `concept RingNumber`

Good enough for Euclidian algorithm. It means that we can perform all the usual
mathematical operations up to multiplication including negation. However, 
negation does not have to return the same type. 

### `data/math/figurate.hpp`

#### `template <WholeNumber N> constexpr nonzero<N> factorial (const N &n)`

#### `template <WholeNumber N> constexpr N binomial (const N &n, const N &k)`

#### `template <WholeNumber N> constexpr N inline multichoose (const N &n, const N &r)`

#### `template <WholeNumber N> constexpr N inline polytopic_number (const N &r, const N &n)`

#### `template <WholeNumber N> constexpr N inline triangular_number (const N &n)`

#### `template <WholeNumber N> constexpr N inline tetrahedral_number (const N &n)`

#### `template <WholeNumber N> constexpr N inline pentatope_number (const N &n)`

### `data/math/modular.hpp`

### `data/size.hpp`

#### `data::size`

### `data/empty.hpp`

#### `data::empty`

### `data/sequence.hpp`

#### `concept data::Sequence`

A type `seq` satisfies `data::Sequence<seq>` if it supports `seq::size`, `seq::first`, and `seq::rest`.

A type `elem` satisfies `data::Sequence<seq, elem>` if `data::Sequence<seq>` 
and the type returned by `seq::first` can be implicitly converted to `elem`.

#### `data::drop`

Given a `data::Sequence seq` and a `size_t n`, return the same type with the first `n` elements removed. 

### `data/fold.hpp`

#### `data::fold`

#### `data::nest`

#### `data::reduce`

### `data/iterable.hpp`

#### `concept ConstIterable`

#### `concept Iterable`

`Iterable` includes `ConstIterable`.

### `data/stack.hpp`

#### `concept data::Stack`

For a type `X`, `data::Stack<X>` if `X` supports `X::empty`, `X::size`, `X::first`, `X::rest`, and `X::prepend`. 

For a value `x` of `X`, `x.prepend (x.first ()) -> X`.

For a type `elem`, `data::Stack<X, elem>` is true if `data::Stack<X>`, 
the type returned by `X::first` can be converted to `elem` and for a
type `e` of `elem`, `x.prepend (x.first ()) -> X`.

#### `class data::stack`

An implementation of `data::Stack` and `data::Iterable` supporting `valid`, 
`empty`, `size`, `first`, `rest`, and `prepend` using argument-dependent 
lookup. 

`data::stack` is optimized to be used in a pure functional way, but we allow
the user to cheat a bit with non-const access and iteration. This can be 
convenient, but it is error-prone. 

To force pure functional use, use `const data::stack<const X>`. 

For a value `z` of type `const data::stack<X>`:

#### `valid`

`valid (z) -> bool`

`valid (z)` is true iff for every element `x` of `z`, `data::valid (x)` is true. 

#### `==`

Iff `const X` has an equality operator, so does `const data::stack<X>`

#### `<<`

Iff `const X` has a function to write to an `std::ostream` via `<<` than so does `const stack<X>`.

#### `empty`

Whether the stack is empty. 

`empty (z) -> bool`

#### `size`

Return the size of the stack.

`size (z) -> size_t`

#### `first`

Return the first element of a stack. 

If `X` is a reference, `first (z) -> X`, otherwise `first (z) -> const X &`.

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

#### `stack<X>::begin` and `stack<X>::end`

`for (const X &x : z)` will iterate over the stack. Only const iteration is 
supported because since two functional lists can share memory, non-const 
iteration would be error prone.

#### `stack<X>::operator []`

For `size_t i`, return the `i`th element of the stack. 

#### `take`

For `size_t n`, take the first `n` elements of `z`. If `n > size (z)` return 
the whole stack. 

`take (z, n) -> data::list<X>`

#### `drop`

Drop the first `n` elements of `z`. If `n > size (z)` return an empty stack. 

`drop (z, n) -> data::list<X>`

#### `reverse`

Reverse a stack.

#### `join`

Join two stacks.

#### `sort`

Sort a stack. 

#### `sorted`

Whether a stack is sorted. 

#### `contains`

Whether a stack contains a given element. 

#### `remove`

#### `erase`

#### `values`

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

#### `data::reverse`

#### `class data::list`

An implementation of `data::List` supporting `empty`, `size`, `first`, `rest`, `append`, and `prepend`.

For a value `z` of type `data::list<X>`: 

#### `valid`

`valid (z) -> bool`

`valid (z)` is true iff for every element `x` of `z`, `data::valid (x)` is true. 

#### `==`

Iff `const X` has an equality operator, so does `const data::list<X>`

#### `<<`

Iff `const X` has a function to write to an `std::ostream` via `<<` than so does `const list<X>`.

#### `empty`

Whether the list is empty. 

`empty (z) -> bool`

#### `size`

Return the size of the list.

`size (z) -> size_t`

#### `first`

Return the first element of a list. 

If `X` is a reference, `first (z) -> X`, otherwise `first (z) -> const X &`.

#### `rest`

Return the rest of the list after the first element.

`rest (z) -> data::list<X>`

#### `prepend`

Return the list with a new element prepended.

`prepend (z, a) -> data::list<X>`

#### `>>`

Return the list with a new element prepended.

`z >> a -> data::list<X>`

#### `append`

Return the list with a new element appended.

`append (z, a) -> data::list<X>`

#### `<<`

Return the list with a new element appended.

`z << a -> data::list<X>`

#### `stack<X>::begin` and `stack<X>::end`

`for (const X &x : z)` will iterate over the stack. Only const iteration is 
supported because since two lists can share memory, non-const iteration would
be error prone.

#### `take`

`take (z, n) -> data::list<X>`

#### `drop`

`drop (z, n) -> data::list<X>`

#### `reverse`

Reverse a stack.

#### `join`

Join two stacks.

#### `sort`

Sort a stack. 

#### `sorted`

Whether a stack is sorted. 

#### `rotate_right`

#### `rotate_left`

#### `contains`

#### `remove`

#### `erase`

#### `values`

### `data/ordered_sequence.hpp`

#### `class data::ordered_sequence`

#### `==`

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

#### `operator |`

Same as `merge`.

#### `operator &`

#### `operator ^`

#### `sort`

#### `sorted`

#### `contains`

#### `remove`

#### `erase`

#### `values`

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

### `data/map.hpp`

#### `class data::map`

### `data/dispatch.hpp`

#### `class data::dispatch`

### `data/cycle.hpp`

### `data/math/permutation.hpp`

### `data/cross.hpp`

#### `struct cross`

Cross (for cross product) is an iterable fixed-size container of any size. 
Cross resembles a vector with the ability to resize removed. 

### `data/math/combinatorics.hpp`

### `data/array.hpp`

## Bytes

A library for working with sequences of data. 

Requires compilation, no dependencies other than the standard library.

NOTE: right now we actually use boost for some things but we can replace all of it easily. 

* `data/slice.hpp`
* `data/stream.hpp`
* `data/string.hpp`
* `data/encoding/hex.hpp`
* `data/encoding/base64.hpp`
* `data/encoding/endian.hpp`
* `data/encoding/integer.hpp`
* `data/bytes.hpp`
* `data/integral.hpp`
* `data/random.hpp`
* `data/async.hpp`

### `data/string.hpp`

#### `typename data::string`

### `data/bytes.hpp`

#### `typename data::bytes`

Just like `std::string` except that when you print it, `"` are included as delimiters. 

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

#### `typename data::uint256`

#### `typename data::int256`

#### `typename data::uint256_big`

#### `typename data::int256_big`

#### `typename data::uint256_little`

#### `typename data::int256_little`

## Hash

A library providing hash functions, including cryptographic hash functions.

### data/hash.hpp

Provides a set of hash functions from crypto++. 

## Numbers

Big numbers library using GMP and crypto++

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
