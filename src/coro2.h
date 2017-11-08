#pragma once
#include <experimental/coroutine>
#include <optional>
#include <utility>
#include <cassert>
#include <cstdio>

#if 1
#define print(x) puts(x)
#else
#define print(x)
#endif

using std::experimental::coroutine_handle;
using std::experimental::suspend_always;
using std::experimental::suspend_never;

// ================================================================================================

class task {
public:
  struct promise_type {
    task get_return_object() noexcept {
      return { *this };
    }

    constexpr suspend_never initial_suspend() noexcept {
      return {};
    }

    constexpr suspend_never final_suspend() noexcept {
      return {};
    }

    constexpr void return_void() noexcept {
    }

    void unhandled_exception() noexcept {
      std::abort();
    }
  };

  using handle_type = coroutine_handle<promise_type>;

  task(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
  }

private:
  handle_type handle_ = nullptr;
};

// ================================================================================================

template <typename T>
class async {
public:
  struct promise_type {
    async get_return_object() noexcept {
      return { *this };
    }

    constexpr suspend_never initial_suspend() noexcept {
      return {};
    }

    constexpr suspend_never final_suspend() noexcept {
      return {};
    }

    void return_value(T value) noexcept {
      value_ = std::move(value);
      if (consumer_) {
        consumer_.resume();
      }
    }

    void unhandled_exception() noexcept {
      std::abort();
    }

    std::optional<T> value_;
    coroutine_handle<> consumer_ = nullptr;
  };

  using handle_type = coroutine_handle<promise_type>;

  async(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
  }

  bool await_ready() noexcept {
    return !!handle_.promise().value_;
  }

  void await_suspend(coroutine_handle<> consumer) noexcept {
    if (await_ready()) {
      consumer.resume();
    } else {
      handle_.promise().consumer_ = consumer;
    }
  }

  auto& await_resume() noexcept {
    return *handle_.promise().value_;
  }

private:
  coroutine_handle<promise_type> handle_ = nullptr;
};

// ================================================================================================

template <typename T>
class generator {
public:
  struct promise_type {
    generator get_return_object() noexcept {
      return { *this };
    }

    constexpr auto initial_suspend() noexcept {
      return suspend_never{};
    }

    constexpr auto final_suspend() noexcept {
      return suspend_always{};
    }

    auto yield_value(T& value) noexcept {
      value_ = std::addressof(value);
      return suspend_always{};
    }

    auto return_void() noexcept {
      value_ = nullptr;
      return suspend_never{};
    }

    void unhandled_exception() noexcept {
      std::abort();
    }

    T* value_ = nullptr;
  };

  using handle_type = coroutine_handle<promise_type>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    iterator() noexcept = default;

    iterator(handle_type handle) noexcept : handle_(handle) {
    }

    ~iterator() {
      if (handle_) {
        handle_.destroy();
      }
    }

    iterator& operator++() noexcept {
      handle_.resume();
      if (handle_.done()) {
        std::exchange(handle_, nullptr).destroy();
      }
      return *this;
    }

    iterator operator++(int) = delete;

    bool operator==(const iterator& other) const noexcept {
      return handle_ == other.handle_;
    }

    bool operator!=(const iterator& other) const noexcept {
      return handle_ != other.handle_;
    }

    reference operator*() noexcept {
      return *handle_.promise().value_;
    }

    pointer operator->() noexcept {
      return handle_.promise().value_;
    }

    handle_type handle_ = nullptr;
  };

  generator(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
  }

  iterator begin() noexcept {
    return std::exchange(handle_, nullptr);
  }

  iterator end() noexcept {
    return {};
  }

private:
  handle_type handle_ = nullptr;
};

// ================================================================================================

template <typename T>
class async_generator {
public:
  struct promise_type {
    async_generator get_return_object() noexcept {
      return { *this };
    }

    constexpr auto initial_suspend() noexcept {
      return suspend_never{};
    }

    constexpr auto final_suspend() noexcept {
      return suspend_always{};
    }

    auto yield_value(T& value) noexcept {
      value_ = std::addressof(value);
      return suspend_always{};
    }

    auto return_void() noexcept {
      value_ = nullptr;
      return suspend_never{};
    }

    void unhandled_exception() noexcept {
      std::abort();
    }

    T* value_ = nullptr;
  };

  using handle_type = coroutine_handle<promise_type>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    iterator() noexcept = default;

    iterator(handle_type handle) noexcept : handle_(handle) {
    }

    ~iterator() {
      if (handle_) {
        handle_.destroy();
      }
    }

    iterator& operator++() noexcept {
      handle_.resume();
      if (handle_.done()) {
        std::exchange(handle_, nullptr).destroy();
      }
      return *this;
    }

    iterator operator++(int) = delete;

    bool operator==(const iterator& other) const noexcept {
      return handle_ == other.handle_;
    }

    bool operator!=(const iterator& other) const noexcept {
      return handle_ != other.handle_;
    }

    reference operator*() noexcept {
      return *handle_.promise().value_;
    }

    pointer operator->() noexcept {
      return handle_.promise().value_;
    }

    handle_type handle_ = nullptr;
  };

  async_generator(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
  }

  iterator begin() noexcept {
    return std::exchange(handle_, nullptr);
  }

  iterator end() noexcept {
    return {};
  }

private:
  handle_type handle_ = nullptr;
};

// ================================================================================================
