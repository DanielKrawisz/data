// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_LIFT
#define DATA_LIFT

#include <data/list.hpp>
#include <data/tree.hpp>
#include <data/map.hpp>
#include <data/cycle.hpp>
#include <data/array.hpp>
#include <data/cross.hpp>
#include <data/for_each.hpp>
#include <data/io/unimplemented.hpp>

namespace data {
    
    // Take a function fun and some lists {x, ...}, {y, ...}, {z, ...} ... and return {f (x, y, z, ...), ...}
    template <typename F, Sequence Val, Sequence... Vals>
    auto lift (F fun, Val list, Vals... lists) -> stack<decltype (fun (first (list), first (lists)...))>;

    template <typename F, typename X, typename... Xs>
    auto lift (F fun, const list<X> &ls, list<Xs>... lss) -> list<decltype (fun (first (ls), first (lss)...))>;

    template <typename F, typename K, typename V, typename... Vs>
    auto lift (F fun, const map<K, V> &m, map<K, Vs>... ms) -> map<K, decltype (fun (m[K {}], ms[K {}]...))>;

    template <typename F, typename X, typename... Xs>
    auto lift (F fun, const tree<X> &tr, tree<Xs>... trs) -> tree<decltype (fun (tr.root (), trs.root ()...))>;

    template <typename F, typename X, typename... Xs>
    auto lift (F fun, const cross<X> &vec, cross<Xs>... vecs) -> cross<decltype (fun (vec[0], vecs[0]...))>;

    template <typename F, typename X, size_t ...sizes, typename... Xs>
    auto lift (F fun, const array<X, sizes...> &arr, array<Xs, sizes...>... arrs) -> array<decltype (fun (arr[0], arrs[0]...)), sizes...>;

    template <typename F, typename... Xs>
    auto lift (F fun, cycle<Xs>... cycles) -> cycle<decltype (fun (cycles.head ()...))>;

    template <typename F, Sequence Val, Sequence... Vals>
    auto lift (F fun, Val list, Vals... lists) -> stack<decltype (fun (first (list), first (lists)...))> {
        using result_type = decltype (fun (first (list), first (lists)...));
        size_t sizes = data::size (list);
        if (!((sizes == data::size (lists)) && ...)) throw exception {} << "unequal sizes";
        function<stack<result_type> (F, Val, Vals...)> inner = [&inner] (F fun, Val list, Vals... lists) {
            if (data::empty (list)) return stack<result_type> {};
            result_type x = fun (data::first (list), data::first (lists)...);
            return data::prepend (inner (fun, data::rest (list), data::rest (lists)...), x);
        };
        return inner (fun, list, lists...);

    }

    template <typename F, typename X, typename... Xs>
    auto lift (F fun, const list<X> &ls, list<Xs>... lss) -> list<decltype (fun (first (ls), first (lss)...))> {
        using result_type = decltype (fun (first (ls), first (lss)...));
        list<result_type> result;
        for_each ([&result, &fun] (const X &x, const Xs &...xs) {
            result <<= fun (x, xs...);
        }, ls, lss...);
        return result;
    }

    template <typename F, typename... Xs>
    auto inline lift (F fun, cycle<Xs>... cycles) -> cycle<decltype (fun (cycles.head ()...))> {
        using result_type = decltype (fun (cycles.head ()...));
        return cycle<result_type> {lift (fun, (cycles.Cycle)...)};
    }

    template <typename F, typename X, typename... Xs>
    auto lift (F fun, const cross<X> &vec, cross<Xs>... vecs) -> cross<decltype (fun (vec[0], vecs[0]...))> {
        using result_type = decltype (fun (vec[0], vecs[0]...));
        size_t sizes = data::size (vec);
        if (!((sizes == data::size (vecs)) && ...)) throw exception {} << "unequal sizes";
        cross<result_type> res (sizes);
        for_each_by ([&res, &fun] (size_t index, const X &x, const Xs &...xs) {
            res[index] = fun (x, xs...);
        }, vec, vecs...);
        return res;
    }

    template <typename F, typename X, size_t ...sizes, typename... Xs>
    auto lift (F fun, const array<X, sizes...> &arr, array<Xs, sizes...>... arrs) -> array<decltype (fun (arr[0], arrs[0]...)), sizes...> {
        using result_type = decltype (fun (arr[0], arrs[0]...));
        array<result_type, sizes...> res;
        for (size_t index = 0; index < array<result_type, sizes...>::Size; index++)
            res[index] = fun (arr.Values[index], arrs.Values[index]...);
        return res;
    }

    template <typename F, typename K, typename V, typename... Vs>
    auto lift (F fun, const map<K, V> &m, map<K, Vs>... ms) -> map<K, decltype (fun (m[K {}], ms[K {}]...))> {
        using result_type = decltype (fun (m[K {}], ms[K {}]...));
        size_t sizes = data::size (m);
        if (!((sizes == data::size (ms)) && ...)) throw exception {} << "unequal sizes";
        if (sizeof...(ms) != 0) throw method::unimplemented {"lift maps with multiple inputs"};
        map<K, result_type> result;
        for (const auto &[key, val]: m) result = result.insert (key, fun (val));
        return result;
    }

    template <typename F, typename X, typename... Xs>
    auto lift (F fun, const tree<X> &tr, tree<Xs>... trs) -> tree<decltype (fun (tr.root (), trs.root ()...))> {
        using result_type = decltype (fun (tr.root (), trs.root ()...));
        size_t sizes = data::size (tr);
        if (!((sizes == data::size (trs)) && ...)) throw exception {} << "unequal sizes";
        if (sizeof...(trs) != 0) throw method::unimplemented {"lift tree with multiple inputs"};
        if (empty (tr)) return tree<result_type> {};
        return tree<result_type> {fun (tr.root ()), lift (std::forward<F> (fun), tr.left ()), lift (std::forward<F> (fun), tr.right ())};
    }

}

#endif
