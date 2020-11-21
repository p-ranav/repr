#pragma once
#include <repr/detail/to_string.h>

template <class T> auto repr(T &&value) {
  return repr_detail::to_string(std::forward<T>(value));
}