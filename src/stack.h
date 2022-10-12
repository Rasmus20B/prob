#include <array>
#include <optional> 
#include <concepts>
#include <iterator>

namespace prob {


template<typename T>
class Stack {
  public:
  [[nodiscard]]bool empty() { return (sp == c.begin());}
  [[nodiscard]]bool full() { return (sp == c.end());}
  void push(T val) {
    [[unlikely]]
    if(full()) 
      return;
    *sp = val;
    sp++;
  }
  std::optional<T> pop() {
    [[unlikely]]
    if(empty())
      return {};
    sp--;
    return *sp;
  }

  std::optional<T> peek() {
    if(empty()) 
      return {};

    return *sp;
  }

  
  std::array<T, 256> c{};
  typename std::array<T, 256>::iterator sp = c.begin();
};

}
