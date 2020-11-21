<p align="center">
  <img height="60" src="img/logo.png"/> 
</p>

## Highlights

* Header-only library
* Requires C++17
* MIT License

`repr()` returns a printable representation of the given object.

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
  
  std::string r = repr(s); // {{1, 2, 3}, {"a": 4, "b": 5}, 3.14, false}
}
```
