#include "async.h"
#include <asio/io_service.hpp>
#include <system_error>
#include <iostream>

asio::io_service service;

class event {
public:
  using handle_type = std::experimental::coroutine_handle<>;
  event(int value) noexcept : value_(value) {
  }
  ~event() {
    handle_ = nullptr;
  }
  constexpr bool await_ready() noexcept {
    return false;
  }
  void await_suspend(handle_type handle) {
    handle_ = handle;
    service.post([this]() {
      value_++;
      if (handle_) {
        handle_.resume();
      }
    });
  }
  auto await_resume() noexcept {
    return value_;
  }
private:
  int value_ = 0;
  handle_type handle_ = nullptr;
};

async<int> async_test(int i) {
  i = co_await event(i);
  co_return i;
}

generator<int> generator_test(int i) {
  i++;
  co_yield i;
  i++;
  co_yield i;
}

async_generator<int> async_generator_test(int i) {
  i = co_await async_test(i);
  co_yield i;
  i = co_await async_test(i);
  co_yield i;
  i = co_await async_test(i);
  co_yield i;
}

task test() {
  auto i = 0;
  std::cout << "== async_test ==" << std::endl;
  i = co_await async_test(i);
  std::cout << i << std::endl;

  std::cout << "== generator_test ==" << std::endl;
  for (auto& e : generator_test(i)) {
    std::cout << e << std::endl;
    i = e;
  }

  std::cout << "== async_generator_test ==" << std::endl;
  for co_await(auto& e : async_generator_test(i)) {
    std::cout << e << std::endl;
    i = e;
    if (i > 4) {
      //break;  // this will leak the async_generator promise
      //co_return;  // this will leak the async_generator promise
    }
  }

  std::cout << "== done ==" << std::endl;
  co_return;
}

int main() {
  test();
  service.run();
}
