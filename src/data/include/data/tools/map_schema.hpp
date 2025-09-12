
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

    // match a rule or match its nonexistence.
    template <typename X> struct optional;

    // specifies that a map may have anything else.
    template <typename X> struct blank;

    // specifies that a map has a given value.
    template <std::default_initializable X> struct value;

    template <typename ...X> struct all;

    template <typename ...X> struct any;

    // here is some stuff that says how optional composes.
    template <typename X> struct apply_optional {
        using result = optional<X>;
    };

    template <typename X> struct apply_optional<blank<X>> {
        using result = optional<X>;
    };

    template <> struct apply_optional<empty> {
        using result = empty;
    };

    // here is some stuff that says how blank composes.
    template <typename X> struct apply_blank {
        using result = blank<X>;
    };

    template <typename X> struct apply_blank<blank<X>> {
        using result = blank<X>;
    };

    // here is some stuff that says how blank composes.
    template <typename val, typename X> struct add_key {
        using result = all<X, value<val>>;
    };

    template <typename val> struct add_key<val, empty> {
        using result = value<val>;
    };

    template <typename val, typename ...X> struct add_key<val, all<X...>> {
        using result = all<X..., value<val>>;
    };

    template <typename val, typename ...X> struct add_key<val, blank<all<X...>>> {
        using result = blank<all<X..., value<val>>>;
    };

    template <typename X, typename Y> struct intersect {
        using result = all<X, Y>;
        map<result> operator () (const map<X> &, const map<Y> &);
    };

    template <typename Y> struct intersect<empty, Y> {
        using result = Y;
        map<result> operator () (map<empty> a, const map<Y> &b);
    };

    template <typename X> struct intersect<X, empty> {
        using result = X;
        map<result> operator () (const map<X> &a, map<empty> b);
    };

    template <typename ...X, typename Y> struct intersect<all<X...>, Y> {
        using result = all<X..., Y>;
        map<result> operator () (const map<all<X...>> &, const map<Y> &);
    };

    template <typename X, typename ...Y> struct intersect<X, all<Y...>> {
        using result = all<X, Y...>;
        map<result> operator () (const map<X> &, const map<all<Y...>> &);
    };

    template <typename ...X, typename ...Y> struct intersect<all<X...>, all<Y...>> {
        using result = all<X..., Y...>;
        map<result> operator () (const map<all<X...>> &, const map<all<Y...>> &);
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

    template <typename X, typename Y> struct intersect<blank<X>, Y> {
        using result = blank<typename intersect<X, Y>::result>;
        map<result> operator () (const map<blank<X>> &, const map<Y> &);
    };

    template <typename X, typename Y> struct intersect<X, blank<Y>> {
        using result = blank<typename intersect<X, Y>::result>;
        map<result> operator () (const map<X> &, const map<blank<Y>> &);
    };

    template <typename X, typename Y> struct intersect<blank<X>, blank<Y>> {
        using result = blank<typename intersect<X, Y>::result>;
        map<result> operator () (const map<blank<X>> &, const map<blank<Y>> &);
    };

    // now we have some definitions of types which we declared earlier.
    template <> struct map<empty> {};

    template <typename X> struct map<value<X>> {
        string Key;
    };

    template <typename X> struct map<blank<X>> : map<X> {
        using map<X>::map;
        map (const map<X> &m): map<X> {m} {}
    };

    template <typename X> struct map<optional<X>> : map<X> {
        using map<X>::map;
        map (const map<X> &m): map<X> {m} {}
    };

    template <typename X, typename ...Y> struct map<all<X, Y...>> : tuple<map<X>, map<Y>...> {
        using tuple<map<X>, map<Y>...>::tuple;
    };

    template <typename X, typename ...Y> struct map<any<X, Y...>> : tuple<map<X>, map<Y>...> {
        using tuple<map<X>, map<Y>...>::tuple;
    };

    // now we know how to create rules. Next follows some stuff the user doesn't
    // need to read that is for validating rules.
    template <typename X, typename ...context> struct validate;

    template <typename string> concept String = std::convertible_to<string, data::string> && std::equality_comparable_with<data::string, string>;

    template <typename ...context> struct validate<empty, context...> {
        using result = void;

        template <typename Map>
        result operator () (const Map &m, map<empty> r);
    };

    template <typename ...context> struct validate<blank<empty>, context...> {
        using result = void;

        template <typename Map>
        result operator () (const Map &m, map<blank<empty>> r);
    };

    template <typename X, typename ...context> struct validate<blank<value<X>>, context...> {
        using result = entry<string, X>;

        template <String string>
        result operator () (data::map<string, string> m, const map<blank<value<X>>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<blank<value<X>>> &r);
    };

    template <typename X, typename ...context> struct validate<value<X>, context...> {
        using result = entry<string, X>;

        template <typename Map>
        result operator () (const Map &m, const map<value<X>> &r);
    };

    template <typename X, typename ...context> struct validate<blank<optional<X>>, context...> {
        using result = maybe<typename validate<X, context...>::result>;

        template <String string>
        result operator () (data::map<string, string> m, const map<blank<optional<X>>> &r);

        template <String string>
        result operator () (const std::map<string, string> &m, const map<blank<optional<X>>> &r);
    };

    template <typename X, typename ...context> struct validate<optional<blank<X>>, context...> {
        using result = maybe<typename validate<X, context...>::result>;
        template <typename Map>
        result operator () (const Map &m, const map<optional<blank<X>>> &r);
    };

    template <typename X, typename ...context> struct validate<optional<X>, context...> {
        using result = maybe<typename validate<X, context...>::result>;
        template <typename Map>
        result operator () (const Map &m, const map<optional<X>> &r);
    };

    template <typename ...X, typename ...context> struct validate<blank<all<X...>>, context...> {
        using result = tuple<typename validate<X, context...>::result...>;
        template <typename Map>
        result operator () (const Map &m, const map<blank<all<X...>>> &r);
    };

    template <typename ...X, typename ...context> struct validate<blank<any<X...>>, context...> {
        using result = either<typename validate<X, context...>::result...>;
        template <String string>
        result operator () (data::map<string, string> m, const map<blank<any<X...>>> &r);
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

    // make a rule that says a map has a given key.
    template <typename X> typename rule::map<rule::value<X>> key (const string &key);
}

namespace data::schema::rule {

    // use the * operator to make any rule optional.
    template <typename X> map<typename apply_optional<X>::result> operator * (const map<X> &);

    // use the + operator to allow a rule to match with extra values.
    template <typename X> map<typename apply_blank<X>::result> operator + (const map<X> &);

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
}

namespace data::schema::rule {

    // This is the end of the user interface.

    // check that no other keys are present.
    template <typename X> struct no_other_keys {
        template <typename Map>
        void operator () (const Map &m, const map<X> &r);
    };

    // this should never happen, therefore we create it with no function. 
    template <typename ...X> struct no_other_keys<any<X...>> {};

    template <typename ...context> 
    template <typename Map>
    typename validate<empty, context...>::result inline 
    validate<empty, context...>::operator () (const Map &m, map<empty> r) {
        no_other_keys<empty> {} (m, r);
    }

    template <typename ...context> 
    template <typename Map>
    typename validate<blank<empty>, context...>::result inline 
    validate<blank<empty>, context...>::operator () (const Map &m, map<blank<empty>> r) {}

    template <typename X, typename ...context> 
    template <typename Map>
    typename validate<value<X>, context...>::result inline 
    validate<value<X>, context...>::operator () (const Map &m, const map<value<X>> &r) {
        auto val = validate<blank<value<X>>, context...> {} (m, +r);
        no_other_keys<value<X>> {} (m, r);
        return val;
    }

    template <typename X, typename ...context> 
    template <String string>
    typename validate<blank<value<X>>, context...>::result inline 
    validate<blank<value<X>>, context...>::operator () (data::map<string, string> m, const map<blank<value<X>>> &r) {
        std::cout << " validate blank value..." << std::endl;
        const string *v = m.contains (r.Key);
        if (bool (v)) {
            maybe<X> x = encoding::read<X, context...> {} (*v);
            if (!bool (x)) throw invalid_value {*v};
            
            return {r.Key, *x};
        }
        throw missing_key {data::string (r.Key)};
    }

    template <typename X, typename ...context> 
    template <String string>
    typename validate<blank<optional<X>>, context...>::result inline
    validate<blank<optional<X>>, context...>::operator () (data::map<string, string> m, const map<blank<optional<X>>> &r) {
        try {
            return {validate<blank<X>, context...> {} (m, +static_cast<map<X>> (r))};
        } catch (const mismatch &) {
            return {};
        }
    }

    template <typename X, typename ...context> 
    template <typename Map>
    typename validate<optional<X>, context...>::result inline
    validate<optional<X>, context...>::operator () (const Map &m, const map<optional<X>> &r) {
        result val;
        try {
            val = validate<blank<X>, context...> {} (m, +static_cast<map<X>> (r));
        } catch (const mismatch &) {}
        no_other_keys<X> {} (m, static_cast<map<X>> (r));
        return val;
    }
    
    template <typename X, typename ...context> 
    template <typename Map>
    typename validate<optional<blank<X>>, context...>::result inline
    validate<optional<blank<X>>, context...>::operator () (const Map &m, const map<optional<blank<X>>> &r) {
        try {
            return validate<blank<X>, context...> {} (m, +static_cast<map<X>> (r));
        } catch (const mismatch &) {}
        no_other_keys<X> {} (m, static_cast<map<X>> (r));
        return {};
    }

    template <typename ...X, typename ...context> 
    template <typename Map>
    typename validate<all<X...>, context...>::result inline 
    validate<all<X...>, context...>::operator () (const Map &m, const map<all<X...>> &r) {
        auto val = validate<blank<all<X...>>, context...> {} (m, +r);
        no_other_keys<all<X...>> {} (m, r);
        return val;
    }

    template <typename X> struct get_rule;

    template <typename X> struct get_rule<map<X>> {
        using type = X;
    };

    template <typename ...X, typename ...context> 
    template <typename Map>
    typename validate<blank<all<X...>>, context...>::result inline 
    validate<blank<all<X...>>, context...>::operator () (const Map &m, const map<blank<all<X...>>> &r) {
        using tuple_type = tuple<map<X>...>;
        return std::apply ([&m] (auto &&...elems) -> result {
            return result {validate<blank<typename get_rule<unconst<unref<decltype (elems)>>>::type>, context...> {} (m, +elems)...};
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

}

namespace data::schema {
    template <typename ...context, rule::String string, typename X>
    typename rule::validate<X, context...>::result inline validate (map<string, string> m, const rule::map<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }
/*
    template <typename ...context, typename X>
    typename rule::validate<X, context...>::result inline validate (const std::map<string, string> &m, const rule::map<X> &r) {
        return rule::validate<X, context...> {} (m, r);
    }*/

    // make a rule that says a map has a given key.
    template <typename X> typename rule::map<rule::value<X>> inline key (const string &key) {
        return rule::map<rule::value<X>> {key};
    }
}

namespace data::schema::rule {
    template <typename X> 
    map<typename apply_blank<X>::result> inline 
    operator + (const map<X> &m) {
        return map<typename apply_blank<X>::result> {m};
    }

    template <typename X> 
    map<typename apply_optional<X>::result> inline 
    operator * (const map<X> &m) {
        return map<typename apply_optional<X>::result> {m};
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

    template <typename ...X, typename ...Y>
    map<typename intersect<all<X...>, all<Y...>>::result> inline intersect<all<X...>, all<Y...>>::operator ()
        (const map<all<X...>> &a, const map<all<Y...>> &b) {
        return map<result> {std::tuple_cat (a, b)};
    }

    template <typename ...X, typename Y>
    map<typename intersect<all<X...>, Y>::result> inline intersect<all<X...>, Y>::operator () (const map<all<X...>> &a, const map<Y> &b) {
        return map<result> {std::tuple_cat (a, tuple<map<Y>> (b))};
    }

    template <typename X, typename ...Y>
    map<typename intersect<X, all<Y...>>::result> inline intersect<X, all<Y...>>::operator () (const map<X> &a, const map<all<Y...>> &b) {
        return map<result> {std::tuple_cat (tuple<map<X>> (a), b)};
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
    map<typename intersect<X, Y>::result> inline intersect<X, Y>::operator () (const map<X> &a, const map<Y> &b) {
        return map<typename intersect::result> {a, b};
    }

    template <typename X, typename Y> 
    map<typename unite<X, Y>::result> inline unite<X, Y>::operator () (const map<X> &a, const map<Y> &b) {
        return map<result> {a, b};
    }

    template <typename X>
    struct expected_keys; 

    template <>
    struct expected_keys<empty> {
        set<string> operator () (const map<empty> &r) {
            return {};
        }
    }; 

    template <typename X>
    struct expected_keys<value<X>> {
        set<string> operator () (const map<value<X>> &r) {
            return {r.Key};
        }
    }; 

    template <typename X>
    struct expected_keys<optional<value<X>>> {
        set<string> operator () (const map<optional<value<X>>> &r) {
            return {r.Key};
        }
    }; 

    template <typename ...X>
    struct expected_keys<all<X...>> {
        template <size_t index = 0>
        set<string> operator () (const map<all<X...>> &r) {
            using tuple_type = std::tuple<map<X>...>;
            constexpr size_t tuple_size = std::tuple_size_v<tuple_type>;
            using index_type = get_rule<std::tuple_element_t<index, tuple_type>>::type;
            if constexpr (index + 1 < tuple_size) {
                return expected_keys<index_type> {} 
                    (std::get<index> (static_cast<const tuple_type &> (r))) & operator ()<index + 1> (r);
            } if constexpr (index + 1 == tuple_size) {
                return expected_keys<index_type> {} 
                    (std::get<index> (static_cast<const tuple_type &> (r)));
            } else {
                throw data::exception {} << "Invalid index parameter";
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

    template <typename X>
    template <typename Map>
    void inline no_other_keys<X>::operator () (const Map &m, const map<X> &r) {
        none_else {} (m, expected_keys<X> {} (r));
    }

}

#endif
