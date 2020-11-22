<p align="center">
  <img height="60" src="img/logo.png"/> 
</p>

## Highlights

* Header-only library
* Requires C++17
* MIT License

`repr()` returns a printable representation of a given object.

```cpp
// standard includes
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <optional>
using namespace std::literals::chrono_literals;

// library includes
#include <repr/repr.h>

// Some user-defined struct
struct my_struct {
  std::map<std::string, std::vector<int>> map{{"a", {1, 2}}, {"b", {3, 4}}};
  std::optional<float> baz{3.14};
  std::tuple<bool, char, std::string> tup = std::make_tuple(true, 'A', "Lisa Simpson");

  enum class log_level { info, warn, error };
  log_level level{log_level::info}; 

  struct nested_struct {
    std::chrono::nanoseconds start{5345ns};
    std::chrono::nanoseconds end{9876ns};
  };
  nested_struct ns;
};

int main() {
  my_struct s;

  std::cout << repr(s) << "\n";
  // {{"a": {1, 2}, "b": {3, 4}}, 3.14, {true, 'A', "Lisa Simpson"}, info, {5345ns, 9876ns}}
}
```

## Getting Started

`repr` is a header-only library. Add `include/` to your `target_include_directories` and include `<repr/repr.h>`. 

`repr(value)` returns a printable string representation of value. To achieve this, `repr` uses [libfmt](https://github.com/fmtlib/fmt), [magic_enum](https://github.com/Neargye/magic_enum) and [boost::pfr](https://github.com/boostorg/pfr) and provides a nice convenience function.

`repr()` supports fundamental types, strings, containers, container adapters, aggregate-initializable classes/structs and chrono.

```cpp
#include <repr/repr.h>
#include <vector>
#include <iostream>

int main() {
  std::vector<std::vector<int>> mat{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  std::cout << repr(mat) << "\n";
  // {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}

  std::map<std::string, std::vector<int>> map{{"a", {1, 2, 3}}, {"b", {4, 5, 6}}, {"c", {7, 8, 9}}};
  std::cout << repr(map) << "\n";
  // {"a": {1, 2, 3}, "b": {4, 5, 6}, "c": {7, 8, 9}}

  std::optional<int> opt;
  std::cout << repr(opt) << "\n";
  // nullopt

  enum class Color { RED = 2, BLUE = 4, GREEN = 8 };
  Color color = Color::BLUE;
  std::cout << repr(color) << "\n";
  // BLUE
}
```

## Building Samples

```bash
git clone https://github.com/p-ranav/repr
cd repr
mkdir build && cd build
cmake .. && make && make install
```

## Contributing
Contributions are welcome, have a look at the [CONTRIBUTING.md](CONTRIBUTING.md) document for more information.

## License
The project is available under the [MIT](https://opensource.org/licenses/MIT) license.

