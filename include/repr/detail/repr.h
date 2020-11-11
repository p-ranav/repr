#pragma once
#include <repr/detail/print.h>
#include <sstream>

template <class T>
auto repr(T&& value) {
  std::stringstream os;
  print(std::forward<T>(value), os, "");
  return os.str();
}