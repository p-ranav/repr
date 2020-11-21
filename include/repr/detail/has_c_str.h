#pragma once
#include <type_traits>

namespace repr_detail {

template <typename, typename T> struct has_cstr : std::false_type {};

template <typename C, typename Ret, typename... Args> struct has_cstr<C, Ret(Args...)> {
private:
  template <typename T>
  static constexpr auto check(T *) ->
      typename std::is_same<decltype(std::declval<T>().c_str(std::declval<Args>()...)), Ret>::type;

  template <typename> static constexpr std::false_type check(...);

  typedef decltype(check<C>(0)) type;

public:
  static constexpr bool value = type::value;
};

} // namespace repr_detail