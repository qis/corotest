# CoroTest
Dependencies: Visual Studio 2017 or LLVM >= 5.0.0

## Submodules
This repo contains the [third_party/asio](https://github.com/chriskohlhoff/asio/)
and [third_party/cppcoro](https://github.com/lewissbaker/cppcoro) submodules.
Please clone it with `git clone --recursive`.

## Problem
Currently the `for co_await(` loop in `main.cpp` line 70 works as it should.

If the for loop is manually aborted with `break` or `co_return` before it can
finish, then the `async_generator` promise is leaked.
