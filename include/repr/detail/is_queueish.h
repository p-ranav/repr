#pragma once
#include <type_traits>

template<typename T, typename _ = void>
struct is_queueish : std::false_type {};

template<typename... Ts>
struct is_queueish_helper {};

template<typename T>
struct is_queueish<
        T,
        std::conditional_t<
            false,
            is_queueish_helper<
                typename T::value_type,
                typename T::size_type,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().pop()),
                decltype(std::declval<T>().front())
                >,
            void
            >
        > : public std::true_type {};