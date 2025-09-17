
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

    // here is some stuff that says how optional composes.
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
    };

    template <typename X, typename ...Y> struct map<any<X, Y...>> : tuple<map<X>, map<Y>...> {
        using parent = tuple<map<X>, map<Y>...>;
        using tuple<map<X>, map<Y>...>::tuple;
        map (parent &&p): parent {p} {}
    };

    template <typename X> struct map<optional<X>> : map<X> {
        using map<X>::map;
        map (const map<X> &m): map<X> {m} {}
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

    template <typename X, typename ...context> struct validate<only<X>, context...> {
        using result = typename validate<X, context...>::result;

        template <typename Map>
        result operator () (const Map &m, const map<only<X>> &r);
    };

    template <typename X, typename ...context> struct validate<optional<X>, context...> {
        using result = maybe<typename validate<X, context...>::result>;

        template <typename Map>
        result operator () (const Map &m, const map<optional<X>> &r);
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

    // use the & operator to combine two map rules.
    template <typename X, typename Y> map<typename intersect<X, Y>::result> operator & (const map<X> &, const map<Y> &);

    // use the & operator to combine two map rules.
    template <typename X, typename Y> map<typename unite<X, Y>::result> operator | (const map<X> &, const map<Y> &);
}

namespace data::schema {

    // Next we describe how to validate rules. First come some errors which
    // may be thrown when there is a mismatch.

    struct mismatch {};

    // thrown when a map has keys that are not provided by the schema.
    struct unknown_key : mismatch {
        data::string Key;
        unknown_key (const data::string &k) : mismatch {}, Key {k} {}
    };

    // thrown when a key was expected that was not available.
    struct missing_key : mismatch {
        data::string Key;
        missing_key (const data::string &k) : mismatch {}, Key {k} {}
    };

    // thrown when the value could not be read.
    struct invalid_value : mismatch {
        data::string Key;
        invalid_value (const data::string &k) : mismatch {}, Key {k} {}
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
        if (bool (v)) {
            maybe<X> x = encoding::read<X, context...> {} (*v);
            if (!bool (x)) throw invalid_value {*v};

            return *x;
        }
        throw missing_key {data::string (r.Key)};
    }

    template <typename X, typename ...context>
    template <typename Map>
    typename validate<optional<X>, context...>::result inline
    validate<optional<X>, context...>::operator () (const Map &m, const map<optional<X>> &r) {
        try {
            return {validate<X, context...> {} (m, static_cast<map<X>> (r))};
        } catch (const mismatch &) {
            return {};
        }
    }

    template <typename X> struct get_rule;

    template <typename X> struct get_rule<map<X>> {
        using type = X;
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

    template <typename ...X, typename ...context>
    template <typename Map, size_t index>
    typename validate<any<X...>, context...>::result
    validate<any<X...>, context...>::operator () (const Map &m, const map<any<X...>> &r) {
        using tuple_type = tuple<map<X>...>;
        constexpr const size_t tuple_size = std::tuple_size_v<tuple_type>;
        if constexpr (index + 1 < tuple_size) {
            try {
                return result {std::in_place_index<index>,
                    validate<typename get_rule<std::tuple_element_t<index, tuple_type>>::type, context...> {}
                    (m, std::get<index> (static_cast<const tuple_type &> (r)))};
            } catch (const mismatch &) {
                return operator ()<Map, index + 1> (m, r);
            }
        } if constexpr (index + 1 == tuple_size) {
            return result {std::in_place_index<index>,
                validate<typename get_rule<std::tuple_element_t<index, tuple_type>>::type, context...> {}
                (m, std::get<index> (static_cast<const tuple_type &> (r)))};
        } else {
            throw data::exception {} << "Invalid index parameter";
        }
    }

    template <typename X, typename ...context> struct validate_exact {
        template <typename Map>
        typename validate<X, context...>::result operator () (const Map &m, const map<X> &r, set<string> &) {
            return validate<X, context...> {} (m, r);
        }
    };

    template <typename X, typename ...context> struct validate_exact<value<X>, context...> {
        template <typename Map>
        typename validate<value<X>, context...>::result operator () (const Map &m, const map<value<X>> &r, set<string> &x) {
            x = x.insert (r.Key);
            return validate<value<X>, context...> {} (m, r);
        }
    };

    template <typename X, typename ...context> struct validate_exact<optional<X>, context...> {
        template <typename Map>
        typename validate<optional<X>, context...>::result operator () (const Map &m, const map<optional<X>> &r, set<string> &x) {
            using result = validate<optional<X>, context...>::result;
            set<string> next = x;
            try {
                result res {validate_exact<X, context...> {} (m, static_cast<map<X>> (r), next)};
                x = next;
                return res;
            } catch (const mismatch &) {
                return {};
            }
        }
    };

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

    template <typename ...X, typename ...context> struct validate_exact<any<X...>, context...> {
        template <typename Map, size_t index = 0>
        typename validate<any<X...>, context...>::result operator () (const Map &m, const map<any<X...>> &r, set<string> &x) {
            using result = validate<any<X...>, context...>::result;
            using tuple_type = tuple<map<X>...>;
            constexpr const size_t tuple_size = std::tuple_size_v<tuple_type>;
            using rule_type = get_rule<std::tuple_element_t<index, tuple_type>>::type;
            if constexpr (index + 1 < tuple_size) {
                try {
                    set<string> next = x;
                    result res {std::in_place_index<index>,
                        validate_exact<rule_type, context...> {} (m, std::get<index> (static_cast<const tuple_type &> (r)), next)};
                    none_else {} (m, next);
                    x = next;
                    return res;
                } catch (const mismatch &) {
                    return operator () <Map, index + 1> (m, r, x);
                }
            } if constexpr (index + 1 == tuple_size) {
                set<string> next = x;
                result res {std::in_place_index<index>,
                    validate_exact<rule_type, context...> {} (m, std::get<index> (static_cast<const tuple_type &> (r)), next)};
                none_else {} (m, next);
                x = next;
                return res;
            } else {
                throw data::exception {} << "Invalid index parameter";
            }
        }
    };

    template <typename ...X, typename ...context> struct validate_exact<all<X...>, context...> {
        template <typename Map>
        typename validate<all<X...>, context...>::result
        operator () (const Map &m, const map<all<X...>> &r, set<string> &x) {
            using tuple_type = tuple<map<X>...>;
            using result = validate<all<X...>, context...>::result;
            return std::apply ([&m, &x] (auto &&...elems) -> result {
                return result {validate_exact<typename get_rule<unconst<unref<decltype (elems)>>>::type, context...> {} (m, elems, x)...};
            }, static_cast<const tuple_type &> (r));
        }
    };

    // TODO this has to work differently.
    template <typename X, typename ...context> 
    template <typename Map>
    typename validate<only<X>, context...>::result inline
    validate<only<X>, context...>::operator () (const Map &m, const map<only<X>> &r) {
        set<string> keys;
        auto val = validate_exact<X, context...> {} (m, +r, keys);
        none_else {} (m, keys);
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
        using tuple_type_cat = tuple<map<X>..., map<Y>>;
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

}

#endif
