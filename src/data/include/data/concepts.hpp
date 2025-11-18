// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONCEPTS
#define DATA_CONCEPTS

#include <type_traits>
#include <concepts>
#include <tuple>

namespace data {

    // the main purpose of this file is to create concepts for implicit and explicit conversions.
    // Sometimes I like to different types to pass on conversions from a sub-type and if I do
    // that I want to know which are explicit and which are implicit.

    namespace {
        template <typename A, typename... B>
        concept same_as_all = (std::same_as<A, B> && ...);
    }

    // check if any number of types are the same.
    template <typename... T>
    concept Same = sizeof...(T) == 0 || sizeof...(T) == 1 ||
        same_as_all<std::tuple_element_t<0, std::tuple<T...>>, T...>;

    // types containing static members that say whether one type is constructible.
    template <typename Type, typename Argument> using is_constructible = std::is_constructible<Type, Argument>;
    template <typename Type, typename Argument> inline constexpr bool is_constructible_v = std::is_constructible<Type, Argument>::value;
    template <typename Type, typename Argument> concept ConstructibleFrom = std::constructible_from<Type, Argument>;

    template <typename From, typename To> using is_implicitly_convertible = std::is_convertible<From, To>;

    template <typename From, typename To> inline constexpr bool is_implicitly_convertible_v = is_implicitly_convertible<From, To>::value;

    // A => B is B || !A

    template <typename From, typename To>
    concept ImplicitlyConvertible =
        std::is_convertible_v<From, To> /*&&
        // if both are integers, then
        //    * To is at least as big as From and has the same signedness
        //    * To is bigger than From and is signed if From is signed.
        ((!(std::integral<From> && std::integral<To>)) || (
            // if From is signed, then To must also be signed and be at least as big as To.
            ((!std::signed_integral<From>) || (std::signed_integral<To> && sizeof (To) >= sizeof (From))) &&
            // if From is unsigend, then
            ((!std::unsigned_integral<From>) || (
                std::unsigned_integral<To> && sizeof (To) >= sizeof (From) ||
                std::signed_integral<To> && sizeof (To) > sizeof (From)))

        // if From is an integer and To is a floating point, then
        )) && ((!(std::integral<From> && std::is_floating_point_v<To>)) || (sizeof (To) > sizeof (From))) &&

        // If both are floating points then To must be at least as big as From
        ((!(std::is_floating_point_v<From> && std::is_floating_point_v<To>)) || (sizeof (To) >= sizeof (From))) &&

        // no floating points to integral types.
        (!(std::is_floating_point_v<From> && std::is_integral_v<To>)) &&
        // no enums to arithmetic types.
        (!(std::is_enum_v<From> && std::is_arithmetic_v<To>))*/;

    template <typename From, typename To> concept ExplicitlyConvertible =
        !ImplicitlyConvertible<From, To> && requires (From from) {
            { To (from) };
        };

    // here convertible to means either explicit or implicit.
    template <typename From, typename To> concept Convertible =
        std::convertible_to<From, To> || requires (From from) {
            { To (from) };
        };

    template <typename Type, typename Argument>
    struct is_implicitly_constructible :
        std::bool_constant<is_constructible_v<Type, Argument> &&
        is_implicitly_convertible_v<Argument, Type>> {};

    template <typename Type, typename Argument>
    struct is_explicitly_constructible :
        std::bool_constant<is_constructible_v<Type, Argument> &&
        !is_implicitly_convertible_v<Argument, Type>> {};

    template <typename Type, typename Argument> inline constexpr bool
    is_explicitly_constructible_v = is_explicitly_constructible<Type, Argument>::value;

    template <typename Type, typename Argument> inline constexpr bool
    is_implicitly_constructible_v = is_implicitly_constructible<Type, Argument>::value;

    template <typename Type, typename Argument>
    concept explicitly_constructible_from = is_explicitly_constructible<Type, Argument>::value;

    template <typename Type, typename Argument>
    concept implicitly_constructible_from = is_implicitly_constructible<Type, Argument>::value;

    template <typename Type> concept member_function_pointer = std::is_member_function_pointer_v<Type>;

    template <typename Type> concept Reference = std::is_reference_v<Type>;

    template <typename Type> struct is_effectively_const : std::false_type {};

    template <typename Type> inline constexpr bool is_effectively_const_v = is_effectively_const<Type>::value;
    
    // && and & types are effectively const because you cannot change the 
    // reference itself. You can change the value pointed to by the reference.
    template <typename Type> struct is_effectively_const<Type &> : std::true_type {};
    template <typename Type> struct is_effectively_const<Type &&> : std::true_type {};
    template <typename Type> struct is_effectively_const<const Type> : std::true_type {};
    template <typename Type> struct is_effectively_const<Type *const> : std::true_type {};

    template <typename Type> concept Const = is_effectively_const_v<Type>;

    template <typename Type> concept Element = std::is_constructible_v<Type, Type>;

    template <typename X> using unref = std::remove_reference_t<X>;
    template <typename X> using unconst = std::remove_const_t<X>;

}

#endif
