#pragma once
#include <repr/detail/print.h>
#include <sstream>
#include <iostream>

template <class T>
auto repr(T&& value) {
  return repr_detail::print(std::forward<T>(value));
}