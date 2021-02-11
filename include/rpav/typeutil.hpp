#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace rpav {

/// Trivial type wrapper, for "passing" types as "values".
/// Useful as tags for constructors.
template<typename T>
struct type_val {
    using type = T;
};

template<typename T>
constexpr auto type = type_val<T>{};

/**
 * Type A if type E is not std::false_type, else type B
 * @tparam A
 * @tparam B
 */
template<typename E, typename A, typename B>
struct if_else {
    using type = A;
};

template<typename A, typename B>
struct if_else<std::false_type, A, B> {
    using type = B;
};

template<typename Expr, typename Then, typename Else>
using if_else_t = typename if_else<Expr, Then, Else>::type;

/**
 * Type B unless type A is not void or false_type
 * @tparam A
 * @tparam B
 */
template<typename A, typename B>
struct or_type {
    using type = A;
};

template<typename B>
struct or_type<void, B> {
    using type = B;
};

template<typename B>
struct or_type<std::false_type, B> {
    using type = B;
};

template<typename A, typename B>
using or_type_t = typename or_type<A, B>::type;

/**
 * std::true_type unless T is std::false_type
 */
template<typename T>
struct is_true {
    using type = if_else_t<typename std::is_base_of<std::false_type, T>::type, std::false_type, std::true_type>;
};

template<typename T>
using is_true_t = typename is_true<T>::type;

/**
 * logical not
 */
template<typename T>
struct not_type {
    using type = std::false_type;
};

template<>
struct not_type<std::false_type> {
    using type = std::true_type;
};

template<typename T>
using not_type_t = typename not_type<T>::type;

/**
 * sfinae not
 */
template<typename T>
struct sfinae_not {};

template<>
struct sfinae_not<std::false_type> {
    using type = std::true_type;
};

template<typename T>
using not_t = typename sfinae_not<T>::type;

/**
 * std::true_type if A and B are not false_type
 */

// Implemented this way to catch derived-from-false_type
template<typename A, typename B>
struct and_type {
    using type = if_else_t<is_true_t<A>, if_else_t<is_true_t<B>, std::true_type, std::false_type>, std::false_type>;
};

template<typename A, typename B>
using and_type_t = typename and_type<A, B>::type;

/**
 * Type B if A is not void
 * @tparam A
 * @tparam B
 */
template<typename A, typename B = A>
struct not_void {
    using type = B;
};

template<typename B>
struct not_void<void, B> {};

template<typename A, typename B = A>
using not_void_t = typename not_void<A, B>::type;

/**
 * `const B` if `const A`, otherwise `B`
 * @tparam A
 * @tparam B
 */
template<typename A, typename B>
struct maybe_const {
    using type = B;
};

template<typename A, typename B>
struct maybe_const<const A, B> {
    using type = const B;
};

/**
 * Either a reference or a local instance.  Useful for generic assignment
 * and handling lvalue ref or rvalue.
 *
 * @tparam T
 */
template<typename T>
struct local_inst {
    using type = T&;
};

template<typename T>
struct local_inst<T&&> {
    using type = T;
};

/**
 * Either a reference or a pointer.
 * @tparam T
 */
template<typename T>
struct ref_or_ptr {
    using type = T&;
};

template<typename T>
struct ref_or_ptr<T*> {
    using type = T*;
};

/**
 * Find the const-qualified iterator
 * @tparam T
 */
template<typename T>
struct iterator_extractor {
    using type = typename std::decay<T>::type::iterator;
};

template<typename T>
struct iterator_extractor<T const> {
    using type = typename std::decay<T>::type::const_iterator;
};

template<typename T, size_t SZ>
struct iterator_extractor<T[SZ]> {
    using type = T*;
};

template<typename T, size_t SZ>
struct iterator_extractor<T const (&)[SZ]> {
    using type = const T*;
};

/**
 * Find the const-qualified reverse_iterator
 * @tparam T
 */
template<typename T>
struct reverse_iterator_extractor {
    using type = typename std::decay<T>::type::reverse_iterator;
};

template<typename T>
struct reverse_iterator_extractor<T const> {
    using type = typename std::decay<T>::type::const_reverse_iterator;
};

/**
 * Use to create "perfectly forwarded" tuple
 */
template<typename T>
struct maybe_ref_helper {
    using type = T;
};

template<typename T>
struct maybe_ref_helper<T&> {
    using type = typename std::reference_wrapper<T>;
};

template<typename T>
auto maybe_ref(T&& v)
{
    return typename maybe_ref_helper<T>::type(std::forward<T>(v));
}

/**
 * Make a std::function from an "invocable"-type.  `std::bind` returns
 * are not supported, use `rpav::mbind` instead .. it sucks less anyway.
 */

template<typename T>
struct extract_function {
    using type      = void;
    using signature = void;
};

template<typename R, typename... A>
struct extract_function<R (*)(A...)> {
    using type      = std::function<R(A...)>;
    using signature = R(A...);
};

template<typename R, typename C, typename... A>
struct extract_function<R (C::*)(A...)> {
    using type      = std::function<R(A...)>;
    using signature = R(A...);
};

template<typename R, typename C, typename... A>
struct extract_function<R (C::*)(A...) const> {
    using type      = std::function<R(A...)>;
    using signature = R(A...);
};

template<typename R, typename... A>
struct extract_function<std::function<R(A...)>> {
    using type      = std::function<R(A...)>;
    using signature = R(A...);
};

template<typename F>
inline auto make_function(const F& f) -> typename extract_function<decltype(&F::operator())>::type
{
    return f;
}

template<typename F, std::enable_if_t<!std::is_class<F>::value, bool> = 0>
inline auto make_function(F f) -> decltype(f, typename extract_function<decltype(f)>::type())
{
    return f;
}

/**
  Take the constness of one type and apply it to another
 */

template<typename From, typename To>
struct const_carry {
    using type                     = To;
    static constexpr bool is_const = false;
};

template<typename From, typename To>
struct const_carry<const From, To> {
    using type                     = const To;
    static constexpr bool is_const = true;
};

template<typename From, typename To>
using const_carry_t = typename const_carry<From, To>::type;

/**
   Find a type in a list of types, or return invalid
*/
template<size_t Invalid, typename... Ts>
struct type_index {
public:
    static constexpr auto invalid = Invalid;

private:
    template<typename M, size_t N, typename S, typename... Ss>
    static constexpr size_t index_helper()
    {
        if constexpr(std::is_same_v<S, M>) return N;
        return index_helper<M, N + 1, Ss...>();
    }

    template<typename M, size_t N>
    static constexpr size_t index_helper()
    {
        return invalid;
    }

public:
    template<typename S>
    static constexpr size_t index()
    {
        return index_helper<S, 0, Ts...>();
    }
};

/**
   Get the Nth T of a parameter pack
*/
template<size_t N, typename... Ts>
struct get_nth {
    template<size_t I, typename S, typename... Ss>
    struct find_nth {
        using type = typename find_nth<I - 1, Ss...>::type;
    };

    template<typename S, typename... Ss>
    struct find_nth<0, S, Ss...> {
        using type = S;
    };

    using type = typename find_nth<N, Ts...>::type;
};

template<size_t N, typename... Ts>
using get_nth_t = typename get_nth<N, Ts...>::type;

/**
   "Safe" unsigned promotion
*/
template<typename T>
struct promote_unsigned {
    template<typename S, typename = std::enable_if_t<sizeof(S) < sizeof(unsigned)>>
    constexpr static unsigned filter()
    {
        return {};
    }

    template<typename S, typename = std::enable_if_t<sizeof(S) >= sizeof(unsigned)>>
    constexpr static std::make_unsigned_t<S> filter()
    {
        return {};
    }

    using type = decltype(filter<T>());
};

// More utilities

template<typename... Ts>
struct parameter_pack {};

/* We _could_ do this with just a template-using, but then
   we apparently can't apply it below
*/
template<typename T, typename... Ts>
struct first_ {
    using type = T;
};

template<typename... Ts>
using first_t = typename first_<Ts...>::type;

template<typename T, typename... Ts>
struct rest_ {
    using type = parameter_pack<Ts...>;
};

template<typename... Ts>
using rest_t = typename rest_<Ts...>::type;

template<template<typename...> typename C, typename T>
struct apply_pack {
private:
    template<template<typename...> typename D, typename... Ts>
    constexpr static C<Ts...> filter(const D<Ts...>*);

public:
    using type = decltype(apply_pack::filter(static_cast<const T*>(nullptr)));
};

template<template<typename...> typename C, typename T>
using apply_pack_t = typename apply_pack<C, T>::type;

template<typename T>
using parameter_pack_of = typename apply_pack<parameter_pack, T>::type;

template<typename P>
using pack_first_t = typename apply_pack<first_t, P>::type;

template<typename P>
using pack_rest_t = typename apply_pack<rest_t, P>::type;

// Math

/*
  "But std has this!" you say.  Yes, that'd be nice if one could *specialize*
  it.  But, you can't.  So any value types that ought to be signed, will never pass.
  Fuck C++.
*/

template<typename T>
struct is_signed {
    static constexpr bool value = std::is_signed_v<T>;
};

template<typename T>
constexpr bool is_signed_v = is_signed<T>::value;

} // namespace rpav
