#include <repr/repr.h>
#include <array>
#include <complex>
#include <map>
#include <string>
#include <string_view>
#include <set>
#include <vector>
#include <list>
#include <queue>
#include <tuple>
#include <optional>
#include <chrono>
#include <iostream>

int main() {
  {
    std::cout << repr("Hello World!") << "\n";
  }

  {
    int * ptr = nullptr;
    std::cout << repr(ptr) << "\n";
  }

  {
    int * ptr = new int(5);
    std::cout << repr(ptr) << "\n";
    delete ptr;
  }

  {
    std::vector<int> vec{1, 2, 3};
    std::cout << repr(vec) << "\n";
  }

  {
    std::complex<double> c{1, 4};
    std::cout << repr(c) << "\n";
  }

  {
    std::complex<double> c{1.5323, 4.12415};
    std::cout << repr(c) << "\n";
  }

  {
    enum class Color { RED = 2, BLUE = 4, GREEN = 8 };
    Color color = Color::BLUE;
    std::cout << repr(color) << "\n";
  }

  {
    float f{3.14f};
    std::cout << repr(f) << "\n";
  }

  {
    double f{2.1753284};
    std::cout << repr(f) << "\n";
  }

  {
    long double f{241.4124186412};
    std::cout << repr(f) << "\n";
  }

  {
    std::cout << repr('a') << "\n";
  }

  {
    std::string str{"Hello"};
    std::cout << repr(str) << "\n";
  }

  {
    std::string_view str{"World"};
    std::cout << repr(str) << "\n";
  }

  {
    std::map<std::string,int> map{{"a", 1}, {"b", 2}, {"c", 3}};
    std::cout << repr(map) << "\n";
  }

  {
    std::vector<std::string> vec{"a", "b", "c"};
    std::cout << repr(vec) << "\n";
  }

  {
    std::vector<char> vec{'a', 'b', 'c'};
    std::cout << repr(vec) << "\n";
  }

  {
    std::map<std::string, std::vector<int>> map{{"a", {1, 2, 3}}, {"b", {4, 5, 6}}, {"c", {7, 8, 9}}};
    std::cout << repr(map) << "\n";
  }

  {
    std::cout << repr(5) << "\n";
  }

  {
    std::initializer_list<double> foo = {1.0, 2.0, 3.0};
    std::cout << repr(foo) << "\n";
  }

  {
    std::vector<std::vector<int>> vec{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::cout << repr(vec) << "\n";
  }

  {
    std::array<int, 3> arr{1, 2, 3};
    std::cout << repr(arr) << "\n";
  }

  {
    typedef std::array<std::array<int, 3>, 3> Mat3x3;
    Mat3x3 matrix;
    matrix[0] = {1, 2, 3};
    matrix[1] = {4, 5, 6};
    matrix[2] = {7, 8, 9};
    std::cout << repr(matrix) << "\n";
  }

  {
    std::priority_queue<int> queue;
    for(int n : {1,8,5,6,3,4,0,9,7,2}) queue.push(n);
    std::cout << repr(queue) << "\n";
  }

  {
    std::queue<int> queue;
    for(int n : {1,8,5,6,3,4,0,9,7,2}) queue.push(n);
    std::cout << repr(queue) << "\n";
  }

  {
    std::set<int> set{1, 2, 3};
    std::cout << repr(set) << "\n";
  }

  {
    std::list<double> list{1., 2., 3.};
    std::cout << repr(list) << "\n";
  }

  {
    std::map<std::string, std::set<int>> foo{ 
      {"foo", {1, 2, 3, 3, 2, 1}}, {"bar", {7, 6, 5, 4}}};
    std::cout << repr(foo) << "\n";
  }

  {
    std::pair<int, char> p{5, 'c'};
    std::cout << repr(p) << "\n";
  }

  {
    std::tuple<int, double, std::string> tup{1, 3.14, "Hello"};
    std::cout << repr(tup) << "\n";
  }

  {
    auto get_student = [](int id) {
      if (id == 0) return std::make_tuple(3.8, 'A', "Lisa Simpson");
      if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
      if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
      throw std::invalid_argument("id");
    };
    auto arr = {get_student(0), get_student(1), get_student(2)};
    std::cout << repr(arr) << "\n";
  }

  {
    std::cout << repr(true) << "\n";
    std::cout << repr(false) << "\n";

    std::array<bool, 3> arr{true, false, true};
    std::cout << repr(arr) << "\n";
  }

  {
    std::optional<int> a = 5;
    std::cout << repr(a) << "\n";

    std::optional<int> b;
    std::cout << repr(b) << "\n";
  }

  {
    std::variant<int, float, char> v = 5;
    std::cout << repr(v) << "\n";

    v = 0.543643f;
    std::cout << repr(v) << "\n";

    v = 'y';
    std::cout << repr(v) << "\n";
  }

  {
    std::vector<std::variant<bool, int, int *, float, std::string, std::vector<int>,		      
          std::map<std::string, std::map<std::string, int>>, 
          std::pair<double, double>>> var;
    var.push_back(5);
    var.push_back(nullptr);
    var.push_back(3.14f);
    var.push_back(std::string{"Hello World"});
    var.push_back(std::vector<int>{1, 2, 3, 4});
    var.push_back(std::map<std::string, std::map<std::string, int>>{{"a",{{"b",1}}}, {"c",{{"d",2}, {"e",3}}}});
    var.push_back(true);
    var.push_back(std::pair<double, double>{1.1, 2.2});

    std::cout << repr(var) << "\n";
  }

  {
    using namespace std::literals::chrono_literals;
    auto ns = 5345ns;
    std::cout << repr(ns) << "\n"; // 5345ns

    auto us = 123us;
    std::cout << repr(us) << "\n"; // 123us

    auto ms = 15ms;
    std::cout << repr(ms) << "\n"; // 15ms

    auto s = 412s;
    std::cout << repr(s) << "\n"; // 412s

    auto m = 60min;
    std::cout << repr(60min) << "\n"; // 60m

    auto dur = std::chrono::duration_cast<std::chrono::microseconds>(15353000ns);
    std::cout << repr(dur) << "\n";
  }

  {
    struct my_struct {
      std::vector<int> foo{1, 2, 3};
      std::map<std::string, int> bar{{"a", 4}, {"b", 5}};
      std::optional<float> baz{3.14};
      bool flag{false};
    };

    my_struct s;
    std::cout << repr(s) << "\n";
  }
}