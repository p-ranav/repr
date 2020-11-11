#pragma once
#include <iostream>
#include <type_traits>

namespace repr_detail {

template<typename _Tp, typename dummy = void>
struct is_printable : std::false_type {};

template<typename _Tp>
struct is_printable<_Tp, 
        typename std::enable_if_t<
            std::is_same_v<decltype(std::cout << std::declval<_Tp>()), std::ostream&>
        >
    > : std::true_type {};

}