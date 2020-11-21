#pragma once
#include <tuple>
#include <sstream>
#include <string>
#include <variant>
#include <repr/detail/has_c_str.h>
#include <repr/detail/has_data.h>
#include <repr/detail/is_complexish.h>
#include <repr/detail/is_initializer_listish.h>
#include <repr/detail/is_vectorish.h>
#include <repr/detail/is_stackish.h>
#include <repr/detail/is_queueish.h>
#include <repr/detail/is_mappish.h>
#include <repr/detail/is_formattable.h>
#include <repr/detail/is_specialization.h>
#include <repr/detail/is_pairish.h>
#include <repr/detail/is_optionalish.h>
#include <repr/detail/magic_enum/magic_enum.hpp>
#include <repr/detail/boost/pfr.hpp>

namespace repr_detail {

template<class Fn, class TupleType, size_t... I>
std::string tuple_to_string(Fn fn, const TupleType& tuple, std::index_sequence<I...>)
{
  std::stringstream os;
  os << "{";
  (..., (os << (I == 0? "" : ", ") << fn(std::get<I>(tuple))));
  os << "}";
  return os.str();
}

template<class Fn, class StructType, size_t... I>
std::string struct_to_string(Fn fn, const StructType& s, std::index_sequence<I...>)
{
  std::stringstream os;
  os << "{";
  (..., (os << (I == 0? "" : ", ") << fn(boost::pfr::get<I>(s))));
  os << "}";
  return os.str();
}

template <class T>
static inline std::string to_string(T&& c) {
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
  else if constexpr (is_complexish<decayed_type>::value) {
    return fmt::format("({} + {}i)", to_string(c.real()), to_string(c.imag()));
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
      result += to_string(k) + ": " + to_string(v);
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
    if constexpr (is_formattable_v<typename decayed_type::value_type>) {
      // value_type is printable
      return fmt::format("{}", c);
    } else {
      std::string result = "{";
      const auto size = c.size();
      std::size_t i{0};
      for (const auto& e: c) {
        result += to_string(e);
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
      result += to_string(e);
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
      result += to_string(q.top());
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
      result += to_string(q.front());
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
  else if constexpr (is_pairish<decayed_type>::value) {
    const std::string result = "{" + to_string(c.first) + ", " + to_string(c.second) + "}";
    return result;
  }
  // tuple type
  else if constexpr (is_specialization<decayed_type, std::tuple>::value) {
    const auto tuple_element_printer = [] (const auto& e) {
      return to_string(e);
    };
    return tuple_to_string(tuple_element_printer, c, std::make_index_sequence<std::tuple_size<decayed_type>::value>());
  }
  // optional type
  else if constexpr (is_optionalish<decayed_type>::value) {
    if (c.has_value()) {
      return to_string(c.value());
    } else {
      return "nullopt";
    }
  }
  // variant type
  else if constexpr (is_specialization<decayed_type, std::variant>::value) {
    std::string result{""};
    std::visit([&result](const auto& value) { 
      result += to_string(value);
    }, c);
    return result;
  }
  // formattable with libfmt
  else if constexpr (is_formattable_v<decayed_type>) {
    return fmt::format("{}", c);
  }
  // class 
  else if constexpr (std::is_class_v<decayed_type>) {
    const auto struct_element_printer = [] (const auto& e) {
      return to_string(e);
    };
    return struct_to_string(struct_element_printer, c, std::make_index_sequence<boost::pfr::tuple_size<decayed_type>::value>());
  }
  // not printable
  else {
    return "[object]";
  }
}

}