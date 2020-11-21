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
  std::tuple<int, char, std::string> tup = std::make_tuple(3.8, 'A', "Lisa Simpson");

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
  // {{"a": {1, 2}, "b": {3, 4}}, 3.14, {3, 'A', "Lisa Simpson"}, info, {5345ns, 9876ns}}
}
```
