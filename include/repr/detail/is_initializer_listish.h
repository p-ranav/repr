#pragma once
#include <type_traits>

namespace repr_detail {

template<typename T, typename _ = void>
struct is_initializer_listish : std::false_type {};

template<typename... Ts>
struct is_initializer_listish_helper {};

template<typename T>
struct is_initializer_listish<
        T,
        std::conditional_t<
            false,
            is_initializer_listish_helper<
                typename T::value_type,
                typename T::size_type,
                typename T::iterator,
                typename T::const_iterator,
                decltype(std::declval<T>().size()),
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end())
                >,
            void
            >
        > : public std::true_type {};

}