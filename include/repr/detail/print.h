#pragma once
#include <iomanip>
#include <optional>
#include <tuple>
#include <variant>
#include <repr/detail/has_c_str.h>
#include <repr/detail/has_data.h>
#include <repr/detail/is_complex.h>
#include <repr/detail/is_vectorish.h>
#include <repr/detail/is_stackish.h>
#include <repr/detail/is_queueish.h>
#include <repr/detail/is_mappish.h>
#include <repr/detail/is_printable.h>
#include <repr/detail/is_specialization.h>
#include <repr/detail/is_pair.h>
#include <repr/detail/magic_enum.hpp>

template<class Fn, class TupleType, size_t... I>
void print_tuple(Fn fn, std::ostream& os, const char * end, const TupleType& tuple, std::index_sequence<I...>)
{
  os << "{";
  (..., (os << (I == 0? "" : ", ") << fn(std::get<I>(tuple))));
  os << "}" << end;
}

template <class T>
auto print(T&& c, std::ostream& os, const char * end = "\n") {
  typedef typename std::decay<T>::type decayed_type;
  // boolean
  if constexpr (std::is_same_v<decayed_type, bool>) {
    if (c) {
      os << "true" << end;
    } else {
      os << "false" << end;
    }
  }
  // pointer
  else if constexpr (std::is_pointer<decayed_type>::value) {
    if (c == nullptr) {
      os << "nullptr" << end;
    } else {
      if constexpr (std::is_same_v<decayed_type, const char*>) {
        os << '"' << c << '"' << end;
      } else {
        os << c << end;
      }
    }
  }
  // enum
  else if constexpr (std::is_enum<decayed_type>::value) {
    using namespace magic_enum::ostream_operators;
    os << c << end;
  }
  // complex
  else if constexpr (is_complex<decayed_type>::value) {
    os  << "(" << c.real() << " + " << c.imag() << "i)" << end;
  }
  // float, double, long double
  else if constexpr (std::is_floating_point<decayed_type>::value) {
    if (std::is_same_v<decayed_type, float>) {
      os << c << 'f' << end;
    } else {
      os << c << end;
    }
  }
  // char
  else if constexpr (std::is_same_v<decayed_type, char>) {
    os << "'" << c << "'" << end;
  }
  else if constexpr (std::is_fundamental<decayed_type>::value) {
    os << c << end;
  }
  // std::string
  else if constexpr (has_cstr<decayed_type, const char *()>::value) {
    os << '"' << c.c_str() << "\"" << end;
  }
  // std::string_view
  else if constexpr (has_data<decayed_type, const char *()>::value) {
    os << '"' << c.data() << "\"" << end;
  }
  // std::map-like container
  else if constexpr (is_mappish<decayed_type>::value) {
    os << "{";
    const auto size = c.size();
    std::size_t i{0};
    for (const auto& [k, v]: c) {
      print(k, os, "");
      os << ": ";
      i += 1;
      if (i < size) 
        print(v, os, ", ");
      else
        print(v, os, "");
    }
    os << "}" << end;
  }
  // std::vector-like container type
  else if constexpr (is_vectorish<decayed_type>::value) {
    os << "{";
    const auto size = c.size();
    std::size_t i{0};
    for (const auto& e: c) {
      print(e, os, "");
      i += 1;
      if (i < size) {
        os << ", ";
      }
    }
    os << "}" << end;
  }
  // stack-like container type
  else if constexpr (is_stackish<decayed_type>::value) {
    os << "{";
    auto q = std::forward<T>(c);
    const auto size = q.size();
    std::size_t i{0};
    while (!q.empty()) {
      print(q.top(), os, "");
      q.pop();
      i += 1;
      if (i < size) {
        os << ", ";
      }
    }
    os << "}" << end;
  }
  // queue-like container type
  else if constexpr (is_queueish<decayed_type>::value) {
    os << "{";
    auto q = std::forward<T>(c);
    const auto size = q.size();
    std::size_t i{0};
    while (!q.empty()) {
      print(q.front(), os, "");
      q.pop();
      i += 1;
      if (i < size) {
        os << ", ";
      }
    }
    os << "}" << end;
  }
  // pair type
  else if constexpr (is_pair<decayed_type>::value) {
    os << "{";
    print(c.first, os, "");
    os << ", ";
    print(c.second, os, "");
    os << "}" << end;
  }
  // tuple type
  else if constexpr (is_specialization<decayed_type, std::tuple>::value) {
    const auto tuple_element_printer = [] (const auto& e) {
      std::stringstream os;
      print(e, os, "");
      return os.str();
    };
    print_tuple(tuple_element_printer, os, "", c, std::make_index_sequence<std::tuple_size<decayed_type>::value>());
  }
  // optional type
  else if constexpr (is_specialization<decayed_type, std::optional>::value) {
    if (c.has_value()) {
      print(c.value(), os, "");
    } else {
      os << "nullopt" << end;
    }
  }
  // variant type
  else if constexpr (is_specialization<decayed_type, std::variant>::value) {
    std::visit([&os](const auto& value) { 
      print(value, os, "");
    }, c);
  }
  else if constexpr (is_printable<decayed_type>::value) {
    os << c << end;
  }
  else {
    os << "<Object>" << end;
  }
}