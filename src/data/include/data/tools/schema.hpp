
#ifndef DATA_TOOLS_SCHEMA
#define DATA_TOOLS_SCHEMA
/**
 * @file schema.hpp
 *
 * Facilities for validating and decoding map-like string→string data
 * structures (e.g. URL query parameters, form data, headers).
 *
 * A schema is expressed as a composable *rule*. Given a map and a rule,
 * validation both checks structural correctness and decodes values into
 * typed results.
 *
 * The core operation is:
 *
 *     validate (data::map<std::string, std::string>, rule)
 *
 * If validation succeeds, a value is returned whose type is determined
 * entirely by the rule. If validation fails, an error is produced.
 *
 * TODO: this should also work on std::map but it doesn't yet.
 *
 * ---------------------------------------------------------------------
 * Mental model
 * ---------------------------------------------------------------------
 *
 * A rule is best understood as a *typed transducer*:
 *
 *     map<string, string>  →  typed C++ value
 *
 * Validation is not just a boolean check. If validation succeeds, the
 * rule *produces a value* whose type is determined entirely by the rule
 * expression itself.
 *
 * Conceptually:
 *
 *     input map
 *         |
 *         v
 *     [ rule expression ]
 *         |
 *         v
 *     decoded, strongly-typed result
 *
 * Rules can be composed to form larger schemas. Composition determines
 * both the structural constraints on the map and the structure of the
 * resulting C++ type.
 *
 * For example:
 *
 *     key<int> ("x") && key<int> ("y")
 *
 * can be visualized as:
 *
 *     key<int> ("x")        key<int> ("y")
 *          |                      |
 *          +--------- && ---------+
 *                     |
 *                     v
 *                tuple<int, int>
 *
 * In this sense, rule operators are not merely logical combinators:
 *
 *   • &&   constructs product types (tuples)
 *   • ||   constructs sum types (variants)
 *   • *    constructs optional types
 *   • -    enforces closed-world constraints on accepted keys
 *
 * The intent is that schemas can be read declaratively while remaining
 * fully type-safe, with result types deduced at compile time from the
 * rule expression.
 *
 * ---------------------------------------------------------------------
 * Design notes
 * ---------------------------------------------------------------------
 *
 * • Rules describe *both* structure and decoding.
 * • Result types are determined statically by the rule expression.
 * • Composition preserves meaning: schemas can be built bottom-up.
 * • No implicit acceptance of unknown keys unless explicitly allowed.
 *
 * This allows map-like inputs (such as URL queries) to be validated
 * declaratively while remaining fully type-safe.
 *
 * ---------------------------------------------------------------------
 * Basic rules
 * ---------------------------------------------------------------------
 *
 * empty ()
 *   Match an empty map.
 *   Result type: void
 *
 * key<X> ("name")
 *   Require the presence of key "name" and parse its value as X.
 *   Result type: X
 *
 * key<X> ("name", X default_value)
 *   Optional key "name". If missing, default_value is supplied.
 *   Result type: X
 *
 * ---------------------------------------------------------------------
 * Rule modifiers
 * ---------------------------------------------------------------------
 *
 * *rule
 *   Make a rule optional.
 *   Result type: maybe<result_of (rule)>
 *
 * -rule   (ONLY)
 *   Restrict the map so that it may not contain keys other than those
 *   explicitly mentioned in the rule.
 *   Result type: same as rule
 *
 * +rule   (INVERT ONLY)
 *   Invert the meaning of the ONLY restriction.
 *   Result type: same as rule
 *
 * ---------------------------------------------------------------------
 * Rule composition
 * ---------------------------------------------------------------------
 *
 * ruleA && ruleB
 *   The map must satisfy both rules.
 *   Result type:
 *     tuple<result_of (ruleA), result_of (ruleB)>
 *
 *   If either result is itself a tuple, it is flattened into the result
 *   (behaves like a parameter pack).
 *
 *   Special case:
 *     -ruleA && -ruleB  is equivalent to  -(ruleA && ruleB)
 *
 * ruleA || ruleB
 *   The map must satisfy exactly one of the rules.
 *   Result type:
 *     std::variant<result_of(ruleA), result_of(ruleB)>
 *
 * ---------------------------------------------------------------------
 * Closed vs open rules
 * ---------------------------------------------------------------------
 *
 * Rules are *closed by default*. A rule describes a complete schema for
 * the map, not a partial constraint.
 *
 * In particular:
 *
 *     key<X>("k")
 *
 * means that the map:
 *   • contains the key "k"
 *   • contains no other keys
 *
 * As a consequence:
 *
 *     -key<X>("k")
 *
 * does not further restrict the rule and has no additional effect.
 *
 * The '+' operator is used to *open* a rule:
 *
 *     +key<X>("k")
 *
 * means that the map must contain "k", but may contain additional keys
 * beyond those mentioned in the rule.
 *
 * The '-' operator becomes meaningful primarily after rule composition,
 * where it enforces that no keys other than those implied by the composed
 * rule are present.
 *
 * ---------------------------------------------------------------------
 * Examples
 * ---------------------------------------------------------------------
 *
 * Example 1: Require an empty query
 *
 *     auto r = empty ();
 *     validate (query, r);          // returns void
 *
 * Example 2: Required and optional keys
 *
 *     auto r =
 *         key<int> ("page") &&
 *         key<int> ("limit", 10);
 *
 *     // Result type: tuple<int, int>
 *     auto [page, limit] = validate (query, r);
 *
 * Example 3: Optional rule
 *
 *     auto r = *key<std::string>("search");
 *
 *     // Result type: std::optional<std::string>
 *     auto value = validate(query, r);
 *
 * Example 4: Allow other keys to be present
 *
 *     auto r = -(
 *         key<int>("x") &&
 *         key<int>("y")
 *     );
 *
 *     // Map may contain values other than "x" and "y", which are ignored.
 *     auto [x, y] = validate(query, r);
 *
 * Example 5: Alternatives
 *
 *     auto r =
 *         key<int>("id") ||
 *         key<std::string>("name");
 *
 *     // Result type: std::variant<int, std::string>
 *     auto v = validate(query, r);
 *
 * ---------------------------------------------------------------------
 * Reading values from strings
 * ---------------------------------------------------------------------
 *
 * Rules that decode values (such as key<X> (...)) rely on a uniform
 * conversion mechanism:
 *
 *     data::encoding::read<X> {} (std::string)
 *
 * This function object is responsible for converting a string into a
 * value of type X. Users may provide template specializations of
 * data::encoding::read<X> to define custom parsing behavior for their
 * own types.
 *
 * The default behavior is:
 *
 *   1. For std::integral types, attempt conversion using std::from_chars
 *   2. Attempt extraction using operator >> with an
 *      std::istream
 *   3. Attempt construction from a string
 *
 * Validation fails if none of these conversions succeed.
 *
 * This design keeps schema logic independent of parsing policy:
 * schemas describe *what* is required, while data::encoding::read<X>
 * defines *how* values of type X are obtained from strings.
 *
 */

#include <data/concepts.hpp>
#include <data/maybe.hpp>
#include <data/either.hpp>
#include <data/tuple.hpp>
#include <data/container.hpp>
#include <data/stack.hpp>
#include <data/list.hpp>
#include <data/map.hpp>
#include <data/dispatch.hpp>
#include <data/set.hpp>
#include <data/cross.hpp>
#include <data/string.hpp>
#include <data/encoding/read.hpp>

namespace data::schema::rule {

    // a schema for reading map-like data structures
    // written as a string.
    template <typename X> struct map;

    // a schema for list-like data structures.
    template <typename X> struct list;

    // a schema for reading map-like data structures that
    // are presented with an ordering and possibly repeated keys
    // such as an HTTP form and a command line. Not currently
    // implemented, so just use map for now.
    template <typename X> struct dispatch;

    // starting here, we have a lot of definitions that you don't
    // really need to read. Scroll down until the next note to
    // arrive at a natural interface for using the map.

    // specifies that a data structure is empty.
    struct empty;

    // specifies that a data structure can have anything.
    struct blank;

    // specifies that a map may not have anything else.
    template <typename X> struct only;

    // specifies that a data structure has a given value.
    template <std::default_initializable X> struct value;

    template <std::default_initializable X> struct default_value;

    // specifies that the structure is equal to a given value.
    template <typename X> struct equal;

}

namespace data::schema::map {
    // make a rule for an empty map.
    constexpr rule::map<rule::empty> empty ();

    // make a rule for an a map that can have anything in it.
    constexpr rule::map<rule::blank> blank ();

    // make a rule that says a map has a given key.
    template <typename X> constexpr rule::map<rule::only<rule::value<X>>> key (const string &key);

    // make a rule that says a map has a given key with a given default.
    template <typename X> constexpr rule::map<rule::only<rule::default_value<X>>> key (const string &key, const X &def);
}

namespace data::schema::list {
    // make a rule for an empty list.
    constexpr rule::list<rule::empty> empty ();

    // make a rule for a list with a single value.
    template <typename X> constexpr rule::list<rule::value<X>> value ();

    // make a rule for a list with a single value with default.
    template <typename X> constexpr rule::list<rule::default_value<X>> value (const X &def);

    // make a rule for a list with a single value that equals a given value.
    template <typename X> constexpr rule::list<rule::equal<X>> equal (const X &val);
}

namespace data::schema::dispatch {
    // make a rule for an empty dispatch.
    constexpr rule::map<rule::empty> empty ();
}

// rules that modify other rules.
namespace data::schema::rule {

    // match a rule or match its nonexistence.
    template <typename X> struct optional;

    template <typename ...X> struct all;

    template <typename ...X> struct any;

    template <typename ...X> struct sequence;

    // specifies that a rule cannot match additional unspecified values.
    template <typename X> constexpr auto operator - (const map<X> &);

    // use the + operator to allow a rule to match with extra values.
    template <typename X> constexpr auto operator + (const map<X> &);

    // use the * operator to make a rule optional.
    template <typename X> constexpr auto operator * (const map<only<X>> &);

    // use the & operator to combine two map rules.
    template <typename X, typename Y> constexpr auto operator && (const map<X> &, const map<Y> &);

    // use the | make alternatives.
    template <typename X, typename Y> constexpr auto operator || (const map<X> &, const map<Y> &);

    // join two list schemas together.
    template <typename X, typename Y> constexpr auto operator + (const list<X> &, const list<Y> &);

    template <typename string> concept String = /*std::convertible_to<string, data::string> && */std::equality_comparable_with<data::string, string>;
}

namespace data::schema {

    // Throw something that inherets from mismatch if validation fails.
    struct mismatch {};

    // If validation succeeds, return a logical type containing
    // all parsed values.
    template <typename ...context, rule::String string, typename X>
    auto validate (data::map<string, string> m, const rule::map<X> &r);

    template <typename ...context, rule::String string, typename X>
    auto validate (const std::map<string, string> &m, const rule::map<X> &r);

    template <typename ...context, rule::String string, typename X>
    auto validate (data::list<string> m, const rule::list<X> &r);

    template <typename ...context, rule::String string, typename X>
    auto validate (data::stack<string> m, const rule::list<X> &r);

    template <typename ...context, rule::String string, typename X>
    auto validate (const data::cross<string> &m, const rule::list<X> &r);

    // thrown when the value could not be read.
    struct invalid_entry : mismatch {
        data::string Key;
        constexpr invalid_entry (const data::string &k) : mismatch {}, Key {k} {}
    };

    // thrown when a key was expected that was not available.
    struct missing_key : mismatch {
        data::string Key;
        constexpr missing_key (const data::string &k) : mismatch {}, Key {k} {}
    };

    // thrown when a key is present which is only valid in some alternative
    // rule that doesn't match the whole map or when an optional expression
    // is partially matched.
    struct incomplete_match : mismatch {
        data::string Key;
        constexpr incomplete_match (const data::string &k): mismatch {}, Key {k} {}
    };

    // thrown when a map has keys that are not provided by the schema.
    struct unknown_key : mismatch {
        data::string Key;
        constexpr unknown_key (const data::string &k) : mismatch {}, Key {k} {}
    };

    struct invalid_value_at : mismatch {
        size_t Position;
        constexpr invalid_value_at (size_t pos) : mismatch {}, Position {pos} {}
    };

    struct end_of_sequence : mismatch {
        size_t Position;
        constexpr end_of_sequence (size_t pos) : mismatch {}, Position {pos} {}
    };

    struct no_end_of_sequence : mismatch {
        size_t Position;
        constexpr no_end_of_sequence (size_t pos) : mismatch {}, Position {pos} {}
    };

}

namespace data::schema::rule {

    template <> struct map<empty> {
        constexpr map () {}
        constexpr map (const map<blank> &) {}
    };

    template <> struct map<blank> {
        constexpr map () {}
        constexpr map (map<empty>) {}
    };

    template <typename X> struct map<value<X>> {
        string Key;
    };

    template <typename X> struct map<default_value<X>> : map<value<X>> {
        X Default;
        constexpr map (const string &k, const X &def): map<value<X>> {k}, Default {def} {}
    };

    template <typename X> struct map<only<X>> : map<X> {
        using map<X>::map;
        constexpr map (const map<X> &m): map<X> {m} {}
    };

    template <typename X, typename ...Y> struct map<all<X, Y...>> : tuple<map<X>, map<Y>...> {
        using parent = tuple<map<X>, map<Y>...>;
        using tuple<map<X>, map<Y>...>::tuple;
        constexpr map (parent &&p): parent {p} {}

        // map rule all is valid if no operand
        // has a same key as any other operand.
        bool valid () const;
    };

    template <typename X, typename ...Y> struct map<any<X, Y...>> : tuple<map<X>, map<Y>...> {
        using parent = tuple<map<X>, map<Y>...>;
        using tuple<map<X>, map<Y>...>::tuple;
        constexpr map (parent &&p): parent {p} {}
        // map rule all is valid if no operand
        // has a same key as any other operand.
        bool valid () const;
    };

    template <typename X> struct map<optional<X>> : map<X> {
        using map<X>::map;
        constexpr map (const map<X> &m): map<X> {m} {}
    };

    template <> struct list<empty> {};

    template <typename X> struct list<value<X>> {};

    template <typename X> struct list<default_value<X>> {
        X Default;
    };

    template <typename X> struct list<equal<X>> {
        X Value;
    };

    template <typename>
    struct is_default : std::false_type {};

    template <typename T>
    struct is_default<default_value<T>> : std::true_type {};

    template <typename...>
    struct defaults_trail : std::true_type {};

    template <typename X, typename Y, typename... Z>
    struct defaults_trail<X, Y, Z...>
        : std::bool_constant<
            !(is_default<X>::value && !is_default<Y>::value)
            && defaults_trail<Y, Z...>::value
        > {};

    template <typename... X>
    struct list<sequence<X...>> {
        static_assert (
            defaults_trail<X...>::value,
            "default_value must be at the end or followed only by default_value"
        );

        using tuple_type = std::tuple<list<X>...>;
        tuple_type rules;

        constexpr list (X... xs)
            : rules (std::move (xs)...) {}
    };

    template <typename X> struct apply_optional;

    template <typename X> struct apply_optional<only<X>> {
        using result = only<optional<X>>;
    };

    // here is some stuff that says how blank composes.
    template <typename X> struct apply_blank {
        using result = X;
    };

    template <> struct apply_blank<empty> {
        using result = blank;
    };

    template <typename X> struct apply_blank<only<X>> {
        using result = X;
    };

    template <typename X> struct apply_only {
        using result = only<X>;
    };

    template <> struct apply_only<empty> {
        using result = empty;
    };

    template <> struct apply_only<blank> {
        using result = empty;
    };

    template <typename X> struct apply_only<only<X>> {
        using result = only<X>;
    };

    // here is some stuff that says how blank composes.
    template <typename X, typename Y> struct intersect;

    template <typename X, typename Y> struct intersect {
        using result = all<X, Y>;
        constexpr map<result> operator () (const map<X> &, const map<Y> &) const;
    };

    // intersect anything with empty is itself
    template <typename Y> struct intersect<empty, Y> {
        using result = Y;
        constexpr map<result> operator () (map<empty> a, const map<Y> &b) const;
    };

    template <typename X> struct intersect<X, empty> {
        using result = X;
        constexpr map<result> operator () (const map<X> &a, map<empty> b) const;
    };

    template <typename Y> struct intersect<blank, Y> {
        using result = apply_blank<Y>::result;
        constexpr map<result> operator () (map<blank> a, const map<Y> &b) const;
    };

    template <typename X> struct intersect<X, blank> {
        using result = apply_blank<X>::result;
        constexpr map<result> operator () (const map<X> &a, map<blank> b) const;
    };

    template <typename X, typename Y> struct intersect<only<X>, only<Y>> {
        using result = only<typename intersect<X, Y>::result>;
        constexpr map<result> operator () (const map<only<X>> &, const map<only<Y>> &) const;
    };

    template <typename X, typename Y> struct intersect<only<X>, Y> {
        using result = intersect<X, Y>::result;
        constexpr map<result> operator () (const map<only<X>> &a, const map<Y> &b) const;
    };

    template <typename X, typename Y> struct intersect<X, only<Y>> {
        using result = intersect<X, Y>::result;
        constexpr map<result> operator () (const map<X> &a, const map<only<Y>> &b) const;
    };

    template <typename ...X, typename ...Y> struct intersect<all<X...>, all<Y...>> {
        using result = all<X..., Y...>;
        constexpr map<result> operator () (const map<all<X...>> &, const map<all<Y...>> &) const;
    };

    template <typename ...X, typename Y> struct intersect<all<X...>, Y> {
        using result = all<X..., Y>;
        constexpr map<result> operator () (const map<all<X...>> &, const map<Y> &) const;
    };

    template <typename X, typename ...Y> struct intersect<X, all<Y...>> {
        using result = all<X, Y...>;
        constexpr map<result> operator () (const map<X> &, const map<all<Y...>> &) const;
    };

    template <typename X, typename Y> struct unite;

    template <typename X, typename Y> struct unite<only<X>, only<Y>> {
        using result = only<typename unite<X, Y>::result>;
        constexpr map<result> operator () (const map<only<X>> &, const map<only<Y>> &) const;
    };

    template <typename X, typename Y> struct unite {
        using result = any<X, Y>;
        constexpr map<result> operator () (const map<X> &, const map<Y> &) const;
    };

    template <typename ...X, typename Y> struct unite<any<X...>, Y> {
        using result = any<X..., Y>;
        constexpr map<result> operator () (const map<any<X...>> &, const map<Y> &) const;
    };

    template <typename X, typename ...Y> struct unite<X, any<Y...>> {
        using result = any<X, Y...>;
        constexpr map<result> operator () (const map<X> &, const map<any<Y...>> &) const;
    };

    template <typename ...X, typename ...Y> struct unite<any<X...>, any<Y...>> {
        using result = any<X..., Y...>;
        constexpr map<result> operator () (const map<any<X...>> &, const map<any<Y...>> &) const;
    };

    template <typename X, typename Y> struct join {
        using result = sequence<X, Y>;
        constexpr list<result> operator () (const list<X> &, const list<Y> &) const;
    };

    template <typename ...X, typename ...Y> struct join<sequence<X...>, sequence<Y...>> {
        using result = sequence<X..., Y...>;
        constexpr list<result> operator () (const list<sequence<X...>> &, const list<sequence<Y...>> &) const;
    };

    template <typename X, typename ...Y> struct join<X, sequence<Y...>> {
        using result = sequence<X, Y...>;
        constexpr list<result> operator () (const list<X> &, const list<sequence<Y...>> &) const;
    };

    template <typename ...X, typename Y> struct join<sequence<X...>, Y> {
        using result = sequence<X..., Y>;
        constexpr list<result> operator () (const list<sequence<X...>> &, const list<Y> &) const;
    };

    template <typename Y> struct join<empty, Y> {
        using result = Y;
        constexpr list<result> operator () (const list<empty> &, const list<Y> &) const;
    };

    template <typename X> struct join<X, empty> {
        using result = X;
        constexpr list<result> operator () (const list<X> &, const list<empty> &) const;
    };

    // specifies that a rule cannot match additional unspecified values.
    template <typename X> constexpr auto inline operator - (const map<X> &m) {
        return map<typename apply_only<X>::result> {m};
    }

    // use the + operator to allow a rule to match with extra values.
    template <typename X> constexpr auto inline operator + (const map<X> &m) {
        return map<typename apply_blank<X>::result> {m};
    }

    // use the * operator to make a rule optional.
    template <typename X> constexpr auto inline operator * (const map<only<X>> &m) {
        return map<typename apply_optional<only<X>>::result> {m};
    }

    // use the & operator to combine two map rules.
    template <typename X, typename Y> constexpr auto inline operator && (const map<X> &a, const map<Y> &b) {
        return intersect<X, Y> {} (a, b);
    }

    // use the | make alternatives.
    template <typename X, typename Y> constexpr auto inline operator || (const map<X> &a, const map<Y> &b) {
        return unite<X, Y> {} (a, b);
    }

    // join two list schemas together.
    template <typename X, typename Y> constexpr auto inline operator + (const list<X> &a, const list<Y> &b) {
        return join<X, Y> {} (a, b);
    }

}

namespace data::schema::rule {

    template <typename X, typename ...context> struct validate;

    using unit = std::monostate;

    template <typename ...context> struct validate<blank, context...> {
        using result = unit;

        template <typename any>
        result operator () (const any &m, map<blank> r);
    };

    template <typename ...context> struct validate<empty, context...> {
        using result = unit;

        template <Iterable seq>
        result operator () (const seq &x, const list<empty> &r);

        template <String string>
        result operator () (data::map<string, string> m, const map<empty> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<empty> &r);
    };

    template <typename X, typename ...context> struct validate<value<X>, context...> {
        using result = X;

        template <String string>
        result operator () (data::map<string, string> m, const map<value<X>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<value<X>> &r);

        template <Iterable seq> result operator () (const seq &x, const list<value<X>> &r);

        template <typename iterator> result sequence (iterator i, const list<value<X>> &r, size_t index);
    };

    template <typename X, typename ...context> struct validate<default_value<X>, context...> {
        using result = X;

        template <String string>
        result operator () (data::map<string, string> m, const map<default_value<X>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<default_value<X>> &r);

        template <Iterable seq> result operator () (const seq &x, const list<default_value<X>> &r);

        template <typename iterator> result sequence (iterator i, const list<default_value<X>> &r, size_t index);
    };

    template <typename X, typename ...context> struct validate<equal<X>, context...> {
        using result = unit;

        template <String string>
        result operator () (data::map<string, string> m, const map<equal<X>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<equal<X>> &r);

        template <Iterable seq> result operator () (const seq &x, const list<equal<X>> &r);

        template <typename iterator> result sequence (iterator i, const list<equal<X>> &r, size_t index);
    };

    template <typename X, typename ...context> struct validate<optional<X>, context...> {
        using result = maybe<typename validate<X, context...>::result>;

        template <String string>
        result operator () (const data::map<string, string> &m, const map<optional<X>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<optional<X>> &r);
    };

    template <typename ...X, typename ...context> struct validate<sequence<X...>, context...> {
        using result = tuple<typename validate<X, context>::result...>;

        template <IterableSequence seq> result operator () (const seq &x, const list<sequence<X...>> &r);
    };

    template <typename ...X, typename ...context> struct validate<all<X...>, context...> {
        using result = tuple<typename validate<X, context...>::result...>;
        template <typename Map>
        result operator () (const Map &m, const map<all<X...>> &r);
    };

    template <typename ...X, typename ...context> struct validate<any<X...>, context...> {
        using result = either<typename validate<X, context...>::result...>;

        template <typename Map, size_t index = 0>
        result operator () (const Map &m, const map<any<X...>> &r);

    };

    template <typename X, typename ...context> struct validate<only<X>, context...> {
        using result = typename validate<X, context...>::result;

        template <typename Map>
        result operator () (const Map &m, const map<only<X>> &r);
    };

}

namespace data::schema::map {

    // this is the end of the user interface. Next comes definitions of functions.
    constexpr typename rule::map<rule::empty> inline empty () {
        return rule::map<rule::empty> {};
    }

    // make a rule that says a map has a given key.
    template <typename X> constexpr typename rule::map<rule::only<rule::value<X>>> inline key (const string &key) {
        return rule::map<rule::only<rule::value<X>>> {rule::map<rule::value<X>> {key}};
    }

    template <typename X> constexpr typename rule::map<rule::only<rule::default_value<X>>> inline key (const string &key, const X &def) {
        return rule::map<rule::only<rule::default_value<X>>> {rule::map<rule::default_value<X>> {key, def}};
    }
}

namespace data::schema::list {
    // make a rule for an empty list.
    constexpr rule::list<rule::empty> inline empty () {
        return rule::list<rule::empty> {};
    }

    // make a rule for a list with a single value.
    template <typename X> constexpr rule::list<rule::value<X>> inline value () {
        return rule::list<rule::value<X>> {};
    }

    // make a rule for a list with a single value with default.
    template <typename X> constexpr rule::list<rule::default_value<X>> inline value (const X &def) {
        return rule::list<rule::default_value<X>> {def};
    }

    // make a rule for a list with a single value that equals a given value.
    template <typename X> constexpr rule::list<rule::equal<X>> inline equal (const X &val) {
        return rule::list<rule::equal<X>> {val};
    }
}

namespace data::schema {

    template <typename ...context, rule::String string, typename X>
    auto inline validate (data::map<string, string> m, const rule::map<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }

    // we don't really support std::map yet.
    template <typename ...context, rule::String string, typename X>
    auto inline validate (const std::map<string, string> &m, const rule::map<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }

    template <typename ...context, rule::String string, typename X>
    auto inline validate (data::list<string> m, const rule::list<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }

    template <typename ...context, rule::String string, typename X>
    auto inline validate (data::stack<string> m, const rule::list<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }

    template <typename ...context, rule::String string, typename X>
    auto inline validate (const data::cross<string> &m, const rule::list<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }
}

namespace data::schema::rule {

    template <typename ...context> template <Iterable seq>
    typename validate<empty, context...>::result
    validate<empty, context...>::operator () (const seq &x, const list<empty> &r) {
        auto b = x.begin ();
        if (b == x.end ()) return {};
        else throw end_of_sequence {0};
    }

    template <typename ...context> template <String string>
    typename validate<empty, context...>::result
    validate<empty, context...>::operator () (data::map<string, string> m, const map<empty> &r) {
        auto b = m.begin ();
        if (b == m.end ()) return {};
        else throw unknown_key {data::string (b->Key)};
    }

    template <typename ...context> template <String string>
    typename validate<empty, context...>::result
    validate<empty, context...>::operator () (const std::map<string, string> &m, const map<empty> &r) {
        auto b = m.begin ();
        if (b == m.end ()) return {};
        else throw unknown_key {b->first};
    }

    template <typename ...context> template <typename any>
    typename validate<blank, context...>::result inline
    validate<blank, context...>::operator () (const any &m, map<blank> r) {
        return {};
    }

    template <typename X, typename ...context> template <String string>
    typename validate<value<X>, context...>::result inline
    validate<value<X>, context...>::operator () (data::map<string, string> m, const map<value<X>> &r) {
        const string *v = m.contains (r.Key);
        if (!bool (v)) throw missing_key {data::string (r.Key)};
        maybe<X> x = encoding::read<X, context...> {} (*v);
        if (!bool (x)) throw invalid_entry {*v};
        return *x;
    }

    template <typename X, typename ...context> template <String string>
    typename validate<value<X>, context...>::result inline
    validate<value<X>, context...>::operator () (const std::map<string, string> &m, const map<value<X>> &r) {
        auto v = m.find (r.Key);
        if (v == m.end ()) throw missing_key {data::string (r.Key)};
        maybe<X> x = encoding::read<X, context...> {} (v->second);
        if (!bool (x)) throw invalid_entry {r.Key};
        return *x;
    }

    template <typename X, typename ...context> template <String string>
    typename validate<default_value<X>, context...>::result inline
    validate<default_value<X>, context...>::operator () (data::map<string, string> m, const map<default_value<X>> &r) {
        const string *v = m.contains (r.Key);
        if (!bool (v)) return r.Default;
        maybe<X> x = encoding::read<X, context...> {} (*v);
        if (!bool (x)) throw invalid_entry {*v};
        return *x;
    }

    template <typename X, typename ...context> template <typename iterator>
    typename validate<value<X>, context...>::result inline
    validate<value<X>, context...>::sequence (iterator i, const list<value<X>> &r, size_t index) {
        maybe<X> x = encoding::read<X, context...> {} (*i);
        if (!bool (x)) throw invalid_value_at {index};
        return *x;
    }

    template <typename X, typename ...context> template <typename iterator>
    typename validate<default_value<X>, context...>::result inline
    validate<default_value<X>, context...>::sequence (iterator i, const list<default_value<X>> &r, size_t index) {
        maybe<X> x = encoding::read<X, context...> {} (*i);
        if (!bool (x)) return r.Default;
        return *x;
    }

    template <typename X, typename ...context> template <typename iterator>
    typename validate<equal<X>, context...>::result inline
    validate<equal<X>, context...>::sequence (iterator i, const list<equal<X>> &r, size_t index) {
        maybe<X> x = encoding::read<X, context...> {} (*i);
        if (!bool (x) || *x != r.Value) throw invalid_value_at {index};
        return unit {};
    }

    template <typename X, typename ...context> template <Iterable seq>
    typename validate<value<X>, context...>::result inline
    validate<value<X>, context...>::operator () (const seq &x, const list<value<X>> &r) {
        if (x.size () == 0) throw end_of_sequence {0};
        if (x.size () > 1) throw no_end_of_sequence {1};
        return sequence (x.begin (), r, 0);
    }

    template <typename X, typename ...context> template <Iterable seq>
    typename validate<default_value<X>, context...>::result inline
    validate<default_value<X>, context...>::operator () (const seq &x, const list<default_value<X>> &r) {
        if (x.size () == 0) throw end_of_sequence {0};
        if (x.size () > 1) throw no_end_of_sequence {1};
        return sequence (x.begin (), r, 0);
    }

    template <typename X, typename ...context> template <Iterable seq>
    typename validate<equal<X>, context...>::result inline
    validate<equal<X>, context...>::operator () (const seq &x, const list<equal<X>> &r) {
        if (x.size () == 0) throw end_of_sequence {0};
        if (x.size () > 1) throw no_end_of_sequence {1};
        return sequence (x.begin (), r, 0);
    }

    template <std::size_t Z, typename SeqIt, typename... X>
    struct validate_sequence;

    template <std::size_t Z, typename SeqIt>
    struct validate_sequence<Z, SeqIt> {
        template <typename ...context>
        static auto apply (SeqIt& it, SeqIt end, const list<sequence<>> &schema) {
            return std::tuple<> {};
        }
    };

    template <std::size_t Z, typename SeqIt, typename Head, typename... Tail>
    struct validate_sequence<Z, SeqIt, Head, Tail...> {

        template <typename ...context>
        static auto apply (SeqIt &it, SeqIt end, const list<sequence<Head, Tail...>>& schema) {
            if (it == end) throw end_of_sequence {Z};

            // validate current element
            auto value = validate<Head, context...> {} (it, static_cast<const list<Head> &> (schema), Z);

            ++it;

            // recurse
            auto tail = validate_sequence<Z + 1, SeqIt, Tail...>::template
                apply<context...> (it, end, static_cast<const list<sequence<Tail...>> &> (schema));

            return std::tuple_cat (
                std::make_tuple (std::move (value)),
                std::move (tail)
            );
        }
    };

    template <typename ...X, typename ...context> template <IterableSequence seq>
    typename validate<sequence<X...>, context...>::result
    validate<sequence<X...>, context...>::operator () (const seq &x, const list<sequence<X...>> &r) {
        auto it  = x.begin ();
        auto end = x.end ();

        auto result = validate_sequence<0, decltype (it), X...>::template apply<context...> (it, end, r);

        if (it != end) throw no_end_of_sequence {};

        return result;
    }

    // NOTE: we should be able to get rid of get_rule by
    // making context... first in validate.
    template <typename X> struct get_rule;

    template <typename X> struct get_rule<map<X>> {
        using type = X;
    };

    template <typename X>
    struct get_keys {
        set<data::string> operator () (const map<X> &) const {
            return {};
        }
    };

    template <typename X>
    struct get_keys<value<X>> {
        set<data::string> operator () (const map<value<X>> &x) const {
            return {x.Key};
        }
    };

    template <typename X>
    struct get_keys<optional<X>> {
        set<data::string> operator () (const map<optional<X>> &x) const {
            return get_keys<X> {} (static_cast<const map<X> &> (x));
        }
    };

    template <typename X>
    struct get_keys<default_value<X>> {
        set<data::string> operator () (const map<default_value<X>> &x) const {
            return {x.Key};
        }
    };

    template <typename X>
    struct get_keys<only<X>> {
        set<data::string> operator () (const map<only<X>> &x) const {
            return get_keys<X> {} (static_cast<const map<X> &> (x));
        }
    };

    template <typename ...X>
    struct get_keys<all<X...>> {
        set<data::string> operator () (const map<all<X...>> &x) const {
            return std::apply ([] (auto &&...elems) {
                return (get_keys<typename get_rule<unconst<unref<decltype (elems)>>>::type> {} (elems) | ...);
            }, static_cast<const tuple<map<X>...> &> (x));
        }
    };

    template <typename ...X>
    struct get_keys<any<X...>> {
        set<data::string> operator () (const map<any<X...>> &x) const {
            return std::apply ([] (auto &&...elems) {
                return (get_keys<typename get_rule<unconst<unref<decltype (elems)>>>::type> {} (elems) | ...);
            }, static_cast<const tuple<map<X>...> &> (x));
        }
    };

    template <typename X, typename ...context>
    template <String string>
    typename validate<optional<X>, context...>::result inline
    validate<optional<X>, context...>::operator () (const data::map<string, string> &m, const map<optional<X>> &r) {
        try {
            return {validate<X, context...> {} (m, static_cast<map<X>> (r))};
        } catch (const mismatch &) {}
        // if the match fails, the map may not contain any keys
        // of the sub rule.
        for (const string &k: get_keys<X> {} (static_cast<map<X>> (r)))
            if (m.contains (k)) throw incomplete_match {k};
        return {};
    }

    template <typename X, typename ...context>
    template <String string>
    typename validate<optional<X>, context...>::result inline
    validate<optional<X>, context...>::operator () (const std::map<string, string> &m, const map<optional<X>> &r) {
        try {
            return {validate<X, context...> {} (m, static_cast<map<X>> (r))};
        } catch (const mismatch &) {}
        // if the match fails, the map may not contain any keys
        // of the sub rule.
        for (const string &k: get_keys<X> {} (static_cast<map<X>> (r)))
            if (m.find (k) != m.end ()) throw incomplete_match {k};
        return {};
    }

    // get all keys from other indices that do not appear in the given index.
    template <typename ...X> struct get_alternative_keys {
        template <size_t index, size_t recursive_index = 0>
        set<data::string> operator () (const map<any<X...>> &r, const set<data::string> &x = {}) {
            using tuple_type = tuple<map<X>...>;
            constexpr const size_t tuple_size = std::tuple_size_v<tuple_type>;
            if constexpr (recursive_index < tuple_size) {
                using rule_type = get_rule<unconst<unref<decltype (std::get<recursive_index> (r))>>>::type;
                set<string> these_keys = get_keys<rule_type> {} (std::get<recursive_index> (r));
                set<string> next_keys;
                if constexpr (index == recursive_index) {
                    next_keys = x;
                    for (const string &y : these_keys) next_keys = next_keys.remove (y);
                } else {
                    next_keys = x | these_keys;
                }
                return operator ()<index, recursive_index + 1> (r, next_keys);
            } else {
                return x;
            }
        }
    };

    template <typename ...X, typename ...context>
    template <typename Map>
    typename validate<all<X...>, context...>::result inline
    validate<all<X...>, context...>::operator () (const Map &m, const map<all<X...>> &r) {
        using tuple_type = tuple<map<X>...>;
        return std::apply ([&m] (auto &&...elems) -> result {
            return result {validate<typename get_rule<unconst<unref<decltype (elems)>>>::type, context...> {} (m, elems)...};
        }, static_cast<const tuple_type &> (r));
    }


    template <size_t index, typename Map, typename ...X> struct has_no_unused_alternatives;

    template <size_t index, String string, typename ...X>
    struct has_no_unused_alternatives<index, data::map<string, string>, X...> {
        unit operator () (const data::map<string, string> &m, const map<any<X...>> &r) const {
            for (const data::string &k: get_alternative_keys<X...> {}.template operator ()<index> (r))
                if (m.contains (k)) throw incomplete_match {k};
            return unit {};
        }
    };

    template <size_t index, String string, typename ...X>
    struct has_no_unused_alternatives<index, std::map<string, string>, X...> {
        unit operator () (const std::map<string, string> &m, const map<any<X...>> &r) const {
            for (const data::string &k: get_alternative_keys<X...> {}.template operator ()<index> (r))
                if (m.find (k) != m.end ()) throw incomplete_match {k};
            return unit {};
        }
    };

    template <typename ...X, typename ...context>
    template <typename Map, size_t index>
    typename validate<any<X...>, context...>::result
    validate<any<X...>, context...>::operator () (const Map &m, const map<any<X...>> &r) {
        using tuple_type = tuple<map<X>...>;
        constexpr const size_t tuple_size = std::tuple_size_v<tuple_type>;
        using rule_type = typename get_rule<std::tuple_element_t<index, tuple_type>>::type;
        struct validate_alternative {
            result operator () (const Map &m, const map<any<X...>> &r) {
                result res {std::in_place_index<index>,
                    validate<rule_type, context...> {}
                    (m, std::get<index> (static_cast<const tuple_type &> (r)))};
                has_no_unused_alternatives<index, Map, X...> {} (m, r);
                return res;
            }
        };

        result res;
        if constexpr (index + 1 < tuple_size) {
            try {
                return validate_alternative {} (m, r);
            } catch (const mismatch &) {
                return operator ()<Map, index + 1> (m, r);
            }
        } else if constexpr (index + 1 == tuple_size) {
            return validate_alternative {} (m, r);
        } else {
            throw data::exception {} << "Invalid index parameter";
        }

    }

    template <typename ...X>
    struct tuple_rule_valid {
        template <size_t index = 0>
        bool operator () (const tuple<map<X>...> &m, const set<string> &x = {}) {
            constexpr const size_t tuple_size = std::tuple_size_v<tuple<map<X>...>>;
            if constexpr (index < tuple_size) {
                using rule_type = get_rule<unconst<unref<decltype (std::get<index> (m))>>>::type;
                set<string> next_keys = get_keys<rule_type> {} (std::get<index> (m));
                auto intersection = x & next_keys;
                if (!data::empty (intersection)) return false;
                return operator ()<index + 1> (m, x | next_keys);
            }

            return true;
        }
    };

    template <typename X, typename ...Y>
    bool inline map<all<X, Y...>>::valid () const {
        if (!std::apply ([] (auto &&...elems) -> bool {
            return (data::valid (elems) && ...);
        }, static_cast<const std::tuple<map<X>, map<Y>...>> (*this))) return false;
        return tuple_rule_valid<X, Y...> {} (static_cast<const tuple<map<X>, map<Y>...>&> (*this), {});
    }

    template <typename X, typename ...Y>
    bool inline map<any<X, Y...>>::valid () const {
        return std::apply ([] (auto &&...elems) -> bool {
            return (data::valid (elems) && ...);
        }, static_cast<const std::tuple<map<X>, map<Y>...>> (*this));
    }

    // check that the map has no other elements than those given.
    struct none_else {
        template <String string>
        void operator () (data::map<string, string> m, const set<data::string> &x) {
            auto z = m.begin ();
            auto n = x.begin ();

            while (z != m.end ()) {
                // if we are at the end of the list of allowed keys,
                // then there must be keys in the map that aren't allowed.
                if (n == x.end ()) throw unknown_key {z->Key};

                if (z->Key > *n) n++;
                else {
                    if (z->Key != *n) throw unknown_key {z->Key};
                    n++;
                    z++;
                }
            }
        }

        template <String string>
        void operator () (std::map<string, string>, const set<data::string> &);
    };

    template <typename X, typename ...context> 
    template <typename Map>
    typename validate<only<X>, context...>::result inline
    validate<only<X>, context...>::operator () (const Map &m, const map<only<X>> &r) {
        auto val = validate<X, context...> {} (m, +r);
        none_else {} (m, get_keys<only<X>> {} (r));
        return val;
    }

    template <typename X, typename Y>
    constexpr map<typename intersect<X, Y>::result> inline
    intersect<X, Y>::operator () (const map<X> &a, const map<Y> &b) const {
        return map<typename intersect::result> {a, b};
    }

    template <typename X, typename Y>
    constexpr map<typename intersect<only<X>, only<Y>>::result> inline
    intersect<only<X>, only<Y>>::operator () (const map<only<X>> &a, const map<only<Y>> &b) const {
        return map<result> {intersect<X, Y> {} (static_cast<const map<X> &> (a), static_cast<const map<Y> &> (b))};
    }

    template <typename X, typename Y>
    constexpr map<typename intersect<only<X>, Y>::result> inline
    intersect<only<X>, Y>::operator () (const map<only<X>> &a, const map<Y> &b) const {
        return map<result> {static_cast<const map<X> &> (a), b};
    }

    template <typename X, typename Y>
    constexpr map<typename intersect<X, only<Y>>::result> inline
    intersect<X, only<Y>>::operator () (const map<X> &a, const map<only<Y>> &b) const {
        return map<result> {a, static_cast<const map<X> &> (b)};
    }

    template <typename ...X, typename ...Y>
    constexpr map<typename intersect<all<X...>, all<Y...>>::result> inline
    intersect<all<X...>, all<Y...>>::operator ()
        (const map<all<X...>> &a, const map<all<Y...>> &b) const {
        return map<result> {std::tuple_cat (a, b)};
    }

    template <typename ...X, typename Y>
    constexpr map<typename intersect<all<X...>, Y>::result> inline
    intersect<all<X...>, Y>::operator () (const map<all<X...>> &a, const map<Y> &b) const {
        using tuple_type_A = tuple<map<X>...>;
        return map<result> {std::tuple_cat (static_cast<tuple_type_A> (a), tuple<map<Y>> (b))};
    }

    template <typename X, typename ...Y>
    constexpr map<typename intersect<X, all<Y...>>::result> inline
    intersect<X, all<Y...>>::operator () (const map<X> &a, const map<all<Y...>> &b) const {
        using tuple_type_B = tuple<map<Y>...>;
        return map<result> {std::tuple_cat (tuple<map<X>> (a), static_cast<tuple_type_B> (b))};
    }

    template <typename Y>
    constexpr map<typename intersect<empty, Y>::result> inline
    intersect<empty, Y>::operator () (map<empty> a, const map<Y> &b) const {
        return b;
    }

    template <typename X>
    constexpr map<typename intersect<X, empty>::result> inline
    intersect<X, empty>::operator () (const map<X> &a, map<empty> b) const {
        return a;
    }

    template <typename X, typename Y> 
    constexpr map<typename unite<X, Y>::result> inline
    unite<X, Y>::operator () (const map<X> &a, const map<Y> &b) const {
        return map<result> {a, b};
    }

    template <typename X, typename Y>
    constexpr map<typename unite<only<X>, only<Y>>::result> inline
    unite<only<X>, only<Y>>::operator () (const map<only<X>> &a, const map<only<Y>> &b) const {
        return map<result> {unite<X, Y> {} (static_cast<const map<X> &> (a), static_cast<const map<Y> &> (b))};
    }

    template <typename ...X, typename Y>
    constexpr map<typename unite<any<X...>, Y>::result> inline
    unite<any<X...>, Y>::operator () (const map<any<X...>> &a, const map<Y> &b) const {
        using tuple_type_A = tuple<map<X>...>;
        return map<result> {std::tuple_cat (static_cast<tuple_type_A> (a), tuple<map<Y>> (b))};
    }

    template <typename X, typename ...Y>
    constexpr map<typename unite<X, any<Y...>>::result> inline
    unite<X, any<Y...>>::operator () (const map<X> &a, const map<any<Y...>> &b) const {
        using tuple_type_B = tuple<map<Y>...>;
        return map<result> {std::tuple_cat (tuple<map<X>> (a), static_cast<tuple_type_B> (b))};
    }


    template <typename X, typename Y>
    constexpr list<typename join<X, Y>::result> inline
    join<X, Y>::operator () (const list<X> &, const list<Y> &) const {
        throw 0;
    }

    template <typename ...X, typename ...Y>
    constexpr list<typename join<sequence<X...>, sequence<Y...>>::result> inline
    join<sequence<X...>, sequence<Y...>>::operator () (const list<sequence<X...>> &, const list<sequence<Y...>> &) const {
        throw 0;
    }

    template <typename X, typename ...Y>
    constexpr list<typename join<X, sequence<Y...>>::result> inline
    join<X, sequence<Y...>>::operator () (const list<X> &, const list<sequence<Y...>> &) const {
        return 0;
    }

    template <typename ...X, typename Y>
    constexpr list<typename join<sequence<X...>, Y>::result> inline
    join<sequence<X...>, Y>::operator () (const list<sequence<X...>> &, const list<Y> &) const {
        return 0;
    }

    template <typename Y>
    constexpr list<typename join<empty, Y>::result> inline
    join<empty, Y>::operator () (const list<empty> &, const list<Y> &) const {
        return 0;
    }

    template <typename X>
    constexpr list<typename join<X, empty>::result>
    join<X, empty>::operator () (const list<X> &, const list<empty> &) const {
        return 0;
    }

}

#endif
