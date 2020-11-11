#pragma once
#include <complex>
#include <type_traits>

template <class T> struct is_complex : public std::false_type {};
template <class T> struct is_complex<const T> : public is_complex<T> {};
template <class T>
struct is_complex<volatile const T> : public is_complex<T> {};
template <class T> struct is_complex<volatile T> : public is_complex<T> {};
template <class T> struct is_complex<std::complex<T>> : public std::true_type {};