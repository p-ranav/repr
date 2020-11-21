#pragma once
#include <type_traits>

namespace repr_detail {

template <typename T, typename _ = void> struct is_vectorish : std::false_type {};

template <typename... Ts> struct is_vectorish_helper {};

template <typename T>
struct is_vectorish<
    T, std::conditional_t<
           false,
           is_vectorish_helper<
               typename T::value_type, typename T::size_type, typename T::iterator,
               typename T::const_iterator, decltype(std::declval<T>().size()),
               decltype(std::declval<T>().begin()), decltype(std::declval<T>().end()),
               decltype(std::declval<T>().cbegin()), decltype(std::declval<T>().cend())>,
           void>> : public std::true_type {};

} // namespace repr_detail