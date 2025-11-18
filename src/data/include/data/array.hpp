// Copyright (c) 2019-2025 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef DATA_ARRAY
#define DATA_ARRAY

#include <data/slice.hpp>
#include <data/valid.hpp>
#include <data/stream.hpp>
#include <data/integral.hpp>
#include <data/encoding/hex.hpp>
#include <data/arithmetic/arithmetic.hpp>
#include <data/arithmetic/words.hpp>

namespace data {

    template <typename X, size_t...> struct array;

    template <std::integral word, size_t ...sizes> using bytes_array = array<word, sizes...>;

    template <size_t size> using byte_array = bytes_array<byte, size>;

    template <size_t size> byte_array<size> read_byte_array (const hex_string &h);

    template <typename X, size_t... sizes>
    constexpr bool operator == (const array<X, sizes...> &a, const array<X, sizes... > &b);

    template <typename X, size_t... sizes>
    constexpr auto operator <=> (const array<X, sizes...> &a, const array<X, sizes... > &b) -> decltype (*a.begin () <=> *b.begin ());

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ~ (const bytes_array<word, size> &b);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ^ (const bytes_array<word, size> &, const bytes_array<word, size> &);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator & (const bytes_array<word, size> &, const bytes_array<word, size> &);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator | (const bytes_array<word, size> &, const bytes_array<word, size> &);

    // bit shift
    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i);

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i);

    // write
    template <std::integral word, size_t... sizes>
    writer<word> &operator << (writer<word> &, const bytes_array<word, sizes...> &);

    template <std::integral word, size_t... sizes>
    reader<word> &operator >> (reader<word> &, bytes_array<word, sizes...> &);

    template <std::integral word, size_t size>
    std::ostream &operator << (std::ostream &o, const array<word, size> &s);

    // vector addition and subtraction.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        { x + y } -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator + (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        { x - y } -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &, const array<X, sizes...> &);

    template <typename X, size_t... sizes> requires requires (const X &x) {
        { -x } -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &);

    // scalar multiplication and division.
    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        { x * y } -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator * (const array<X, sizes...> &, const X &);

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        { x / y } -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator / (const array<X, sizes...> &, const X &);

    // matrix multiplication (also serves as a dot product for vectors)
    template <typename X, size_t A, size_t... AA, size_t B, size_t... BB>
    constexpr auto operator * (const array<X, A, AA...> &a, const array<X, B, BB...> &b);

    // this array is a structural type and therefore can be
    // used as a non-type template parameter.
    template <std::default_initializable X, size_t z> struct array<X, z> {
        constexpr static size_t Size = z;

        X Values[Size];

        constexpr array ();
        constexpr array (std::initializer_list<X> x);
        constexpr array (slice<const X, z> x) {
            std::copy (x.begin (), x.end (), this->begin ());
        }

        // make an array filled with a particular value.
        constexpr static array filled (const X &x);

        constexpr bool valid () const;

        constexpr X &operator [] (size_t i);
        constexpr const X &operator [] (size_t i) const;

        constexpr slice<X> range (int);
        constexpr slice<X> range (int, int);

        constexpr slice<const X> range (int) const;
        constexpr slice<const X> range (int, int) const;

        constexpr operator slice<X, Size> () {
            return slice<X, Size> {Values};
        }

        constexpr operator slice<const X, Size> () const {
            return slice<const X, Size> {Values};
        }

        constexpr size_t size () const {
            return Size;
        }

        constexpr X *data () {
            return Values;
        }

        constexpr const X *data () const {
            return Values;
        }

        using iterator = slice<X, Size>::iterator;
        using const_iterator = slice<const X, Size>::iterator;
        using reverse_iterator = slice<X, Size>::reverse_iterator;
        using const_reverse_iterator = slice<const X, Size>::reverse_iterator;;

        constexpr iterator begin () {
            return slice<X, Size> (*this).begin ();
        }

        constexpr iterator end () {
            return slice<X, Size> (*this).end ();
        }

        constexpr const_iterator begin () const {
            return slice<const X, Size> (*this).begin ();
        }

        constexpr const_iterator end () const {
            return slice<const X, Size> (*this).end ();
        }

        constexpr reverse_iterator rbegin () {
            return slice<X, Size> (*this).rbegin ();
        }

        constexpr reverse_iterator rend () {
            return slice<X, Size> (*this).rend ();
        }

        constexpr const_reverse_iterator rbegin () const {
            return slice<const X, Size> (*this).rbegin ();
        }

        constexpr const_reverse_iterator rend () const {
            return slice<const X, Size> (*this).rend ();
        }
    };

    template <std::default_initializable X, size_t size, size_t... sizes>
    struct array<X, size, sizes...> : public array<X, size * array<X, sizes...>::Size> {
        constexpr static size_t Size = size * array<X, sizes...>::Size;

        array ():  array<X, Size> {} {}
        array (std::initializer_list<array<X, sizes...>> z): array {} {
            if (z.size () != size) throw exception {} << "invalid size " << z.size ();
            int i = 0;
            for (const array<X, sizes...> &a : z) {
                std::copy (a.Values, a.Values + array<X, sizes...>::Size, this->Values + i);
                i += array<X, sizes...>::Size;
            }
        }

        template <size_t size1>
        static size_t get_index (size_t i) {
            return i;
        }

        template <size_t size1, size_t size2, size_t ...remaining, typename ...Sizes>
        static size_t get_index (size_t i, size_t j, Sizes... z) {
            return i * array<X, size2, remaining...>::Size + get_index<size2, remaining...> (j, z...);
        }

        template <typename ...Sizes> X &operator [] (size_t i, Sizes... j) {
            return this->Values[get_index<size, sizes...> (i, j...)];
        }

        template <typename ...Sizes> const X &operator [] (size_t i, Sizes... j) const {
            return this->Values[get_index<size, sizes...> (i, j...)];
        }

    };

    template <std::default_initializable X> struct array<X> {
        constexpr static size_t Size = 1;

        X Value;

        constexpr array (): Value {} {}
        constexpr array (const X &x): Value {x} {}

        array &operator = (const X &x) {
            Value = x;
            return *this;
        }

        constexpr array (X &&x): Value {std::move (x)} {}

        constexpr array &operator = (X &&x) {
            Value = std::move (x);
            return *this;
        }

        constexpr X &operator [] () {
            return Value;
        }

        constexpr const X &operator [] () const {
            return Value;
        }

        constexpr operator X () const;

        constexpr bool valid () const;

        explicit operator slice<X, 1> ();

        slice<X> range (int);
        slice<X> range (int, int);

        explicit operator slice<const X, 1> () const;

        slice<const X> range (int) const;
        slice<const X> range (int, int) const;

        constexpr slice<X>::iterator begin () {
            return slice<X> {&Value, 1}.begin ();
        }

        constexpr slice<X>::iterator end () {
            return slice<X> {&Value, 1}.end ();
        }

        constexpr slice<const X>::iterator begin () const {
            return slice<const X> {&Value, 1}.begin ();
        }

        constexpr slice<const X>::iterator end () const {
            return slice<const X> {&Value, 1}.end ();
        }

    };

    // everything beyond this point is a complete mess.

    template<class T>
    struct array_info;

    template<class X, size_t... Ds>
    struct array_info<array<X, Ds...>> {
        using dims = std::index_sequence<Ds...>;
        using field = X;
        constexpr static const size_t order = sizeof... (Ds);
    };

    template<class T>
    struct make_array;

    template<size_t... Ds>
    struct make_array<std::index_sequence<Ds...>> {
        constexpr static const array<size_t, sizeof...(Ds)> result {Ds...};
    };

    template <typename Seq>
    struct seq_to_array_params;

    template <size_t... Sizes>
    struct seq_to_array_params<std::integer_sequence<size_t, Sizes...>> {
        template <typename X>
        using apply = array<X, Sizes...>;
    };

    struct contracted_indices {
        size_t First;
        size_t Second;
    };

    template <contracted_indices...>
    struct unequal_contracted_indices;

    template <>
    struct unequal_contracted_indices<> {
        constexpr static const bool value = true;
    };

    template <contracted_indices last>
    struct unequal_contracted_indices<last> {
        constexpr static const bool value = last.First != last.Second;
    };

    template <contracted_indices first, contracted_indices second, contracted_indices ...rest>
    struct unequal_contracted_indices<first, second, rest...> {
        constexpr static const bool value =
            first.First != first.Second &&
            first.First != second.First &&
            first.Second != second.First && unequal_contracted_indices<second, rest...>::value;
    };

    // calculate an arbitrary contraction expression
    //   contract<{a, b}, {c, d}...> {} (m1, m2...);
    // the result is equivalent to the outer product of m1, m2 ...
    // with index a contracted over with index b, index c contracted
    // with index d, etc.
    template <contracted_indices... contracted>
    struct contract {

        struct counter {
            size_t Index;
            size_t Limit;

            friend std::ostream &operator << (std::ostream &o, const counter &c) {
                o << "{" << c.Index << ": " << c.Limit << "}";
                return o;
            }
        };

        // the contraction is valid if none of the contracted indices are equal to one another
        // if all the matrices are over the same field, and if each pair of contracted indices
        // has the same dimension as its mate.
        template <typename ...arrays>
        requires (unequal_contracted_indices<contracted...>::value &&
            Same<typename array_info<arrays>::field...> && (... && (
                ( meta::get<typename meta::concat<typename array_info<arrays>::dims...>::type, contracted.First>::value ==
                meta::get<typename meta::concat<typename array_info<arrays>::dims...>::type, contracted.Second>::value ))))
        auto operator () (arrays... m) {

            using index_limits = meta::concat<typename array_info<arrays>::dims...>::type;
            auto index_limits_array = make_array<index_limits>::result;
            constexpr const size_t total_input_indices = meta::count<index_limits>::result;
            // all indices for all input matricies put together.
            counter *input_indices[total_input_indices];
            for (int i = 0; i < total_input_indices; i++) input_indices[i] = nullptr;

            constexpr const size_t total_contracted_indices = sizeof... (contracted);
            counter contract_indices[total_contracted_indices];

            // initialize contract_indices and initialize the
            // appropriate input_indices to point to them.
            {
                counter *next = contract_indices;
                ([&] (contracted_indices contract) {
                    *next = {0, index_limits_array[contract.First]};
                    input_indices[contract.First] = next;
                    input_indices[contract.Second] = next;
                    next++;
                } (contracted), ...);

            }

            // get underlying field
            using field = std::tuple_element_t<0, std::tuple<typename array_info<arrays>::field...>>;

            using result_index_limits = meta::remove_at<index_limits,
                typename meta::sort<std::index_sequence<contracted.First..., contracted.Second...>>::result>::result;

            constexpr const size_t total_result_indices = meta::count<result_index_limits>::result;
            counter result_indices[total_result_indices];

            // initialize the result_indices and initialize input_indices to point to
            // the result indices, skipping those that are already initialized.
            {
                counter *next_result = result_indices;
                counter **next_input = input_indices;
                meta::for_each<result_index_limits> {} ([&](size_t limit) {
                    *next_result = {0, limit};
                    while (*next_input != nullptr) next_input++;
                    *next_input = next_result;
                    next_result++;
                    next_input++;
                });
            }

            // the return type is an array with the indices of input matrices
            // without those marked to be contracted.
            using return_type = seq_to_array_params<result_index_limits>::template apply<field>;

            constexpr const size_t number_of_inputs = sizeof... (arrays);

            return_type result {};

            size_t result_index_counter = 0;
            while (true) {

                // retrieve the element of the result which
                // corresponds to the state of result_indices.
                field *contract_result;
                {
                    size_t result_index = 0;
                    for (size_t i = 0; i < total_result_indices; i++) {
                        result_index *= result_indices[i].Limit;
                        result_index += result_indices[i].Index;
                    }
                    contract_result = result.Values + result_index;
                }

                // initialize this element to zero.
                *contract_result = field {};

                // reset contract indices
                for (int i = 0; i < total_contracted_indices; i++)
                    contract_indices[i].Index = 0;

                size_t contract_index_counter = 0;
                do {

                    field multiplication_result {1};

                    // for each input matrix, retrieve the value
                    // corresponding to the input indices and
                    // add it to the result.
                    size_t input_index_counter = 0;
                    ([&](const field *values, size_t order) {
                        size_t value_index = 0;
                        while (order > 0) {
                            value_index *= input_indices[input_index_counter]->Limit;
                            value_index += input_indices[input_index_counter]->Index;
                            input_index_counter++;
                            order--;
                        }
                        multiplication_result *= values[value_index];
                    }(m.Values, array_info<decltype (m)>::order), ...);

                    // add result to final
                    *contract_result += multiplication_result;

                    // increment contracted indices
                    while (contract_index_counter != total_contracted_indices) {
                        counter *contracted_counter = contract_indices + contract_index_counter;
                        contracted_counter->Index++;
                        if (contracted_counter->Index != contracted_counter->Limit) {
                            contract_index_counter = 0;
                            break;
                        }

                        contracted_counter->Index = 0;
                        contract_index_counter++;
                    }
                } while (contract_index_counter != total_contracted_indices);

                // increment result_indices.
                while (true) {
                    if (result_index_counter == total_result_indices)
                        return result;

                    counter *result_counter = result_indices + result_index_counter;
                    result_counter->Index++;
                    if (result_counter->Index != result_counter->Limit) {
                        result_index_counter = 0;
                        break;
                    }

                    result_counter->Index = 0;
                    result_index_counter++;
                }
            }
        }
    };

    // matrix multiplication (also serves as a dot product for vectors)
    template <typename X, size_t A, size_t... AA, size_t B, size_t... BB>
    constexpr auto inline operator * (const array<X, A, AA...> &a, const array<X, B, BB...> &b) {
        return contract<{sizeof... (AA), sizeof... (AA) + 1}> {} (a, b);
    }

    template <std::default_initializable X, size_t size>
    constexpr inline array<X, size>::array () {
        for (size_t index = 0; index < Size; index++) Values[index] = X {};
    }

    template <std::default_initializable X, size_t size>
    constexpr inline array<X, size>::array (std::initializer_list<X> x) {
        if (x.size () != Size) throw exception {} << "out of range";
        size_t index = 0;
        for (const X &xx : x) Values[index++] = xx;
    }

    template <typename X, size_t... sizes>
    constexpr bool inline operator == (const array<X, sizes...> &a, const array<X, sizes... > &b) {
        auto i = a.begin ();
        auto j = b.begin ();
        while (i != a.end ()) {
            if (*i != *j) return false;
            i++;
            j++;
        }
        return true;
    }

    template <typename X, size_t... sizes>
    constexpr auto operator <=> (const array<X, sizes...> &a, const array<X, sizes... > &b) -> decltype (*a.begin () <=> *b.begin ()) {
        auto i = a.begin ();
        auto j = b.begin ();

        // For certain types of comparison we can have equivalent
        // values that are not equal so we keep track of a running
        // value for that case.
        using comparison = decltype (*i <=> *j);
        comparison so_far = comparison::equal;

        while (i != a.end ()) {
            auto comp = *i <=> *j;
            if (comp == comparison::greater || comp == comparison::less) return comp;
            if (comp != comparison::equal) so_far = comp;
            i++;
            j++;
        }

        return so_far;
    }

    template <std::default_initializable X, size_t size>
    constexpr array<X, size> inline array<X, size>::filled (const X &z) {
        array n {};
        for (X &x: n.Values) x = z;
        return n;
    }

    template <std::default_initializable X, size_t size>
    constexpr bool inline array<X, size>::valid () const {
        for (const X &x : *this) if (!data::valid (x)) return false;
        return true;
    }

    template <std::default_initializable X, size_t size>
    constexpr X inline &array<X, size>::operator [] (size_t i) {
        if (Size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= Size) return this->operator [] ((i + Size) % Size);
        return Values[i];
    }

    template <std::default_initializable X, size_t size>
    constexpr const X inline &array<X, size>::operator [] (size_t i) const {
        if (Size == 0) throw std::out_of_range {"cross size 0"};
        if (i < 0 || i >= Size) return this->operator [] ((i + Size) % Size);
        return Values[i];
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x + y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator + (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        array<X, sizes...> x {};
        auto ai = a.begin ();
        auto bi = b.begin ();

        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai + *bi;
            ai++;
            bi++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x) {
        {-x} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &x) {
        array<X, sizes...> result {};
        auto xi = x.begin ();

        for (auto ri = result.begin (); ri != result.end (); ri++) {
            *ri = -*xi;
            xi++;
        }

        return result;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x - y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator - (const array<X, sizes...> &a, const array<X, sizes...> &b) {
        array<X, sizes...> x {};
        auto ai = a.begin ();
        auto bi = b.begin ();

        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai - *bi;
            ai++;
            bi++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x * y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator * (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai * b;
            ai++;
        }

        return x;
    }

    template <typename X, size_t... sizes> requires requires (const X &x, const X &y) {
        {x / y} -> ImplicitlyConvertible<X>;
    } constexpr array<X, sizes...> operator / (const array<X, sizes...> &a, const X &b) {
        array<X, sizes...> x {};

        auto ai = a.begin ();
        for (auto xi = x.begin (); xi != x.end (); xi++) {
            *xi = *ai / b;
            ai++;
        }

        return x;
    }

    template <std::integral word, size_t... sizes>
    writer<word> inline &operator << (writer<word> &w, const bytes_array<word, sizes...> &x) {
        w.write (x.data (), (1 * sizes)...);
        return w;
    }

    template <std::integral word, size_t... sizes>
    reader<word> &operator >> (reader<word> &r, bytes_array<word, sizes...> &x) {
        r.read (x.data (), (1 * sizes)...);
        return r;
    }

    template <std::integral word, size_t size>
    std::ostream inline &operator << (std::ostream &o, const bytes_array<word, size> &s) {
        return o << "\"" << encoding::hex::write (slice<const word> (s)) << "\"";
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator << (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_right (-i, false);
        else arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_left (i, false);
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator >> (const bytes_array<word, size> &b, int32 i) {
        bytes_array<word, size> n (b);
        if (i < 0) arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_left (-i, false);
        else arithmetic::Words<endian::big, word> (slice<word> (n)).bit_shift_right (i, false);
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ~ (const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_negate<word> (n.end (), n.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator ^ (const bytes_array<word, size> &a, const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_xor<word> (n.end (), n.begin (), a.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator & (const bytes_array<word, size> &a, const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_and<word> (n.end (), n.begin (), a.begin (), b.begin ());
        return n;
    }

    template <std::integral word, size_t size>
    constexpr bytes_array<word, size> operator | (const bytes_array<word, size> &a, const bytes_array<word, size> &b) {
        bytes_array<word, size> n;
        arithmetic::bit_or<word> (n.end (), n.begin (), a.begin (), b.begin ());
        return n;
    }

    template <size_t size> byte_array<size> read_byte_array (const hex_string &x) {
        if (!x.valid () || ((x.size () % 2) != 0)) throw encoding::invalid {encoding::hex::Format, x};
        if ((x.size () / 2) != size) throw exception {} << "invalid hex string size";
        byte_array<size> T;
        encoding::hex::decode (x.end (), x.begin (), T.data ());
        return T;
    }

}

#endif
