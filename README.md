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
#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <optional>
using namespace std::literals::chrono_literals;

// library includes
#include <repr/repr.h>

int main() {
  
  struct my_struct {
    std::vector<int> foo{1, 2};
    std::map<std::string, int> bar{{"a", 4}, {"b", 5}};
    std::optional<float> baz{3.14};
    bool flag{false};
    std::tuple<int, char, std::string> tup = std::make_tuple(3.8, 'A', "Lisa Simpson");

    enum class log_level { info, warn, error };
    log_level level{log_level::info}; 

    struct nested_struct {
      std::chrono::nanoseconds start{5345ns};
      std::chrono::nanoseconds end{9876ns};
    };
    nested_struct ns;
  };

  my_struct s;
  
  std::cout << repr(s) << "\n";
  // {{1, 2}, {"a": 4, "b": 5}, 3.14, false, {3, 'A', "Lisa Simpson"}, info, {5345ns, 9876ns}}
}
```
