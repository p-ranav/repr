<p align="center">
  <img height="60" src="img/logo.png"/> 
</p>

This library provides a single function `repr(value) -> std::string` that returns a printable string representation of `value`.

* Header-only library
* Requires C++17
* MIT License

```cpp
// standard includes
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <optional>

// library includes
#include <repr/repr.h>

int main() {
  
  struct my_struct {
    std::vector<int> foo{1, 2, 3};
    std::map<std::string, int> bar{{"a", 4}, {"b", 5}};
    std::optional<float> baz{3.14};
    bool flag{false};
  };

  my_struct s;
  
  fmt::print("{}\n", repr(s)); // {{1, 2, 3}, {"a": 4, "b": 5}, 3.14, false}

}
```