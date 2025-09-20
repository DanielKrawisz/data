
#include <data/maybe.hpp>
#include <data/either.hpp>
#include <data/concepts.hpp>
#include <data/string.hpp>
#include <data/map.hpp>
#include <data/set.hpp>
#include <data/encoding/read.hpp>

#ifndef DATA_SCHEMA_MAP
#define DATA_SCHEMA_MAP

namespace data::schema::rule {

    // a schema for reading map-like data structures, such as
    // command lines and HTTP forms.
    template <typename X> struct map;

    // starting here, we have a lot of definitions that you don't
    // really need to read. Scroll down until the next note to
    // arrive at a natural interface for using the map.

    // specifies that a map is empty.
    struct empty;

    // specifies that a map can have anything.
    struct blank;

    // specifies that a map may not have anything else.
    template <typename X> struct only;

    // match a rule or match its nonexistence.
    template <typename X> struct optional;

    // specifies that a map has a given value.
    template <std::default_initializable X> struct value;

    template <typename ...X> struct all;

    template <typename ...X> struct any;

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
        map<result> operator () (const map<X> &, const map<Y> &);
    };

    // intersect anything with empty is itself
    template <typename Y> struct intersect<empty, Y> {
        using result = Y;
        map<result> operator () (map<empty> a, const map<Y> &b);
    };

    template <typename X> struct intersect<X, empty> {
        using result = X;
        map<result> operator () (const map<X> &a, map<empty> b);
    };

    template <typename Y> struct intersect<blank, Y> {
        using result = apply_blank<Y>::result;
        map<result> operator () (map<blank> a, const map<Y> &b);
    };

    template <typename X> struct intersect<X, blank> {
        using result = apply_blank<X>::result;
        map<result> operator () (const map<X> &a, map<blank> b);
    };

    template <typename X, typename Y> struct intersect<only<X>, only<Y>> {
        using result = only<typename intersect<X, Y>::result>;
        map<result> operator () (const map<only<X>> &, const map<only<Y>> &);
    };

    template <typename X, typename Y> struct intersect<only<X>, Y> {
        using result = intersect<X, Y>::result;
        map<result> operator () (const map<only<X>> &a, const map<Y> &b);
    };

    template <typename X, typename Y> struct intersect<X, only<Y>> {
        using result = intersect<X, Y>::result;
        map<result> operator () (const map<X> &a, const map<only<Y>> &b);
    };

    template <typename ...X, typename ...Y> struct intersect<all<X...>, all<Y...>> {
        using result = all<X..., Y...>;
        map<result> operator () (const map<all<X...>> &, const map<all<Y...>> &);
    };

    template <typename ...X, typename Y> struct intersect<all<X...>, Y> {
        using result = all<X..., Y>;
        map<result> operator () (const map<all<X...>> &, const map<Y> &);
    };

    template <typename X, typename ...Y> struct intersect<X, all<Y...>> {
        using result = all<X, Y...>;
        map<result> operator () (const map<X> &, const map<all<Y...>> &);
    };

    template <typename X, typename Y> struct unite;

    template <typename X, typename Y> struct unite<only<X>, only<Y>> {
        using result = only<typename unite<X, Y>::result>;
        map<result> operator () (const map<only<X>> &, const map<only<Y>> &);
    };

    template <typename X, typename Y> struct unite {
        using result = any<X, Y>;
        map<result> operator () (const map<X> &, const map<Y> &);
    };

    template <typename ...X, typename Y> struct unite<any<X...>, Y> {
        using result = any<X..., Y>;
        map<result> operator () (const map<any<X...>> &, const map<Y> &);
    };

    template <typename X, typename ...Y> struct unite<X, any<Y...>> {
        using result = any<X, Y...>;
        map<result> operator () (const map<X> &, const map<any<Y...>> &);
    };

    template <typename ...X, typename ...Y> struct unite<any<X...>, any<Y...>> {
        using result = any<X..., Y...>;
        map<result> operator () (const map<any<X...>> &, const map<any<Y...>> &);
    };

    // now we know how to create rules. Next follows some stuff the user doesn't
    // need to read that is for validating rules.
    template <typename X, typename ...context> struct validate;

    template <typename string> concept String = std::convertible_to<string, data::string> && std::equality_comparable_with<data::string, string>;

    template <typename ...context> struct validate<empty, context...> {
        using result = void;

        template <String string>
        result operator () (data::map<string, string> m, map<empty> r);

        template <String string>
        result operator () (const std::map<string, string> &m, map<empty> r);
    };

    template <typename ...context> struct validate<blank, context...> {
        using result = void;

        template <typename Map>
        result operator () (const Map &m, map<blank> r);
    };

    template <typename X, typename ...context> struct validate<value<X>, context...> {
        using result = X;

        template <String string>
        result operator () (data::map<string, string> m, const map<value<X>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<value<X>> &r);
    };

    template <typename X, typename ...context> struct validate<optional<X>, context...> {
        using result = maybe<typename validate<X, context...>::result>;

        template <String string>
        result operator () (const data::map<string, string> &m, const map<optional<X>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<optional<X>> &r);
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

    template <> struct map<empty> {};

    template <> struct map<blank> {
        map () {}
        map (map<empty>) {}
    };

    template <typename X> struct map<value<X>> {
        string Key;
    };

    template <typename X> struct map<only<X>> : map<X> {
        using map<X>::map;
        map (const map<X> &m): map<X> {m} {}
    };

    template <typename X, typename ...Y> struct map<all<X, Y...>> : tuple<map<X>, map<Y>...> {
        using parent = tuple<map<X>, map<Y>...>;
        using tuple<map<X>, map<Y>...>::tuple;
        map (parent &&p): parent {p} {}

        // map rule all is valid if no operand
        // has a same key as any other operand.
        bool valid () const;
    };

    template <typename X, typename ...Y> struct map<any<X, Y...>> : tuple<map<X>, map<Y>...> {
        using parent = tuple<map<X>, map<Y>...>;
        using tuple<map<X>, map<Y>...>::tuple;
        map (parent &&p): parent {p} {}
        // map rule all is valid if no operand
        // has a same key as any other operand.
        bool valid () const;
    };

    template <typename X> struct map<optional<X>> : map<X> {
        using map<X>::map;
        map (const map<X> &m): map<X> {m} {}
    };

}

namespace data::schema {

    // here begins an easy user interface.

    typename rule::map<rule::empty> empty ();

    // make a rule that says a map has a given key.
    template <typename X> typename rule::map<rule::only<rule::value<X>>> key (const string &key);
}

namespace data::schema::rule {

    // use the + operator to allow a rule to match with extra values.
    template <typename X> map<typename apply_blank<X>::result> operator + (const map<X> &);

    // specifies that a rule cannot match additional unspecified values.
    template <typename X> map<typename apply_only<X>::result> operator - (const map<X> &);

    // use the * operator to make a rule optional.
    template <typename X> map<typename apply_optional<only<X>>::result> operator * (const map<only<X>> &);

    // NOTE these two operators have a reversed meaning from what the ought to be.
    // use the & operator to combine two map rules.
    template <typename X, typename Y> map<typename intersect<X, Y>::result> operator & (const map<X> &, const map<Y> &);

    // use the | make alternatives.
    template <typename X, typename Y> map<typename unite<X, Y>::result> operator | (const map<X> &, const map<Y> &);
}

namespace data::schema {

    // Next we describe how to validate rules. First come some errors which
    // may be thrown when there is a mismatch.

    struct mismatch {};

    // thrown when the value could not be read.
    struct invalid_value : mismatch {
        data::string Key;
        invalid_value (const data::string &k) : mismatch {}, Key {k} {}
    };

    // thrown when a key was expected that was not available.
    struct missing_key : mismatch {
        data::string Key;
        missing_key (const data::string &k) : mismatch {}, Key {k} {}
    };

    // thrown when a key is present which is only valid in some alternative
    // rule that doesn't match the whole map or when an optional expression
    // is partially matched.
    struct incomplete_match : mismatch {
        data::string Key;
        incomplete_match (const data::string &k): mismatch {}, Key {k} {}
    };

    // thrown when a map has keys that are not provided by the schema.
    struct unknown_key : mismatch {
        data::string Key;
        unknown_key (const data::string &k) : mismatch {}, Key {k} {}
    };

    // we work with data::map or std::map.
    template <typename ...context, rule::String string, typename X>
    typename rule::validate<X, context...>::result validate (map<string, string> m, const rule::map<X> &r);

    template <typename ...context, typename X>
    typename rule::validate<X, context...>::result validate (const std::map<string, string> &m, const rule::map<X> &r);

    // this is the end of the user interface. Next comes definitions of functions.
    typename rule::map<rule::empty> inline empty () {
        return rule::map<rule::empty> {};
    }

    // make a rule that says a map has a given key.
    template <typename X> typename rule::map<rule::only<rule::value<X>>> inline key (const string &key) {
        return rule::map<rule::only<rule::value<X>>> {rule::map<rule::value<X>> {key}};
    }

    template <typename ...context, rule::String string, typename X>
    typename rule::validate<X, context...>::result inline validate (map<string, string> m, const rule::map<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }
    // we don't really support std::map yet.
/*
    template <typename ...context, typename X>
    typename rule::validate<X, context...>::result inline validate (const std::map<string, string> &m, const rule::map<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }*/
}

namespace data::schema::rule {

    template <typename ...context> 
    template <String string>
    typename validate<empty, context...>::result inline 
    validate<empty, context...>::operator () (data::map<string, string> m, map<empty> r) {
        auto b = m.begin ();
        if (b == m.end ()) return;
        else throw unknown_key {data::string (b->Key)};
    }

    template <typename ...context>
    template <String string>
    typename validate<empty, context...>::result inline
    validate<empty, context...>::operator () (const std::map<string, string> &m, map<empty> r) {
        auto b = m.begin ();
        if (b == m.end ()) return;
        else throw unknown_key {data::string (b->first)};
    }

    template <typename ...context> 
    template <typename Map>
    typename validate<blank, context...>::result inline
    validate<blank, context...>::operator () (const Map &m, map<blank> r) {}

    template <typename X, typename ...context>
    template <String string>
    typename validate<value<X>, context...>::result inline
    validate<value<X>, context...>::operator () (data::map<string, string> m, const map<value<X>> &r) {
        const string *v = m.contains (r.Key);
        if (!bool (v)) throw missing_key {data::string (r.Key)};
        maybe<X> x = encoding::read<X, context...> {} (*v);
        if (!bool (x)) throw invalid_value {*v};
        return *x;
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
            if (m.find (k) != m.end ()) throw mismatch {k};
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
        void operator () (const data::map<string, string> &m, const map<any<X...>> &r) const {
            for (const data::string &k: get_alternative_keys<X...> {}.template operator ()<index> (r))
                if (m.contains (k)) throw incomplete_match {k};
        }
    };

    template <size_t index, String string, typename ...X>
    struct has_no_unused_alternatives<index, std::map<string, string>, X...> {
        void operator () (const std::map<string, string> &m, const map<any<X...>> &r) const {
            for (const data::string &k: get_alternative_keys<X...> {}.template operator ()<index> (r))
                if (m.find (k) != m.end ()) throw incomplete_match {k};
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

    template <typename X> 
    map<typename apply_blank<X>::result> inline
    operator + (const map<X> &m) {
        return map<typename apply_blank<X>::result> {m};
    }

    template <typename X>
    map<typename apply_only<X>::result> inline
    operator - (const map<X> &m) {
        return map<typename apply_only<X>::result> {m};
    }

    template <typename X> 
    map<typename apply_optional<only<X>>::result> inline
    operator * (const map<only<X>> &m) {
        return map<typename apply_optional<only<X>>::result> {m};
    }

    template <typename X, typename Y> 
    map<typename intersect<X, Y>::result> inline
    operator & (const map<X> &a, const map<Y> &b) {
        return intersect<X, Y> {} (a, b);
    }

    template <typename X, typename Y> 
    map<typename unite<X, Y>::result> inline 
    operator | (const map<X> &a, const map<Y> &b) {
        return unite<X, Y> {} (a, b);
    }

    template <typename X, typename Y>
    map<typename intersect<X, Y>::result> inline intersect<X, Y>::operator () (const map<X> &a, const map<Y> &b) {
        return map<typename intersect::result> {a, b};
    }

    template <typename X, typename Y>
    map<typename intersect<only<X>, only<Y>>::result> inline
    intersect<only<X>, only<Y>>::operator () (const map<only<X>> &a, const map<only<Y>> &b) {
        return map<result> {intersect<X, Y> {} (static_cast<const map<X> &> (a), static_cast<const map<Y> &> (b))};
    }

    template <typename X, typename Y>
    map<typename intersect<only<X>, Y>::result> inline intersect<only<X>, Y>::operator () (const map<only<X>> &a, const map<Y> &b) {
        return map<result> {static_cast<const map<X> &> (a), b};
    }

    template <typename X, typename Y>
    map<typename intersect<X, only<Y>>::result> inline intersect<X, only<Y>>::operator () (const map<X> &a, const map<only<Y>> &b) {
        return map<result> {a, static_cast<const map<X> &> (b)};
    }

    template <typename ...X, typename ...Y>
    map<typename intersect<all<X...>, all<Y...>>::result> inline intersect<all<X...>, all<Y...>>::operator ()
        (const map<all<X...>> &a, const map<all<Y...>> &b) {
        return map<result> {std::tuple_cat (a, b)};
    }

    template <typename ...X, typename Y>
    map<typename intersect<all<X...>, Y>::result> inline intersect<all<X...>, Y>::operator () (const map<all<X...>> &a, const map<Y> &b) {
        using tuple_type_A = tuple<map<X>...>;
        return map<result> {std::tuple_cat (static_cast<tuple_type_A> (a), tuple<map<Y>> (b))};
    }

    template <typename X, typename ...Y>
    map<typename intersect<X, all<Y...>>::result> inline intersect<X, all<Y...>>::operator () (const map<X> &a, const map<all<Y...>> &b) {
        using tuple_type_B = tuple<map<Y>...>;
        return map<result> {std::tuple_cat (tuple<map<X>> (a), static_cast<tuple_type_B> (b))};
    }

    template <typename Y>
    map<typename intersect<empty, Y>::result> inline intersect<empty, Y>::operator () (map<empty> a, const map<Y> &b) {
        return b;
    }

    template <typename X>
    map<typename intersect<X, empty>::result> inline intersect<X, empty>::operator () (const map<X> &a, map<empty> b) {
        return a;
    }

    template <typename X, typename Y> 
    map<typename unite<X, Y>::result> inline unite<X, Y>::operator () (const map<X> &a, const map<Y> &b) {
        return map<result> {a, b};
    }

    template <typename X, typename Y>
    map<typename unite<only<X>, only<Y>>::result> inline
    unite<only<X>, only<Y>>::operator () (const map<only<X>> &a, const map<only<Y>> &b) {
        return map<result> {unite<X, Y> {} (static_cast<const map<X> &> (a), static_cast<const map<Y> &> (b))};
    }

    template <typename ...X, typename Y>
    map<typename unite<any<X...>, Y>::result> inline
    unite<any<X...>, Y>::operator () (const map<any<X...>> &a, const map<Y> &b) {
        using tuple_type_A = tuple<map<X>...>;
        return map<result> {std::tuple_cat (static_cast<tuple_type_A> (a), tuple<map<Y>> (b))};
    }

    template <typename X, typename ...Y>
    map<typename unite<X, any<Y...>>::result> inline
    unite<X, any<Y...>>::operator () (const map<X> &a, const map<any<Y...>> &b) {
        using tuple_type_B = tuple<map<Y>...>;
        return map<result> {std::tuple_cat (tuple<map<X>> (a), static_cast<tuple_type_B> (b))};
    }

}

#endif
