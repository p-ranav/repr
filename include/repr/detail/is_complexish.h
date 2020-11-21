#pragma once
#include <type_traits>

namespace repr_detail {

template <typename T, typename _ = void> struct is_complexish : std::false_type {};

template <typename... Ts> struct is_complexish_helper {};

template <typename T>
struct is_complexish<T, std::conditional_t<false,
                                           is_complexish_helper<typename T::value_type,
                                                                decltype(std::declval<T>().real()),
                                                                decltype(std::declval<T>().imag())>,
                                           void>> : public std::true_type {};

} // namespace repr_detail