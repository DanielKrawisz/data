// Copyright (c) 2024 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_CONCEPTS
#define DATA_CONCEPTS

#include <type_traits>
#include <concepts>

namespace data {

    // the main purpose of this file is to create concepts for implicit and explicit conversions.
    // Sometimes I like to different types to pass on conversions from a sub-type and if I do
    // that I want to know which are explicit and which are implicit.

    template <typename A, typename B> concept Same = std::same_as<A, B>;

    // types containing static members that say whether one type is constructible.
    template <typename Type, typename Argument> using is_constructible = std::is_constructible<Type, Argument>;
    template <typename Type, typename Argument> inline constexpr bool is_constructible_v = std::is_constructible<Type, Argument>::value;
    template <typename Type, typename Argument> concept constructible_from = std::constructible_from<Type, Argument>;

    template <typename From, typename To> using is_implicitly_convertible = std::is_convertible<From, To>;

    template <typename From, typename To> inline constexpr bool is_implicitly_convertible_v = is_implicitly_convertible<From, To>::value;

    template <typename From, typename To> concept ImplicitlyConvertible = std::is_convertible_v<From, To>;

    template <typename From, typename To> concept ExplicitlyConvertible =
        !is_implicitly_convertible_v<From, To> && requires (From from) {
            { To (from) };
        };

    // here convertible to means either explicit or implicit.
    template <typename From, typename To> concept Convertible =
        std::convertible_to<From, To> || requires (From from) {
            { To (from) };
        };

    template <typename Type, typename Argument>
    struct is_implicitly_constructible :
        std::bool_constant<is_constructible_v<Type, Argument> && is_implicitly_convertible_v<Argument, Type>> {};

    template <typename Type, typename Argument>
    struct is_explicitly_constructible :
        std::bool_constant<is_constructible_v<Type, Argument> && !is_implicitly_convertible_v<Argument, Type>> {};

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

    template <typename X> using unref = std::remove_reference_t<X>;
    template <typename X> using unconst = std::remove_const_t<X>;

}

#endif
