# CoroTest
Dependencies: Visual Studio 2017 or LLVM >5.0.0

## LLVM
If you're on Ubuntu 16.04 or in Windows 10 WSL, just install LLVM from sources using this guide:

<https://github.com/qis/windows-10/blob/master/llvm.md>

## LLVM Output (Broken)
This is what you get if you run `make run` in WSL.

```
task::promise_type::promise_type()
promise_type& task::promise_type::get_return_object()
suspend_never task::promise_type::initial_suspend()
async_generator::promise_type::promise_type()
promise_type& async_generator::promise_type::get_return_object()
suspend_always async_generator::promise_type::initial_suspend()
async_generator::async_generator(promise_type& promise)
async_generator::promise_type::~promise_type()
await_iterator<T, promise_type> async_generator::begin()
await_iterator::await_iterator(coroutine_handle<GeneratorPromise> coro)
bool await_iterator::await_ready()
void await_iterator::await_suspend(coroutine_handle<> await_iterator)
resume co_yield
makefile:14: recipe for target 'run' failed
make: *** [run] Segmentation fault (core dumped)
```

## MSVC Output (Expected)
This is what you get if you open the solution in Visual Studio and press CTRL+F5.

```
task::promise_type::promise_type()
suspend_never task::promise_type::initial_suspend()
promise_type& task::promise_type::get_return_object()
task::task(promise_type& promise)
async_generator::promise_type::promise_type()
suspend_always async_generator::promise_type::initial_suspend()
promise_type& async_generator::promise_type::get_return_object()
async_generator::async_generator(promise_type& promise)
await_iterator<T, promise_type> async_generator::begin()
await_iterator::await_iterator(coroutine_handle<GeneratorPromise> coro)
bool await_iterator::await_ready()
void await_iterator::await_suspend(coroutine_handle<> await_iterator)
first resume
task::~task()
await_consumer<T, promise_type> async_generator::promise_type::yield_value(T& value)
await_consumer::await_consumer(coroutine_handle<GeneratorPromise> coro)
bool await_consumer::await_ready()
void await_consumer::await_suspend(coroutine_handle<> await_consumer)
async_iterator<T, GeneratorPromise> await_iterator::await_resume()
async_iterator::async_iterator(coroutine_handle<GeneratorPromise> coro)
async_iterator::~async_iterator()
await_iterator::~await_iterator()
await_iterator<T, promise_type> async_generator::end()
async_iterator::async_iterator(std::nullptr_t)
bool async_iterator::operator!=(const async_iterator& other)
reference async_iterator::operator*()
1
await_iterator<T, GeneratorPromise> async_iterator::operator++()
await_iterator::await_iterator(async_iterator<T, GeneratorPromise>* it)
bool await_iterator::await_ready()
void await_iterator::await_suspend(coroutine_handle<> await_iterator)
resume co_yield
void await_consumer::await_resume()
await_consumer::~await_consumer()
await_consumer<T, promise_type> async_generator::promise_type::yield_value(T& value)
await_consumer::await_consumer(coroutine_handle<GeneratorPromise> coro)
bool await_consumer::await_ready()
void await_consumer::await_suspend(coroutine_handle<> await_consumer)
async_iterator<T, GeneratorPromise> await_iterator::await_resume()
async_iterator::~async_iterator()
await_iterator::~await_iterator()
bool async_iterator::operator!=(const async_iterator& other)
reference async_iterator::operator*()
2
await_iterator<T, GeneratorPromise> async_iterator::operator++()
await_iterator::await_iterator(async_iterator<T, GeneratorPromise>* it)
bool await_iterator::await_ready()
void await_iterator::await_suspend(coroutine_handle<> await_iterator)
resume co_yield
void await_consumer::await_resume()
await_consumer::~await_consumer()
await_consumer<T, promise_type> async_generator::promise_type::yield_value(T& value)
await_consumer::await_consumer(coroutine_handle<GeneratorPromise> coro)
bool await_consumer::await_ready()
void await_consumer::await_suspend(coroutine_handle<> await_consumer)
async_iterator<T, GeneratorPromise> await_iterator::await_resume()
async_iterator::~async_iterator()
await_iterator::~await_iterator()
bool async_iterator::operator!=(const async_iterator& other)
reference async_iterator::operator*()
3
await_iterator<T, GeneratorPromise> async_iterator::operator++()
await_iterator::await_iterator(async_iterator<T, GeneratorPromise>* it)
bool await_iterator::await_ready()
void await_iterator::await_suspend(coroutine_handle<> await_iterator)
resume co_yield
void await_consumer::await_resume()
await_consumer::~await_consumer()
void async_generator::promise_type::return_void()
async_iterator<T, GeneratorPromise> await_iterator::await_resume()
async_iterator::~async_iterator()
await_iterator::~await_iterator()
bool async_iterator::operator!=(const async_iterator& other)
async_iterator::~async_iterator()
async_iterator::~async_iterator()
async_generator::~async_generator()
await_consumer::~await_consumer()
async_generator::promise_type::~promise_type()
void task::promise_type::return_void()
suspend_never task::promise_type::final_suspend()
task::promise_type::~promise_type()
suspend_always async_generator::promise_type::final_suspend()
```
