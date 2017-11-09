#pragma once
#include <experimental/coroutine>
#include <optional>
#include <utility>
#include <cassert>
#include <cstdio>

#define COROTEST_DEBUG_TASK 1
#define COROTEST_DEBUG_ASYNC 1
#define COROTEST_DEBUG_GENERATOR 1
#define COROTEST_DEBUG_ASYNC_GENERATOR 1

// ================================================================================================

#ifdef _MSC_VER
#define COROTEST_PRINTFUNC puts(__FUNCSIG__);
#else
#define COROTEST_PRINTFUNC puts(__PRETTY_FUNCTION__);
#endif

#if COROTEST_DEBUG_TASK
#define COROTEST_TASK_CONSTEXPR
#define COROTEST_TASK_PRINTFUNC COROTEST_PRINTFUNC
#else
#define COROTEST_TASK_CONSTEXPR constexpr
#define COROTEST_TASK_PRINTFUNC
#endif

#if COROTEST_DEBUG_ASYNC
#define COROTEST_ASYNC_CONSTEXPR
#define COROTEST_ASYNC_PRINTFUNC COROTEST_PRINTFUNC
#else
#define COROTEST_ASYNC_CONSTEXPR constexpr
#define COROTEST_ASYNC_PRINTFUNC
#endif

#if COROTEST_DEBUG_GENERATOR
#define COROTEST_GENERATOR_CONSTEXPR
#define COROTEST_GENERATOR_PRINTFUNC COROTEST_PRINTFUNC
#else
#define COROTEST_GENERATOR_CONSTEXPR constexpr
#define COROTEST_GENERATOR_PRINTFUNC
#endif

#if COROTEST_DEBUG_ASYNC_GENERATOR
#define COROTEST_ASYNC_GENERATOR_CONSTEXPR
#define COROTEST_ASYNC_GENERATOR_PRINTFUNC COROTEST_PRINTFUNC
#else
#define COROTEST_ASYNC_GENERATOR_CONSTEXPR constexpr
#define COROTEST_ASYNC_GENERATOR_PRINTFUNC
#endif

// ================================================================================================

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

    COROTEST_TASK_CONSTEXPR auto initial_suspend() noexcept {
      COROTEST_TASK_PRINTFUNC
      return suspend_never{};
    }

    COROTEST_TASK_CONSTEXPR auto final_suspend() noexcept {
      COROTEST_TASK_PRINTFUNC
      return suspend_never{};
    }

    COROTEST_TASK_CONSTEXPR void return_void() noexcept {
      COROTEST_TASK_PRINTFUNC
    }

    void unhandled_exception() noexcept {
      COROTEST_TASK_PRINTFUNC
      std::abort();
    }

#if COROTEST_DEBUG_TASK
    promise_type() noexcept {
      COROTEST_TASK_PRINTFUNC
    }

    ~promise_type() {
      COROTEST_TASK_PRINTFUNC
    }
#endif
  };

  using handle_type = coroutine_handle<promise_type>;

  task(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
    COROTEST_TASK_PRINTFUNC
  }

#if COROTEST_DEBUG_TASK
  ~task() {
    COROTEST_TASK_PRINTFUNC
  }
#endif

private:
  handle_type handle_ = nullptr;
};

// ================================================================================================

template <typename T>
class async {
public:
  struct promise_type {
    async get_return_object() noexcept {
      COROTEST_ASYNC_PRINTFUNC
      return { *this };
    }

    COROTEST_ASYNC_CONSTEXPR auto initial_suspend() noexcept {
      COROTEST_ASYNC_PRINTFUNC
      return suspend_never{};
    }

    COROTEST_ASYNC_CONSTEXPR auto final_suspend() noexcept {
      COROTEST_ASYNC_PRINTFUNC
      return suspend_never{};
    }

    void return_value(T value) noexcept {
      COROTEST_ASYNC_PRINTFUNC
      value_ = std::move(value);
      if (consumer_) {
        // Resume the consumer if it is suspended.
        consumer_.resume();
      }
    }

    void unhandled_exception() noexcept {
      COROTEST_ASYNC_PRINTFUNC
      std::abort();
    }

#if COROTEST_DEBUG_ASYNC
    promise_type() noexcept {
      COROTEST_ASYNC_PRINTFUNC
    }

    ~promise_type() {
      COROTEST_ASYNC_PRINTFUNC
    }
#endif

    std::optional<T> value_;
    coroutine_handle<> consumer_ = nullptr;
  };

  using handle_type = coroutine_handle<promise_type>;

  async(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
    COROTEST_ASYNC_PRINTFUNC
  }

#if COROTEST_DEBUG_ASYNC
  ~async() {
    COROTEST_ASYNC_PRINTFUNC
  }
#endif

  bool await_ready() noexcept {
    COROTEST_ASYNC_PRINTFUNC
    return !!handle_.promise().value_;
  }

  void await_suspend(coroutine_handle<> consumer) noexcept {
    COROTEST_ASYNC_PRINTFUNC
    if (await_ready()) {
      // Resume the consumer if a value is ready.
      consumer.resume();
    } else {
      // Suspend the consumer and let promise_type::return_value() resume it.
      handle_.promise().consumer_ = consumer;
    }
  }

  auto& await_resume() noexcept {
    COROTEST_ASYNC_PRINTFUNC
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
      COROTEST_GENERATOR_PRINTFUNC
      return { *this };
    }

    COROTEST_GENERATOR_CONSTEXPR auto initial_suspend() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return suspend_never{};
    }

    COROTEST_GENERATOR_CONSTEXPR auto final_suspend() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return suspend_always{};
    }

    COROTEST_GENERATOR_CONSTEXPR auto return_void() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return suspend_never{};
    }

    auto yield_value(T& value) noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      value_ = std::addressof(value);
      return suspend_always{};
    }

    void unhandled_exception() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      std::abort();
    }

#if COROTEST_DEBUG_GENERATOR
    promise_type() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
    }

    ~promise_type() {
      COROTEST_GENERATOR_PRINTFUNC
    }
#endif

    T* value_ = nullptr;
  };

  using handle_type = coroutine_handle<promise_type>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

#if COROTEST_DEBUG_GENERATOR
    iterator() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
    }
#else
    iterator() noexcept = default;
#endif

    iterator(handle_type& handle) noexcept : handle_(std::addressof(handle)) {
      COROTEST_GENERATOR_PRINTFUNC
    }

#if COROTEST_DEBUG_GENERATOR
    ~iterator() {
      COROTEST_GENERATOR_PRINTFUNC
      if (handle_) {
        handle_->destroy();
      }
    }
#endif

    iterator& operator++() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      handle_->resume();
      if (handle_->done()) {
        handle_->destroy();
        handle_ = nullptr;
      }
      return *this;
    }

    iterator operator++(int) = delete;

    bool operator==(const iterator& other) const noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return handle_ == other.handle_;
    }

    bool operator!=(const iterator& other) const noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return handle_ != other.handle_;
    }

    reference operator*() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return *handle_->promise().value_;
    }

    pointer operator->() noexcept {
      COROTEST_GENERATOR_PRINTFUNC
      return handle_->promise().value_;
    }

    handle_type* handle_ = nullptr;
  };

  generator(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
    COROTEST_GENERATOR_PRINTFUNC
  }

#if COROTEST_DEBUG_GENERATOR
  ~generator() {
    COROTEST_GENERATOR_PRINTFUNC
  }
#endif

  iterator begin() noexcept {
    COROTEST_GENERATOR_PRINTFUNC
    return { handle_ };
  }

  iterator end() noexcept {
    COROTEST_GENERATOR_PRINTFUNC
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
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
      return { *this };
    }

    COROTEST_ASYNC_GENERATOR_CONSTEXPR auto initial_suspend() noexcept {
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
      return suspend_never{};
    }

    COROTEST_ASYNC_GENERATOR_CONSTEXPR auto final_suspend() noexcept {
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
      return suspend_always{};
    }

    COROTEST_ASYNC_GENERATOR_CONSTEXPR auto return_void() noexcept {
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
      return suspend_never{};
    }

    auto yield_value(T& value) noexcept {
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
      value_ = std::addressof(value);
      return suspend_always{};
    }

    void unhandled_exception() noexcept {
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
      std::abort();
    }

#if COROTEST_DEBUG_ASYNC_GENERATOR
    promise_type() noexcept {
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
    }

    ~promise_type() {
      printf(">>> ");  // this should not happen until the end
      COROTEST_ASYNC_GENERATOR_PRINTFUNC
    }
#endif

    T* value_ = nullptr;
    coroutine_handle<> consumer_ = nullptr;
  };

  using handle_type = coroutine_handle<promise_type>;

  struct iterator {
    using iterator_category = std::input_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

#if COROTEST_DEBUG_ASYNC_GENERATOR
    iterator() noexcept {
      printf("ITERATOR %d: iterator()\n", index_);
    }
#else
    iterator() noexcept = default;
#endif

    iterator(handle_type& handle) noexcept : handle_(std::addressof(handle)) {
      printf("ITERATOR %d: iterator(handle_type& handle)\n", index_);
    }

#if COROTEST_DEBUG_ASYNC_GENERATOR
    ~iterator() {
      printf(">>> ");  // this should not happen until the end
      printf("ITERATOR %d: ~iterator()\n", index_);
      if (handle_) {
        handle_->destroy();
      }
    }
#endif

    iterator& operator++() noexcept {
      printf("ITERATOR %d: operator++()\n", index_);
      handle_->resume();
      if (handle_->done()) {
        handle_->destroy();
        handle_ = nullptr;
      }
      return *this;
    }

    iterator operator++(int) = delete;

    bool operator==(const iterator& other) const noexcept {
      printf("ITERATOR %d: operator==(%d)\n", index_, other.index_);
      return handle_ == other.handle_;
    }

    bool operator!=(const iterator& other) const noexcept {
      printf("ITERATOR %d: operator!=(%d)\n", index_, other.index_);
      return handle_ != other.handle_;
    }

    reference operator*() noexcept {
      printf("ITERATOR %d: operator*()\n", index_);
      return *handle_->promise().value_;
    }

    pointer operator->() noexcept {
      printf("ITERATOR %d: operator->()\n", index_);
      return handle_->promise().value_;
    }

    bool await_ready() noexcept {
      printf("ITERATOR %d: await_ready()\n", index_);
      return handle_->promise().value_ != nullptr;
    }

    void await_suspend(coroutine_handle<> consumer) noexcept {
      printf("ITERATOR %d: await_suspend(coroutine_handle<> consumer)\n", index_);
      //if (await_ready()) {
      //  // Resume the consumer if a value is ready.
      //  puts("RESUMING CONSUMER");
      //  consumer.resume();
      //} else {
      //  // Suspend the consumer and let promise_type::return_value() resume it.
      //  //handle_.promise().consumer_ = consumer;
      //  puts("THIS SHOULD NOT HALLEN YET!");
      //}
    }

    iterator& await_resume() noexcept {
      printf("ITERATOR %d: await_resume()\n", index_);
      //auto handle = std::exchange(handle_, nullptr);
      return *this;
    }

    handle_type* handle_ = nullptr;

    static int iterator_index() noexcept {
      static int index = 0;
      return index++;
    }
    int index_ = iterator_index();
  };

  async_generator(promise_type& promise) noexcept : handle_(handle_type::from_promise(promise)) {
    COROTEST_ASYNC_GENERATOR_PRINTFUNC
  }

#if COROTEST_DEBUG_ASYNC_GENERATOR
  ~async_generator() {
    COROTEST_ASYNC_GENERATOR_PRINTFUNC
  }
#endif

  iterator& begin() noexcept {
    COROTEST_ASYNC_GENERATOR_PRINTFUNC
    return iterator_;
  }

  iterator end() noexcept {
    COROTEST_ASYNC_GENERATOR_PRINTFUNC
    return {};
  }

private:
  handle_type handle_ = nullptr;
  iterator iterator_ = { handle_ };
};

// ================================================================================================
