#pragma once
#include <iomanip>
#include <optional>
#include <tuple>
#include <string>
#include <variant>
#include <repr/detail/has_c_str.h>
#include <repr/detail/has_data.h>
#include <repr/detail/is_complex.h>
#include <repr/detail/is_initializer_listish.h>
#include <repr/detail/is_vectorish.h>
#include <repr/detail/is_stackish.h>
#include <repr/detail/is_queueish.h>
#include <repr/detail/is_mappish.h>
#include <repr/detail/is_printable.h>
#include <repr/detail/is_specialization.h>
#include <repr/detail/is_pair.h>
#include <repr/detail/magic_enum.hpp>
#define FMT_HEADER_ONLY
#include <repr/detail/fmt/format.h>
#include <repr/detail/fmt/ranges.h>

namespace repr_detail {

template<class Fn, class TupleType, size_t... I>
std::string print_tuple(Fn fn, const TupleType& tuple, std::index_sequence<I...>)
{
  std::stringstream os;
  os << "{";
  (..., (os << (I == 0? "" : ", ") << fn(std::get<I>(tuple))));
  os << "}";
  return os.str();
}

template <class T>
static inline std::string print(T&& c) {
  typedef typename std::decay<T>::type decayed_type;
  // boolean
  if constexpr (std::is_same_v<decayed_type, bool>) {
    if (c) {
      return "true";
    } else {
      return "false";
    }
  }
  // pointer
  else if constexpr (std::is_pointer<decayed_type>::value) {
    if (c == nullptr) {
      return "nullptr";
    } else {
      if constexpr (std::is_same_v<decayed_type, const char*>) {
        return fmt::format("\"{}\"", c);
      } else {
        std::stringstream os;
        os << c;
        return os.str();
      }
    }
  }
  // enum
  else if constexpr (std::is_enum<decayed_type>::value) {
    using namespace magic_enum::ostream_operators;
    std::stringstream os;
    os << c;
    return os.str();
  }
  // complex
  else if constexpr (is_complex<decayed_type>::value) {
    return fmt::format("({} + {}i)", print(c.real()), print(c.imag()));
  }
  // char
  else if constexpr (std::is_same_v<decayed_type, char>) {
    return fmt::format("'{}'", c);
  }
  // integral
  else if constexpr (std::is_integral<decayed_type>::value) {
    return fmt::format("{}", c);
  }
  // float, double, long double
  else if constexpr (std::is_floating_point<decayed_type>::value) {
    return fmt::format("{}", c);
  }
  else if constexpr (std::is_fundamental<decayed_type>::value) {
    return fmt::format("{}", c);
  }
  // std::string
  else if constexpr (has_cstr<decayed_type, const char *()>::value) {
    return fmt::format("\"{}\"", c);
  }
  // std::string_view
  else if constexpr (has_data<decayed_type, const char *()>::value) {
    return fmt::format("\"{}\"", c.data());
  }
  // std::map-like container
  else if constexpr (is_mappish<decayed_type>::value) {
    std::string result = "{";
    const auto size = c.size();
    std::size_t i{0};
    for (const auto& [k, v]: c) {
      result += print(k) + ": " + print(v);
      i += 1;
      if (i < size) {
        result += ", ";
      }
    }
    result += "}";
    return result;
  }
  // std::vector-like container type
  else if constexpr (is_vectorish<decayed_type>::value) {
    if constexpr (is_printable<typename decayed_type::value_type>::value) {
      // value_type is printable
      return fmt::format("{}", c);
    } else {
      std::string result = "{";
      const auto size = c.size();
      std::size_t i{0};
      for (const auto& e: c) {
        result += print(e);
        i += 1;
        if (i < size) {
          result += ", ";
        }
      }
      result += "}";
      return result;
    }
  }
  // std::initializer_list-like container type
  else if constexpr (is_initializer_listish<decayed_type>::value) {
    std::string result = "{";
    const auto size = c.size();
    std::size_t i{0};
    for (const auto& e: c) {
      result += print(e);
      i += 1;
      if (i < size) {
        result += ", ";
      }
    }
    result += "}";
    return result;
  }
  // stack-like container type
  else if constexpr (is_stackish<decayed_type>::value) {
    std::string result = "{";
    auto q = std::forward<T>(c);
    const auto size = q.size();
    std::size_t i{0};
    while (!q.empty()) {
      result += print(q.top());
      q.pop();
      i += 1;
      if (i < size) {
        result += ", ";
      }
    }
    result += "}";
    return result;
  }
  // queue-like container type
  else if constexpr (is_queueish<decayed_type>::value) {
    std::string result = "{";
    auto q = std::forward<T>(c);
    const auto size = q.size();
    std::size_t i{0};
    while (!q.empty()) {
      result += print(q.front());
      q.pop();
      i += 1;
      if (i < size) {
        result += ", ";
      }
    }
    result += "}";
    return result;
  }
  // pair type
  else if constexpr (is_pair<decayed_type>::value) {
    const std::string result = "{" + print(c.first) + ", " + print(c.second) + "}";
    return result;
  }
  // tuple type
  else if constexpr (is_specialization<decayed_type, std::tuple>::value) {
    const auto tuple_element_printer = [] (const auto& e) {
      return print(e);
    };
    return print_tuple(tuple_element_printer, c, std::make_index_sequence<std::tuple_size<decayed_type>::value>());
  }
  // optional type
  else if constexpr (is_specialization<decayed_type, std::optional>::value) {
    if (c.has_value()) {
      return print(c.value());
    } else {
      return "nullopt";
    }
  }
  // variant type
  else if constexpr (is_specialization<decayed_type, std::variant>::value) {
    std::string result{""};
    std::visit([&result](const auto& value) { 
      result += print(value);
    }, c);
    return result;
  }
  // printable object of type T
  else if constexpr (is_printable<decayed_type>::value) {
    return fmt::format("{}", c);
  }
  // not printable
  else {
    return "[object]";
  }
}

}