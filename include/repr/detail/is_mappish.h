#pragma once
#include <type_traits>

namespace repr_detail {

// Needed for some older versions of GCC
template<typename...>
  struct voider { using type = void; };

// std::void_t will be part of C++17, but until then define it ourselves:
template<typename... T>
  using void_t = typename voider<T...>::type;

template<typename T, typename U = void>
  struct is_mappish_impl : std::false_type { };

template<typename T>
  struct is_mappish_impl<T, void_t<typename T::key_type,
                                    typename T::mapped_type,
                                    decltype(std::declval<T&>()[std::declval<const typename T::key_type&>()])>>
  : std::true_type { };

template<typename T>
struct is_mappish : is_mappish_impl<T>::type { };

}