#pragma once
#include <type_traits>

namespace repr_detail {

template <typename T, typename _ = void> struct is_optionalish : std::false_type {};

template <typename... Ts> struct is_optionalish_helper {};

template <typename T>
struct is_optionalish<
    T, std::conditional_t<
           false,
           is_optionalish_helper<typename T::value_type, decltype(std::declval<T>().value()),
                                 decltype(std::declval<T>().has_value())>,
           void>> : public std::true_type {};

} // namespace repr_detail