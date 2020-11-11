#pragma once
#include <type_traits>

namespace repr_detail {

template<typename T, typename _ = void>
struct is_pair : std::false_type {};

template<typename... Ts>
struct is_pair_helper {};

template<typename T>
struct is_pair<
        T,
        std::conditional_t<
            false,
            is_pair_helper<
                typename T::first_type,
                typename T::second_type,
                decltype(std::declval<T>().first),
                decltype(std::declval<T>().second)
                >,
            void
            >
        > : public std::true_type {};

}