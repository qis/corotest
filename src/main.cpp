#include "coro.h"
#include <boost/asio/io_service.hpp>
#include <system_error>
#include <iostream>

boost::asio::io_service service;

class event {
public:
  using handle_type = std::experimental::coroutine_handle<>;

  event(int value) : value_(value) {
  }

  ~event() = default;

  bool await_ready() {
    return false;
  }

  void await_suspend(handle_type handle) {
    handle_ = handle;
    service.post([this]() {
      value_++;
      handle_.resume();
    });
  }

  auto await_resume() {
    return value_;
  }

private:
  int value_ = 0;
  handle_type handle_ = nullptr;
};

async_generator<int> async_generator_func() {
  for (int i = 0; i < 3; i++) {
    auto value = co_await event(i);
    co_yield value;
  }
}

task task_func() {
  for co_await(auto& e : async_generator_func()) {
    std::cout << e << std::endl;
  }
}

int main() {
  task_func();
  service.run();
}
